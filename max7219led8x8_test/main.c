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

// #define F_CPU 1000000UL
// NOTE: The F_CPU (CPU frequency) should not be defined in the source code.
//       It should be defined in either (1) Makefile; or (2) in the IDE. 

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

// Change the ports, if necessary.
// #define MAX7219_DIN		PB0	// DI,	Pin 3 on LED8x8 Board
// #define MAX7219_CS		PB1	// CS,	Pin 4 on LED8x8 Board
// #define MAX7219_CLK		PB2	// CLK,	Pin 5 on LED8x8 Board

#include "max7219led8x8/max7219led8x8.h"

//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 ATtiny
//                25/45/85
//              +----------+   (-)--GND--
//      (RST)---+ PB5  Vcc +---(+)--VCC--
// --[OWOWOD]---+ PB3  PB2 +--------CLK--
//           ---+ PB4  PB1 +---------CS--
// -------(-)---+ GND  PB0 +--------DIN--
//              +----------+
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

// ----------------------------------------------------------------------------

int main(void) {

	uint8_t data[][8] = {
	{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, },	// Pattern, full
	{	0b00000100,
		0b00000110,
		0b11111111,
		0b00100110,
		0b01100100,
		0b11111111,
		0b01100000,
		0b00100000,	},	// ARROWS #1 (bottom to top)
	{	0b00100000,
		0b01100000,
		0b11111111,
		0b01100100,
		0b00100110,
		0b11111111,
		0b00000110,
		0b00000100,	},	// ARROWS #2 (top to bottom)
	{	0b01111110,
		0b10000001,
		0b10010101,
		0b10100001,
		0b10100001,
		0b10010101,
		0b10000001,
		0b01111110,	},	// TEST smiley face :)
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },	// Pattern, empty
	};

#define MAX7219_SEG_NUM 2	// Segments, number of 8x8 matrices
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8

	uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

	// ---- Initialization ----
	max7219b_init(max7219_buffer, MAX7219_BUFFER_SIZE);
	
	// ---- Main Loop ----
	for (;;) {
		for (uint8_t i = 0; i <= sizeof(data) / 8 - 1; i++) {
			for (uint8_t x = 0; x <= 7; x++) {
				uint8_t d = data[i][x];
				for (uint8_t y = 0; y <= 7; y++) {
					if ((d & 1) == 1) {
						max7219b_set(x, y);	// Set pixel
						max7219b_clr(x + 8, y);	// Set pixel
					} else {
						max7219b_clr(x, y);	// Clear pixel
						max7219b_set(x + 8, y);	// Clear pixel
					}
					d >>= 1;
					max7219b_out();	// Output the buffer
					_delay_ms(5);
				}
			}
			_delay_ms(1000);
		}
	}

	return 0;
}

// ============================================================================
