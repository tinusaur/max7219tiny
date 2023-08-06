/**
 * MAX7219tiny - Tinusaur MAX7219 Library for LED 8x8 Matrix
 * @author Neven Boyanov
 * This is part of the Tinusaur/MAX7219tiny project.
 * ----------------------------------------------------------------------------
 *  Copyright (c) 2022 Tinusaur (https://tinusaur.com). All rights reserved.
 *  Distributed as open source under the MIT License (see the LICENSE.txt file)
 *  Please, retain in your work a link to the Tinusaur project website.
 * ----------------------------------------------------------------------------
 * Source code available at: https://gitlab.com/tinusaur/max7219tiny
 */

// ============================================================================

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "tinyavrlib/scheduler.h"

#include "max7219tiny.h"

// ----------------------------------------------------------------------------

#define MAX7219_CS_HI()		PORTB |= (1 << MAX7219_CS)
#define MAX7219_CS_LO() 	PORTB &= ~(1 << MAX7219_CS)

#define MAX7219_CLK_HI()	PORTB |= (1 << MAX7219_CLK)
#define MAX7219_CLK_LO()	PORTB &= ~(1 << MAX7219_CLK)

#define MAX7219_DIN_HI()	PORTB |= (1 << MAX7219_DIN)
#define MAX7219_DIN_LO()	PORTB &= ~(1 << MAX7219_DIN)

// ----------------------------------------------------------------------------

uint8_t __max7219_num;

// ----------------------------------------------------------------------------

void max7219_byte(uint8_t data) {
	for(uint8_t i = 8; i != 0; i--) {
		MAX7219_CLK_LO();		// Set CLK to LOW
		if (data & 0x80)		// Mask the MSB of the data
			MAX7219_DIN_HI();	// Set DIN to HIGH
		else
			MAX7219_DIN_LO();	// Set DIN to LOW
		MAX7219_CLK_HI();		// Set CLK to HIGH
		data <<= 1;				// Shift to the left
	}
}

void max7219_word(uint8_t addr, uint8_t data, uint8_t num) {
	MAX7219_CS_LO();		// Set CS to LOW ____ start of transmission
	for (uint8_t n = num; n != 0; n--) { // Send multiple times for cascaded matrices
		max7219_byte(addr);	// Sending the address
		max7219_byte(data);	// Sending the data
	}
	MAX7219_CS_HI();		// Set CS to HIGH \__ end of transmission
	MAX7219_CLK_LO();		// Set CLK to LOW /
}

const uint8_t max7219_initseq[] PROGMEM = {
	0x09, 0x00,	// Decode-Mode Register, 00 = No decode
	0x0a, 0x01,	// Intensity Register, 0x00 .. 0x0f
	0x0b, 0x07,	// Scan-Limit Register, 0x07 to show all lines
	0x0c, 0x01,	// Shutdown Register, 0x01 = Normal Operation
	0x0f, 0x00,	// Display-Test Register, 0x01, 0x00 = Normal Operation
	// 0x00, 0x00, // Necessary when cascading (reason: unknown)	// is this needed?
};

void max7219_init(uint8_t num) {
	DDRB |= (1 << MAX7219_CLK);	// Set CLK port as output
	DDRB |= (1 << MAX7219_CS);	// Set CS port as output
	DDRB |= (1 << MAX7219_DIN);	// Set DIN port as output
	__max7219_num = num;
	for (uint8_t i = 0; i < sizeof (max7219_initseq);) {
		uint8_t addr = pgm_read_byte(&max7219_initseq[i++]);
		uint8_t data = pgm_read_byte(&max7219_initseq[i++]);
		max7219_word(addr, data, __max7219_num);
	}
}

// NOTE: address is from 1 to 8
void max7219_row(uint8_t addr, uint8_t data) {
	if (addr >= 1 && addr <= 8) max7219_word(addr, data, __max7219_num);
}

// ----------------------------------------------------------------------------

uint8_t *__max7219_buffer;
uint8_t __max7219_buffer_size;

// ----------------------------------------------------------------------------

// TODO: Write function to clear the buffer.

void max7219b_init(uint8_t num, uint8_t *buffer, uint8_t buffer_size) {
	max7219_init(num);
	__max7219_buffer = buffer;
	__max7219_buffer_size = buffer_size;
}

void max7219b_out(void) {
	uint8_t bit_mask = 0x80;
	for (uint8_t row = 1; row <= 8; row++) {	// Loop through the buffer rows
		uint8_t buffer_seg = __max7219_buffer_size;	// Starting from the last segment of the buffer - the "-8" below.
		MAX7219_CS_LO();	// Set CS to LOW \___ start of transmission
		while (buffer_seg != 0) {	// Loop through the segments
			max7219_byte(row);		// Send the address out.
			// Send 8 bits - 1 from each column
			for (uint8_t index = 8; index != 0; index--) {	// Loop through the columns
				uint8_t col = __max7219_buffer[buffer_seg + index - 1 - 8];
				// Send 1 bit out.
				MAX7219_CLK_LO();	// Set CLK to LOW
				if (col & bit_mask) // Mask the bit of the data
					MAX7219_DIN_HI();	// Set DIN to HIGH
				else
					MAX7219_DIN_LO();	// Set DIN to LOW
				MAX7219_CLK_HI();		// Set CLK to HIGH
			}
			buffer_seg -= 8;		// Decrease the buffer segment index.
		}
		MAX7219_CS_HI();	// Set CS to HIGH \__ end of transmission
		MAX7219_CLK_LO();	// Set CLK to LOW /
		bit_mask >>= 1;		// Shift bit mask to the left.
	}
}

void max7219b_set(uint8_t x, uint8_t y) {
	if (x < __max7219_buffer_size) __max7219_buffer[x] |= (1 << y);
}

void max7219b_clr(uint8_t x, uint8_t y) {
	if (x < __max7219_buffer_size) __max7219_buffer[x] &= ~(1 << y);
}

void max7219b_col(uint8_t x, uint8_t data) {
	if (x < __max7219_buffer_size) __max7219_buffer[x] = data;
}

uint8_t max7219b_get(uint8_t x) {
	return __max7219_buffer[x];
}

void max7219b_left(void) {
	memcpy(__max7219_buffer, __max7219_buffer + 1, __max7219_buffer_size - 1);
}

/*
void max7219b_right(void) {
	memmove(__max7219_buffer + 1, __max7219_buffer, __max7219_buffer_size - 1);
	// NOTE: memmove takes more memory and resources.
}
*/

// ----------------------------------------------------------------------------

// Buffered output scheduler function.
// Task to be executed by the system scheduler.
// NOTE: Asynchronous mode function.
void max7219b_scheduler_usertask(void) {
	max7219b_out();
}

// Init the system scheduler with the library task.
// This should be if any of the functions with buffering are used.
void max7219b_scheduler() {
	scheduler_usertask(max7219b_scheduler_usertask, 60);
	// Note: The second argument could be used to specify the refresh rate.
}

// ============================================================================
