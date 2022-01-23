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
#include <avr/pgmspace.h>

#include "max7219tiny/max7219tiny.h"
// To reassign the DIN/CS/CLK to different I/O pins you must edit the library code
// Find the definitions in the "max7219tiny.h" file in the MAX7219Tiny library.
// IMPORTANT/NOTE: Do that ONLY of you know what you are doing!
#include "max7219tiny/max7219tinyfx.h"

#include "font6x3num.h"

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

#define MAX7219_SEG_NUM (4+1)	// The number of the segments. Increase this for multiple matrices.
// NOTE: Add 1 extra element at the end of the buffer for a "hidden" symbol to scroll in.
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8		// The size of the buffer

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

// ----------------------------------------------------------------------------

// NOTE: The font is 6x3.
void max7219tx_num(uint8_t x, uint8_t n) {
	uint8_t index = (n << 1) + n; // Fast multiply by 3.
	for (uint8_t i = 0; i < 3; i++) {
		uint8_t d = pgm_read_byte(&font6x3num_data[index++]);
		// d >>= 1; // Use "<<" to move one row up, ">>" one row down.
		max7219b_col(x++, d);
	}
	max7219b_col(x, 0); // Last (empty) column.
}

// ----------------------------------------------------------------------------

#define MAX7219FX_SCROLL_DELAY 2 // Delay in centisecond

int main(void) {
	// ---- Initialization ----
	scheduler_init(SCHEDULER_USERFUNC_NULL);
	scheduler_reinit(SCHEDULER_TCCR0B_1024, SCHEDULER_OCR0A_MIN);	// Adjust, if necessary
	scheduler_start();
	max7219b_init(MAX7219_SEG_NUM, max7219_buffer, MAX7219_BUFFER_SIZE);
	max7219b_scheduler();
	max7219fx_init();
	// ---- Main Loop ----
	for (;;) {
		for (uint8_t c = 0; c <= 15; c++) {
			max7219tx_num(MAX7219_BUFFER_SIZE - 8, c);
			max7219fx_left(4, MAX7219FX_SCROLL_DELAY);
		}
		_delay_ms(200);
	}
	return 0; // Return the mandatory result value.
}

// ============================================================================
