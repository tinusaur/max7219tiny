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
#include <avr/io.h>
#include <util/delay.h>

#include "max7219led8x8.h"

// ----------------------------------------------------------------------------

void max7219_byte(uint8_t data) {
	PORTB &= ~(1 << MAX7219_CS);	// Set to LOW
	for(uint8_t i = 8; i >= 1; i--)
	{
		PORTB &= ~(1 << MAX7219_CLK);	// Set to LOW
		if ((data & 0x80) != 0)			// Mask the MSB of the data
			PORTB |= (1 << MAX7219_DIN);	// Set to HIGH
		else
			PORTB &= ~(1 << MAX7219_DIN);	// Set to LOW
		data = data<<1;
		PORTB |= (1 << MAX7219_CLK);		// Set to HIGH
	}
}

void max7219_word(uint8_t address, uint8_t data) {
	PORTB &= ~(1 << MAX7219_CS);	// Set to LOW
	max7219_byte(address);			//
	max7219_byte(data);				//
	PORTB |= (1 << MAX7219_CS);		// Set to HIGH
}

void max7219_init(void) {
	DDRB |= (1 << MAX7219_CLK);	// Set port as output
	DDRB |= (1 << MAX7219_CS);	// Set port as output
	DDRB |= (1 << MAX7219_DIN);	// Set port as output
	_delay_ms(50);	// TODO: Q: Is this necessary?
	max7219_word(0x09, 0x00);	// Decode: BCD
	max7219_word(0x0a, 0x03);	// Brightness
	max7219_word(0x0b, 0x07);	//
	max7219_word(0x0c, 0x01);	//
	max7219_word(0x0f, 0x00);	//
}

void max7219_row(uint8_t address, uint8_t data) {
	if (address >= 1 && address <= 8) max7219_word(address, data);
}

// ----------------------------------------------------------------------------

uint8_t max7219_buffer[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// TODO: replace this with pointer to buffer specified by the calling application.
// TODO: buffer length should be also specified.

void max7219b_out(void) {
	// Output the buffer
	for (uint8_t row = 1; row <= 8; row++)
		max7219_row(row, max7219_buffer[row - 1]);
}

void max7219b_set(uint8_t x, uint8_t y) {
	uint8_t sx = 7 - (x & 0b0111);
	uint8_t sy = (y & 0b0111);
	max7219_buffer[sy] |= (1 << sx);
}

void max7219b_clr(uint8_t x, uint8_t y) {
	uint8_t sx = 7 - (x & 0b0111);
	uint8_t sy = (y & 0b0111);
	max7219_buffer[sy] &= ~(1 << sx);
}

void max7219b_row(uint8_t row, uint8_t y) {
	max7219_buffer[y & 0b0111] = row;
}

// ----------------------------------------------------------------------------

void max7219bs_scheduler_userfunc(uint32_t scheduler_tick) {
	max7219b_out();
}

// ============================================================================
