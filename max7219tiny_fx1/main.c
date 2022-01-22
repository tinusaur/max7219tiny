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

#include "tinyavrlib/scheduler.h"

#include "max7219tiny/max7219tiny.h"
// To reassign the DIN/CS/CLK to different I/O pins you must edit the library code
// Find the definitions in the "max7219tiny.h" file in the MAX7219Tiny library.
// IMPORTANT/NOTE: Do that ONLY of you know what you are doing!
#include "max7219tiny/max7219tinyfx.h"

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

#define MAX7219_SEG_NUM (4+2)	// The number of the segments. Increase this for multiple matrices.
// NOTE: Add 1 or more for an extra element at the end of the buffer for "hidden" symbols to scroll in.
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8		// The size of the buffer

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

#define MAX7219_SEG_LAST (MAX7219_SEG_NUM - 1) * 8	// The index in the buffer of the last segment.

// ----------------------------------------------------------------------------

#include "tinu16x8_data.h"

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
		// Show a bitmap
		max7219fx_bmp(MAX7219_BUFFER_SIZE - 16, tinu16x8_data, 0, 16);
		max7219fx_left(16, MAX7219FX_SCROLL_DELAY);
		_delay_ms(MAX7219FX_SCROLL_DELAY << 4);
		// Flash the screen
		max7219fx_flash(3, MAX7219FX_SCROLL_DELAY);
		_delay_ms(MAX7219FX_SCROLL_DELAY << 4);
	}
	return 0; // Return the mandatory result value.
}

// ============================================================================
