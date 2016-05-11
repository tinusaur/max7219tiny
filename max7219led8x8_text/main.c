/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @created 2014-07-12
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/MAX7219LED8x8 project.
 *
 * Copyright (c) 2016 Neven Boyanov, Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Please, as a favor, retain the link http://tinusaur.org to The Tinusaur Project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// ============================================================================

// #define F_CPU 1000000UL
// NOTE: The F_CPU (CPU frequency) should not be defined in the source code.
//       It should be defined in either (1) Makefile; or (2) in the IDE. 

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

// Change the ports, if necessary.
// #define MAX7219_DIN		PB0	// DI,	Pin 3 on LED8x8 Board
// #define MAX7219_CS		PB1	// CS,	Pin 4 on LED8x8 Board
// #define MAX7219_CLK		PB2	// CLK,	Pin 5 on LED8x8 Board

#include "max7219led8x8/max7219led8x8.h"
#include "max7219led8x8/max7219led8x8f.h"

//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 ATtiny
//                25/45/85
//              +----------+   (-)--GND--
//      (RST)---+ PB5  Vcc +---(+)--VCC--
// --[OWOWOD]---+ PB3  PB2 +--------CLK--
//           ---+ PB4  PB1 +---------CS--
// -------(-)---+ GND  PB0 +--------DIN--
//              +----------+
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

// ----------------------------------------------------------------------------

void max7219_ext_char(uint8_t c) {
	for(uint8_t i = 1; i < 9; i++)
		max7219_row(i, pgm_read_byte(&max7219led8x8_font[c - 32][i - 1]));
}

void max7219_ext_charxy(uint8_t c, uint8_t x, uint8_t y);	// TODO: Implement this.

// ----------------------------------------------------------------------------

int main(void) {

	// ---- Initialization ----
	max7219_init();

	// ---- Main Loop ----
	for (;;) {
		for (uint8_t c = ' '; c <= '_'; c++) {
			max7219_ext_char(c);
			_delay_ms(1000);
		}
		_delay_ms(400);
	}

	return 0;
}

// ============================================================================
