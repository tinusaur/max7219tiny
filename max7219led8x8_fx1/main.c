/**
 * MAX7219LED8x8fx - Testing script.
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
#include <avr/pgmspace.h>

#include "tinyavrlib/scheduler.h"

#include "max7219led8x8/max7219led8x8.h"
// If you need to change the ports for the DIN/CS/CLK you should do so
// in the "max7219led8x8.h" source code file in the MAX7219LED8x8 library 
// so it will take affect on all the code.
#include "max7219led8x8/max7219led8x8fx.h"

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
// NOTE: One extra element added at the end of the buffer for a "hidden" symbol to scroll in.
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

	max7219b_init(max7219_buffer, MAX7219_BUFFER_SIZE);
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
		max7219fx_flash(3, MAX7219FX_SCROLL_DELAY);
		_delay_ms(MAX7219FX_SCROLL_DELAY << 6);
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
