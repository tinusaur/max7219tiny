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

#define MAX7219_SEG_NUM 2	// The number of the segments. Change this for multiple matrices.
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8		// The size of the buffer

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

// ----------------------------------------------------------------------------

uint8_t pattern_smileyface[] = { // Smiley face :)
	0b00111100,
	0b01000010,
	0b10010101,
	0b10100001,
	0b10100001,
	0b10010101,
	0b01000010,
	0b00111100,
};

// ----------------------------------------------------------------------------
// NOTE: This currently uses on only the first 2 matrices on the chain.
// TODO: Make it use all of the any 2+ matrices in a chain.

int main(void) {
	// ---- Initialization ----
	max7219b_init(MAX7219_SEG_NUM, max7219_buffer, MAX7219_BUFFER_SIZE);
	// ---- Main Loop ----
	uint8_t x = 0;	// Position X
	uint8_t s = 1;	// Step - left(-1) or right(+1)
	for (;;) {
		for (uint8_t i = 0; i <= 7; i++) {
			max7219b_col(x + i, pattern_smileyface[i]);
			max7219b_out();	// Output the buffer.
		}
		x += s;
		if (x > 0) max7219b_col(x - 1, 0);	// Clean on the left
		if (x < MAX7219_SEG_NUM * 8 - 8) max7219b_col(x + 8, 0);	// Clean on the right
		if (x >= MAX7219_SEG_NUM * 8 - 8) s = -1;		// Change direction to the left
		if (x <= 0) s = 1;		// Change direction to the right
		_delay_ms(20);
	}
	return 0; // Return the mandatory result value.
}

// ============================================================================
