/**
 * MAX7219tiny - Testing script.
 * @author Neven Boyanov
 * This is part of the Tinusaur/MAX7219tiny project.
 * ----------------------------------------------------------------------------
 *  Copyright (c) 2023 Tinusaur (https://tinusaur.com). All rights reserved.
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

#define MAX7219_SEG_NUM 2	// The number of the segments. Increase this for multiple matrices.
// NOTE: Add 1 for an extra element at the end of the buffer for a "hidden" symbol to scroll in.
#define MAX7219_SEG_LAST (MAX7219_SEG_NUM - 1) * 8	// The index in the buffer of the last segment.
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8		// The size of the buffer

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

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

#define SCROLLXXX_____ 0b11100000
#define SCROLLXXX____X 0b11100001
#define SCROLLXXX___XX 0b11100011
#define SCROLLXXX__XXX 0b11100111

#define SCROLLXXXX____ 0b11110000
#define SCROLLXXXX___X 0b11110001
#define SCROLLXXXX__XX 0b11110011

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

int main(void) {
	// ---- Initialization ----
	scheduler_init();
	scheduler_reinit(SCHEDULER_TCCR0B_1024, SCHEDULER_OCR0A_MIN);	// Adjust, if necessary
	scheduler_start();
	max7219b_init(MAX7219_SEG_NUM, max7219_buffer, MAX7219_BUFFER_SIZE);
	max7219b_scheduler();

	// ---- Main Loop ----
	for (;;) {
		for (uint8_t i = 0; i < sizeof(scrolls); i++) {
			max7219b_col(MAX7219_BUFFER_SIZE - 1, pgm_read_byte(&scrolls[i]));
			max7219b_left();	// Scroll to the left.
			_delay_ms(45);
		}
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
