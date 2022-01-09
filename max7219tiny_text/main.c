/**
 * Max7219Tiny - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/Max7219Tiny project.
 *
 * Copyright (c) 2018 Neven Boyanov, The Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Retain in your source code the link http://tinusaur.org to the Tinusaur project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219tiny
 *
 */

// ============================================================================

// NOTE: About F_CPU - it should be set in either (1) Makefile; or (2) in the IDE.

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "max7219tiny/max7219tiny.h"
#include "max7219tiny/max7219tinyf.h"
// If you need to change the ports for the DIN/CS/CLK you should do so
// in the "max7219tiny.h" source code file in the Max7219Tiny library 
// so it will take affect on all the code.

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 ATtiny85
//               +----------+   (-)--GND--
//       (RST)---+ PB5  Vcc +---(+)--VCC--
//  --[OWOWOD]---+ PB3  PB2 +------[DIN]--
//  -------------+ PB4  PB1 +-------[CS]--
//  --GND--(-)---+ GND  PB0 +------[CLK]--
//               +----------+
//              Tinusaur Board
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ----------------------------------------------------------------------------

#define MAX7219_SEG_NUM (1+1)	// The number of the segments. Increase this for multiple matrices.
// NOTE: Add 1 extra element at the end of the buffer for a "hidden" symbol to scroll in.
#define MAX7219_SEG_LAST (MAX7219_SEG_NUM - 1) * 8	// The index in the buffer of the last segment.
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8		// The size of the buffer

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

// ----------------------------------------------------------------------------

// TODO: Move this function to a library.

void max7219b_char(uint8_t x, uint8_t c) {
	for(uint8_t i = 0; i <= 7; i++)
		max7219b_col(x + i, pgm_read_byte(&max7219tiny_font[((c - 32) << 3) + i]));
}

// ----------------------------------------------------------------------------

int main(void) {

	// ---- Initialization ----
	max7219b_init(max7219_buffer, MAX7219_BUFFER_SIZE);

	// ---- Main Loop ----
	for (;;) {
		for (uint8_t c = ' '; c <= 127; c++) {
			max7219b_char((MAX7219_SEG_NUM - 1) * 8, c);
			max7219b_out();	// Output the buffer
			for (uint8_t s = 0; s < 8; s++) {
				max7219b_left();	// Scroll to the left.
				max7219b_out();		// Manually output the buffer.
				_delay_ms(40);
			}
		}
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
