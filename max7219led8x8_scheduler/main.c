/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @created 2014-07-18
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

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "tinyavrlib/scheduler.h"

// Change the ports, if necessary.
// #define MAX7219_DIN		PB0	// DI,	Pin 3 on LED8x8 Board
// #define MAX7219_CS		PB1	// CS,	Pin 4 on LED8x8 Board
// #define MAX7219_CLK		PB2	// CLK,	Pin 5 on LED8x8 Board

#include "max7219led8x8/max7219led8x8.h"

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

void scheduler_userfunc(uint32_t scheduler_tick) {
	max7219b_out();
}

// ----------------------------------------------------------------------------

int main(void) {

	// ---- Initialization ----
	max7219_init();
	scheduler_init(scheduler_userfunc);
	// scheduler_reinit(SCHEDULER_TCCR0B_1024, SCHEDULER_OCR0A_DEFAULT);	// Adjust, if necessary
	scheduler_start();

	// ---- Main Loop ----
	for (;;) {
		uint8_t xp = 0, yp = 0;
		for (uint8_t y = 0; y <= 7; y++) {
			for (uint8_t x = 0; x <= 7; x++) {
				if (x == 0 && y == 0) xp = yp = 7;
				max7219b_set(x, y);	// Set pixel
				_delay_ms(100);
				max7219b_clr(x, y);	// Clear pixel
			}
		}
	}

	return 0;
}

// ============================================================================
