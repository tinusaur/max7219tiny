/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @created 2014-07-12
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/MAX7219LED8x8 project.
 *
 * Copyright (c) 2016 Neven Boyanov, Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Please, as a favor, retain the link http://tinusaur.org to The Tinusaur Project.
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
	// PORTB &= ~(1 << MAX7219_CS);	// Set to LOW, not necessary - done in the max7219_word
	for(uint8_t i = 8; i >= 1; i--) {
		PORTB &= ~(1 << MAX7219_CLK);	// Set CLK to LOW
		if (data & 0x80)				// Mask the MSB of the data
			PORTB |= (1 << MAX7219_DIN);	// Set DIN to HIGH
		else
			PORTB &= ~(1 << MAX7219_DIN);	// Set DIN to LOW
		data <<= 1;
		PORTB |= (1 << MAX7219_CLK);		// Set CLK to HIGH
	}
}

void max7219_word(uint8_t address, uint8_t data) {
	PORTB &= ~(1 << MAX7219_CS);	// Set CS to LOW
	max7219_byte(address);			//
	max7219_byte(data);				//
	PORTB |= (1 << MAX7219_CS);		// Set CS to HIGH
	PORTB &= ~(1 << MAX7219_CLK);	// Set CLK to LOW
}

void max7219_init(void) {
	DDRB |= (1 << MAX7219_CLK);	// Set CLK port as output
	DDRB |= (1 << MAX7219_CS);	// Set CS port as output
	DDRB |= (1 << MAX7219_DIN);	// Set DIN port as output
	_delay_ms(50);	// TODO: Q: Is this necessary?
	max7219_word(0x09, 0x00);	// Decode-Mode Register, 00 = No decode
	max7219_word(0x0a, 0x01);	// Intensity Register, 0x00 .. 0x0f
	max7219_word(0x0b, 0x07);	// Scan-Limit Register, 0x07 to show all lines
	max7219_word(0x0c, 0x01);	// Shutdown Register, 0x01 = Normal Operation
	max7219_word(0x0f, 0x00);	// Display-Test Register, 0x00 = Normal Operation
}

void max7219_row(uint8_t address, uint8_t data) {
	if (address >= 1 && address <= 8) max7219_word(address, data);
}

// TODO: Write function to clear the buffer.

// ----------------------------------------------------------------------------

#define MAX7219_SEG_NUM		1	// Segments, number of 8x8 matrices
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8

// ----------------------------------------------------------------------------

uint8_t __max7219_buffer_int[MAX7219_BUFFER_SIZE]; // = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t *__max7219_buffer = __max7219_buffer_int;
uint8_t __max7219_buffer_size = MAX7219_BUFFER_SIZE;

// uint8_t __max7219_buffer_x_mask = MAX7219_BUFFER_SIZE - 1;
// NOTE: The __max7219_buffer_x_mask could be used as bit mask for buffer overrun protection.
// If X is larger that the max allowed value the MSB will be trimmed by the bit mask.
// IMPORTANT: This will work correctly ONLY for __max7219_buffer_size values that are 2^n.

// TODO: replace this with pointer to buffer specified by the calling application.
// TODO: buffer length should be also specified.

// TODO: Add parameter, init scheduler, optional (if not null)
void max7219b_init(uint8_t *buffer, uint8_t buffer_size) {
	max7219_init();
	__max7219_buffer = buffer;
	__max7219_buffer_size = buffer_size;
	// __max7219_buffer_x_mask = __max7219_buffer_size - 1;
}

void max7219b_out(void) {
	uint8_t bit_mask = 0x80;
	for (uint8_t row = 0; row <= 7; row++) {
		int8_t buffer_seg = __max7219_buffer_size - 8;
		PORTB &= ~(1 << MAX7219_CS);	// Set CS to LOW
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
			buffer_seg -= 8;	// Increase the buffer segment index.
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

void max7219bs_scheduler_userfunc(uint32_t scheduler_tick) {
	max7219b_out();
}

// ============================================================================
