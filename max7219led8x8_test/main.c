/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/MAX7219LED8x8 project.
 *
 * Copyright (c) 2017 Neven Boyanov, The Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Retain in your source code the link http://tinusaur.org to the Tinusaur project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// ============================================================================

// #define F_CPU 1000000UL
// NOTE: The F_CPU (CPU frequency) should not be defined in the source code.
//       It should be defined in either (1) Makefile; or (2) in the IDE. 

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

// If you need to change the ports for the DIN/CS/CLK you should do so
// in the "max7219led8x8.h" source code file in the MAX7219LED8x8 library 
// so it will take affect on all the code.

#include "max7219led8x8/max7219led8x8.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 ATtiny85
//               +----------+   (-)--GND--
//       (RST)---+ PB5  Vcc +---(+)--VCC--
// ---[OWOWOD]---+ PB3  PB2 +---------DIN-
// --------------+ PB4  PB1 +---------CS--
// --------(-)---+ GND  PB0 +---------CLK-
//               +----------+
//              Tinusaur Board
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ----------------------------------------------------------------------------

#define MAX7219_SEG_NUM 4	// Segments, number of 8x8 matrices
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8

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

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

// ----------------------------------------------------------------------------

int main(void) {

	// ---- Initialization ----
	max7219b_init(max7219_buffer, MAX7219_BUFFER_SIZE);

	// ---- Main Loop ----
	for (;;) {
		for (uint8_t i = 0; i <= sizeof(data) / 8 - 1; i++) {
			for (uint8_t x = 0; x <= 7; x++) {
				uint8_t d = data[i][x];
				for (uint8_t y = 0; y <= 7; y++) {
					for (uint8_t s = 0; s < MAX7219_SEG_NUM - 1; s++) { // Loop through the segments
						if ((d & 1) == 1) {
							max7219b_set(x + (s << 3), y);	// Set pixel
						} else {
							max7219b_clr(x + (s << 3), y);	// Clear pixel
						}
						max7219b_out();	// Output the buffer
						_delay_ms(20);	// Delay. Remove this for quicker drawing
					}
					d >>= 1;
				}
			}
			_delay_ms(500);
		}
	}

	// Return the mandatory for the "main" function int value. It is "0" for success.
	return 0;
}

// ============================================================================
