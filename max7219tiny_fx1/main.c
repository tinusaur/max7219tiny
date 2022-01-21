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
// If you need to change the ports for the DIN/CS/CLK you should do so
// in the "max7219tiny.h" source code file in the MAX7219tiny library
// so it will take affect on all the code.
#include "max7219tiny/max7219tinyfx.h"

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

#define MAX7219_SEG_NUM (2+1)	// The number of the segments. Increase this for multiple matrices.
// NOTE: Add 1 for an extra element at the end of the buffer for a "hidden" symbol to scroll in.
#define MAX7219_SEG_LAST (MAX7219_SEG_NUM - 1) * 8	// The index in the buffer of the last segment.
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8		// The size of the buffer

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

// ----------------------------------------------------------------------------

#include "font8x6_data.h"
#include "tinu16x8_data.h"

// ----------------------------------------------------------------------------

#define MAX7219FX_SCROLL_DELAY 4 // Delay in centisecond

char *text_hello = "Hello, World!";

int main(void) {
	// ---- Initialization ----
	scheduler_init(SCHEDULER_USERFUNC_NULL);
	scheduler_reinit(SCHEDULER_TCCR0B_1024, SCHEDULER_OCR0A_MIN);	// Adjust, if necessary
	scheduler_start();
	max7219b_init(MAX7219_SEG_NUM, max7219_buffer, MAX7219_BUFFER_SIZE);
	max7219b_scheduler();
	max7219fx_init(font8x6_data, ' ');
	
	// ---- Main Loop ----
	for (;;) {
		// Print some text
		char *text = text_hello;
		while (*text) {
			max7219fx_char(MAX7219_SEG_LAST, *text++);
			max7219fx_left(6, MAX7219FX_SCROLL_DELAY);
		}
		// Show a bitmap
		max7219fx_bmp(MAX7219_SEG_LAST, tinu16x8_data, 0, 8);
		max7219fx_left(8, MAX7219FX_SCROLL_DELAY);
		max7219fx_bmp(MAX7219_SEG_LAST, tinu16x8_data, 8, 8);
		max7219fx_left(8, MAX7219FX_SCROLL_DELAY);
		// Flash the screen
		_delay_ms(MAX7219FX_SCROLL_DELAY << 6);
		max7219fx_flash(3, MAX7219FX_SCROLL_DELAY << 1);
		_delay_ms(MAX7219FX_SCROLL_DELAY << 6);
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
