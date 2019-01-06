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

// NOTE: About F_CPU - it should be set in either (1) Makefile; or (2) in the IDE.

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "max7219led8x8/max7219led8x8.h"
// If you need to change the ports for the DIN/CS/CLK you should do so
// in the "max7219led8x8.h" source code file in the MAX7219LED8x8 library
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

#define MAX7219_SEG_NUM 4	// Segments, number of 8x8 matrices

// ----------------------------------------------------------------------------

int main(void) {

	// ---- Initialization ----
	max7219_init(MAX7219_SEG_NUM);

	// ---- Main Loop ----
	for (;;) {
		for (uint8_t row = 0; row < 9; row++) { // 9 times, so both row will be output.
			uint8_t d = 1;
			for (uint8_t i = 9; i > 0; i--) { // 9 times, so the last is "0".
				max7219_row(row, d);
				max7219_row(row + 1, ~d);
				d = d << 1;
				_delay_ms(50);
			}
		}
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
