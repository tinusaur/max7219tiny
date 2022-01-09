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

#include "max7219tiny/max7219tiny.h"
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

int main(void) {

	// ---- Initialization ----
	max7219_init();

	// ---- Main Loop ----
	for (;;) {
		for (uint8_t row = 0; row < 9; row++) {	// 9 times, so both rows will be output.
			uint8_t d = 1;
			for (uint8_t i = 9; i > 0; i--) {	// 9 times, so the last is "0".
				max7219_row(row, d);			// Display 1 pixel (1 bit)
				max7219_row(row + 1, ~d);		// Display inverted value - 7 pixels.
				d = d << 1;
				_delay_ms(50);
			}
		}
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================