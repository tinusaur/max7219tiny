/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/MAX7219LED8x8 project.
 *
 * Copyright (c) 2018 Neven Boyanov, The Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Retain in your source code the link http://tinusaur.org to the Tinusaur project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// ============================================================================

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "max7219led8x8.h"

// ----------------------------------------------------------------------------

void max7219_byte(uint8_t data) {
	for(uint8_t i = 8; i >= 1; i--) {
		PORTB &= ~(1 << MAX7219_CLK);	// Set CLK to LOW
		if (data & 0x80)				// Mask the MSB of the data
			PORTB |= (1 << MAX7219_DIN);	// Set DIN to HIGH
		else
			PORTB &= ~(1 << MAX7219_DIN);	// Set DIN to LOW
		PORTB |= (1 << MAX7219_CLK);		// Set CLK to HIGH
		data <<= 1;						// Shift to the left
	}
}

void max7219_word(uint8_t address, uint8_t data) {
	PORTB &= ~(1 << MAX7219_CS);	// Set CS to LOW
	max7219_byte(address);			// Sending the address
	max7219_byte(data);				// Sending the data
	PORTB |= (1 << MAX7219_CS);		// Set CS to HIGH
	PORTB &= ~(1 << MAX7219_CLK);	// Set CLK to LOW
}

void max7219_init(void) {
	uint8_t initseq[] = {
		0x09, 0x00,	// Decode-Mode Register, 00 = No decode
		0x0a, 0x01,	// Intensity Register, 0x00 .. 0x0f
		0x0b, 0x07,	// Scan-Limit Register, 0x07 to show all lines
		0x0c, 0x01,	// Shutdown Register, 0x01 = Normal Operation
		0x0f, 0x00,	// Display-Test Register, 0x01, 0x00 = Normal Operation
		0x00, 0x00, // Necessary when cascading (reason: unknown)
	};
	DDRB |= (1 << MAX7219_CLK);	// Set CLK port as output
	DDRB |= (1 << MAX7219_CS);	// Set CS port as output
	DDRB |= (1 << MAX7219_DIN);	// Set DIN port as output
	// PORTB &= ~(1 << MAX7219_CLK);	// Set CLK to LOW, // TODO: Q: Is this necessary?
	// _delay_ms(50);	// Wait, TODO: Q: Is this necessary?
	uint8_t i = 0;
	while (i < sizeof(initseq)) {
		uint8_t opcode = initseq[i++];
		uint8_t opdata = initseq[i++];
		max7219_word(opcode, opdata);
	}
	// TODO: Remove this unused code
	/*
	// Replaced by more optimized version.
	for (uint8_t i = 0; i < sizeof(initseq); ) {
		uint8_t opcode = initseq[i++];
		uint8_t opdata = initseq[i++];
		PORTB &= ~(1 << MAX7219_CLK);	// Set CLK to LOW
		PORTB &= ~(1 << MAX7219_CS);	// Set CS to LOW (start of transmission)
		for (uint8_t seg = 0; seg < seg_num; seg++) {
			max7219_byte(opcode);	// Send the opcode out.
			max7219_byte(opdata);	// Send the opdata out.
		}
		PORTB |= (1 << MAX7219_CS);		// Set CS to HIGH (end of transmission)
		PORTB &= ~(1 << MAX7219_CLK);	// Set CLK to LOW
	}
	*/
}

// NOTE: address is from 1 to 8
void max7219_row(uint8_t address, uint8_t data) {
	if (address >= 1 && address <= 8) max7219_word(address, data);
}

// TODO: Write function to clear the buffer.

// ----------------------------------------------------------------------------

uint8_t *__max7219_buffer;	// TODO: Is it needed ... = __max7219_buffer_int;
uint8_t __max7219_buffer_size;	// TODO: Is it needed ...  = MAX7219_BUFFER_SIZE;

// ----------------------------------------------------------------------------

void max7219b_init(uint8_t *buffer, uint8_t buffer_size) {
	max7219_init();
	__max7219_buffer = buffer;
	__max7219_buffer_size = buffer_size;
}

void max7219b_out(void) {
	uint8_t bit_mask = 0x80;
	for (uint8_t row = 0; row <= 7; row++) {
		int8_t buffer_seg = __max7219_buffer_size - 8;
		PORTB &= ~(1 << MAX7219_CS);	// Set CS to LOW (start of transmission)
		while (buffer_seg >= 0) {	// Loop until the last segment is processed
			max7219_byte(row + 1);	// Send the address out.
			// Then, send the data out ...
			for(int8_t index = 7; index >= 0; index--) {	// NOTE: Must be signed int.
				// Send 1 bit out.
				PORTB &= ~(1 << MAX7219_CLK);	// Set CLK to LOW
				if (__max7219_buffer[buffer_seg + index] & bit_mask) // Mask the bit of the data
					PORTB |= (1 << MAX7219_DIN);	// Set DIN to HIGH
				else
					PORTB &= ~(1 << MAX7219_DIN);	// Set DIN to LOW
				PORTB |= (1 << MAX7219_CLK);		// Set CLK to HIGH
			}
			buffer_seg -= 8;	// Decrease the buffer segment index.
		}
		PORTB |= (1 << MAX7219_CS);		// Set CS to HIGH (end of transmission)
		PORTB &= ~(1 << MAX7219_CLK);	// Set CLK to LOW
		bit_mask >>= 1;	// Shift bit mask to the left.
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

// Task to be executed by the system scheduler.
// NOTE: Asynchronous mode function.
void max7219b_scheduler_usertask(scheduler_status_p scheduler) {
	max7219b_out();
}

// ============================================================================
