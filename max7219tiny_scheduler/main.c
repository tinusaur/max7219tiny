/**
 * Max7219Tiny - Testing script.
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

#define MAX7219_SEG_NUM (1+0)	// The number of the segments. Increase this for multiple matrices.
// NOTE: Add 1 for an extra element at the end of the buffer for a "hidden" symbol to scroll in.
#define MAX7219_SEG_LAST (MAX7219_SEG_NUM - 1) * 8	// The index in the buffer of the last segment.
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8		// The size of the buffer

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

// ----------------------------------------------------------------------------

int main(void) {
	// ---- Initialization ----
	scheduler_init(SCHEDULER_USERFUNC_NULL);
	scheduler_reinit(SCHEDULER_TCCR0B_1024, SCHEDULER_OCR0A_MIN);	// Adjust, if necessary
	scheduler_start();
	max7219b_init(max7219_buffer, MAX7219_BUFFER_SIZE);
	max7219b_scheduler();

	// ---- Main Loop ----
	for (;;) {
		for (uint8_t y = 0; y <= 7; y++) {
			for (uint8_t x = 0; x < MAX7219_BUFFER_SIZE; x++) {
				max7219b_set(x, y);	// Set pixel
				_delay_ms(50);
				max7219b_clr(x, y);	// Clear pixel
			}
		}
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
