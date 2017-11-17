/**
 * MAX7219LED8x8fx - Testing script.
 *
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/MAX7219LED8x8 project.
 *
 * Copyright (c) 2017 Neven Boyanov, The Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Retain in your source code the link http://tinusaur.org to the Tinusaur project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// ============================================================================

// #define F_CPU 1000000UL
// NOTE: The F_CPU (CPU frequency) should not be defined in the source code.
//       It should be defined in either (1) Makefile; or (2) in the IDE. 

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

// If you need to change the ports for the DIN/CS/CLK you should do so
// in the "max7219led8x8.h" source code file in the MAX7219LED8x8 library 
// so it will take affect on all the code.

#include "tinyavrlib/scheduler.h"

#include "max7219led8x8/max7219led8x8.h"
#include "max7219led8x8/max7219led8x8f.h"
#include "max7219led8x8/max7219led8x8fx.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 ATtiny85
//               +----------+   (-)--GND--
//       (RST)---+ PB5  Vcc +---(+)--VCC--
// ---[OWOWOD]---+ PB3  PB2 +--------CLK--
// --------------+ PB4  PB1 +---------CS--
// --------(-)---+ GND  PB0 +--------DIN--
//               +----------+
//              Tinusaur Board
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ----------------------------------------------------------------------------

#define MAX7219_SEG_NUM (4 + 1)	// Segments, number of 8x8 matrices + 1 extra
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

// ----------------------------------------------------------------------------

// TODO: Move these functions to the FX library.

void max7219fx_char(uint8_t x, uint8_t c) {
	for(uint8_t i = 0; i <= 7; i++)
		max7219b_col(x + i, pgm_read_byte(&max7219led8x8_font[((c - 32) << 3) + i]));
}

void max7219fx_inv(void) {
	for(uint8_t i = 0; i < MAX7219_BUFFER_SIZE; i++) {
		max7219_buffer[i] ^= 0xff;
	}
}

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
		for (uint8_t c = ' '; c <= 127; c++) {
			max7219fx_char((MAX7219_SEG_NUM - 1) * 8, c);
			for (uint8_t s = 0; s < 8; s++) {
				max7219b_left();
				_delay_ms(20);
			}
			max7219fx_inv();
			_delay_ms(750);
			max7219fx_inv();
			_delay_ms(250);
		}
	}

	// Return the mandatory for the "main" function int value. It is "0" for success.
	return 0;
}

// ============================================================================
