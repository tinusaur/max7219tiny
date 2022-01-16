/**
 * MAX7219tiny - Testing script.
 * @author Neven Boyanov
 * This is part of the Tinusaur/MAX7219tiny project.
 * ----------------------------------------------------------------------------
 *  Copyright (c) 2022 Tinusaur (https://tinusaur.com). All rights reserved.
 *  Distributed as open source under the MIT License (see the LICENSE.txt file)
 *  Please, retain in your work a link to the Tinusaur project website.
 * ----------------------------------------------------------------------------
 * Source code available at: https://gitlab.com/tinusaur/max7219tiny
 */

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "max7219tiny/max7219tiny.h"
// To reassign the DIN/CS/CLK to different I/O pins you must edit the library code
// Find the definitions in the "max7219tiny.h" file in the MAX7219Tiny library.
// IMPORTANT/NOTE: Do that ONLY of you know what you are doing!

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 ATtiny85
//               +----------+   (-)--GND---
//       (RST)---+ PB5  Vcc +---(+)--VCC---
//  --[OWOWOD]---+ PB3  PB2 +-------[CLK]--
//  -------------+ PB4  PB1 +-------[DIN]--
//  --GND--(-)---+ GND  PB0 +-------[CS]---
//               +----------+
//              Tinusaur Board
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(void) {
	// ---- Initialization ----
	max7219_init();
	// ---- Main Loop ----
	for (;;) {
		for (uint8_t r = 1; r <= 8; r++) { // 8 times, row values are 1 to 8.
			uint8_t d = 1;
			for (uint8_t i = 9; i > 0; i--) { // 9 times, so the last value is "0".
				max7219_row(r, d);
				d = d << 1;
				_delay_ms(100);
			}
		}
	}
	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
