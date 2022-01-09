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
//               +----------+   (-)--GND--
//       (RST)---+ PB5  Vcc +---(+)--VCC--
//  --[OWOWOD]---+ PB3  PB2 +------[DIN]--
//  -------------+ PB4  PB1 +-------[CS]--
//  --GND--(-)---+ GND  PB0 +------[CLK]--
//               +----------+
//              Tinusaur Board
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ----------------------------------------------------------------------------

#define MAX7219_SEG_NUM (2+1)	// The number of the segments. Increase this for multiple matrices.
// NOTE: Add 1 extra element at the end of the buffer for a "hidden" symbol to scroll in.
#define MAX7219_SEG_LAST (MAX7219_SEG_NUM - 1) * 8	// The index in the buffer of the last segment.
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8		// The size of the buffer

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

uint8_t data[][8] = {
	{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, },	// Pattern, full
	{
		0b00000100,
		0b00000110,
		0b11111111,
		0b00100110,
		0b01100100,
		0b11111111,
		0b01100000,
		0b00100000,
	},	// ARROWS #1 (bottom to top)
	{
		0b00100000,
		0b01100000,
		0b11111111,
		0b01100100,
		0b00100110,
		0b11111111,
		0b00000110,
		0b00000100,
	},	// ARROWS #2 (top to bottom)
	{
		0b01111110,
		0b10000001,
		0b10010101,
		0b10100001,
		0b10100001,
		0b10010101,
		0b10000001,
		0b01111110,
	},	// TEST smiley face :)
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },	// Pattern, empty
};

// ----------------------------------------------------------------------------

int main(void) {

	// ---- Initialization ----
	max7219b_init(max7219_buffer, MAX7219_BUFFER_SIZE);

	// ---- Main Loop ----
	for (;;) {
		for (uint8_t i = 0; i <= sizeof(data) / 8 - 1; i++) {	// Loop through the patterns.
			for (uint8_t x = 0; x <= 7; x++) {	// Loop through each byte of the pattern.
				uint8_t d = data[i][x];	// Read one byte of data.
				for (uint8_t y = 0; y <= 7; y++) {	// Loop through each bit of the
					for (uint8_t s = 0; s <= MAX7219_SEG_NUM - 1; s++) { // Loop through the matrices.
						// NOTE: This "for" cycle and the use of the "s" variable is necessary
						//       only if more than one matrix is present in the hardware.
						if ((s & 1) == 1) d ^= 1;	// Trick: Flip the data bit for even/odd matrix.
						if ((d & 1) == 1) { // Check the bit if it is "0" or "1"
							max7219b_set(x + (s << 3), y);	// Set pixel
						} else {
							max7219b_clr(x + (s << 3), y);	// Clear pixel
						}
						max7219b_out();	// Manually output the buffer.
						// _delay_ms(10);	// Delay. Remove this for quicker drawing.
					}
					d >>= 1;	// Shift bits to the right
				}
			}
			_delay_ms(500);
		}
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
