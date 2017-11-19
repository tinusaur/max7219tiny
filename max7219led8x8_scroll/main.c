/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
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

#include "tinyavrlib/scheduler.h"

// If you need to change the ports for the DIN/CS/CLK you should do so
// in the "max7219led8x8.h" source code file in the MAX7219LED8x8 library 
// so it will take affect on all the code.

#include "max7219led8x8/max7219led8x8.h"

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

#define SCROLL________ 0b00000000

#define SCROLL_______X 0b00000001
#define SCROLL______XX 0b00000011
#define SCROLL_____XXX 0b00000111
#define SCROLL____XXXX 0b00001111
#define SCROLL___XXXXX 0b00011111

#define SCROLLX_______ 0b10000000
#define SCROLLX______X 0b10000001
#define SCROLLX_____XX 0b10000011
#define SCROLLX____XXX 0b10000111
#define SCROLLX___XXXX 0b10001111

#define SCROLLXX______ 0b11000000
#define SCROLLXX_____X 0b11000001
#define SCROLLXX____XX 0b11000011
#define SCROLLXX___XXX 0b11000111
#define SCROLLXXX__XXX 0b11100111

#define SCROLLXXX_____ 0b11100000
#define SCROLLXXX____X 0b11100001
#define SCROLLXXX___XX 0b11100011

#define SCROLLXXXX____ 0b11110000
#define SCROLLXXXX___X 0b11110001

#define SCROLLXXXXX___ 0b11111000

#define SCROLL____X___ 0b00001000
#define SCROLLX___X___ 0b10001000
#define SCROLL___X____ 0b00010000
#define SCROLL___X___X 0b00010001
#define SCROLL___XX___ 0b00011000

#define SCROLL_X_X_X_X 0b01010101
#define SCROLLX_X_X_X_ 0b10101010
#define SCROLLXXXXXXXX 0b11111111

const uint8_t scrolls[] PROGMEM = {
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLLX______X,
	SCROLLX______X,
	SCROLLXX____XX,
	SCROLLXX____XX,
	SCROLLXXX___XX,
	SCROLLXXX___XX,
	SCROLLXX____XX,
	SCROLLX______X,
	SCROLL________,
	SCROLL________,
	SCROLL_______X,
	SCROLL______XX,
	SCROLL_____XXX,
	SCROLL____XXXX,
	SCROLL___XXXXX,
	SCROLLX___XXXX,
	SCROLLXX___XXX,
	SCROLLXXX___XX,
	SCROLLXXXX___X,
	SCROLLXXXXX___,
	SCROLLXXXX___X,
	SCROLLXXX___XX,
	SCROLLXX___XXX,
	SCROLLX___XXXX,
	SCROLL___XXXXX,
	SCROLLX___XXXX,
	SCROLLXX___XXX,
	SCROLLXX____XX,
	SCROLLX______X,
	SCROLLX______X,
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLL____X___,
	SCROLL____X___,
	SCROLL____X___,
	SCROLL___XX___,
	SCROLL___XX___,
	SCROLL___XX___,
	SCROLL____X___,
	SCROLL____X___,
	SCROLL____X___,
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLLX______X,
	SCROLLX______X,
	SCROLLXX____XX,
	SCROLLXX____XX,
	SCROLLXXX__XXX,
	SCROLLXXX__XXX,
	SCROLLXXX__XXX,
	SCROLLXXX__XXX,
	SCROLLXX____XX,
	SCROLLXX____XX,
	SCROLLX______X,
	SCROLLX______X,
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLL___X____,
	SCROLL___X____,
	SCROLL___XX___,
	SCROLL___XX___,
	SCROLL___X____,
	SCROLL___X____,
	SCROLL___X___X,
	SCROLL___X___X,
	SCROLL___X___X,
	SCROLL___X____,
	SCROLL___X____,
	SCROLL___XX___,
	SCROLL___XX___,
	SCROLL____X___,
	SCROLL____X___,
	SCROLL________,
	SCROLL________,
	SCROLLX______X,
	SCROLLXX____XX,
	SCROLLXX___XXX,
	SCROLLX____XXX,
	SCROLL____XXXX,
	SCROLL___XXXXX,
	SCROLL___XXXXX,
	SCROLL___XXXXX,
	SCROLLX___XXXX,
	SCROLLXX___XXX,
	SCROLLXX____XX,
	SCROLLX______X,
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLLX_X_X_X_,
	SCROLL_X_X_X_X,
	SCROLLX_X_X_X_,
	SCROLL_X_X_X_X,
	SCROLLXXXXXXXX,
	SCROLLXXXXXXXX,
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLL________,
};

// ----------------------------------------------------------------------------

#define MAX7219_SEG_NUM 4	// Segments, number of 8x8 matrices
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8

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
		for (uint8_t i = 0; i < sizeof(scrolls); i++) {
			max7219b_col(MAX7219_BUFFER_SIZE - 1, pgm_read_byte(&scrolls[i]));
			max7219b_left();
			_delay_ms(50);
		}
	}

	// Return the mandatory for the "main" function int value. It is "0" for success.
	return 0;
}

// ============================================================================
