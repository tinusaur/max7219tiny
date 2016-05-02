/*
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @file: main.c
 * @created: 2014-07-12
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// ============================================================================

#define F_CPU 1000000UL

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

// --------------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                ATtiny
//               25/45/85
//             +----------+   (-)---GND--
//           --+ PB5  Vcc +---(+)---VCC--
// ---OWOWOD---+ PB3  PB2 +---------CLK--
//           --+ PB4  PB1 +----------CS--
// ------(-)---+ GND  PB0 +---------DIN--
//             +----------+
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define MAX7219_DIN		PB0	// DI,	Pin 3 on LED8x8 Board
#define MAX7219_CS		PB1	// CS,	Pin 4 on LED8x8 Board
#define MAX7219_CLK		PB2	// CLK,	Pin 5 on LED8x8 Board

#include "../max7219led8x8/max7219led8x8.h"

// --------------------------------------------------------------------

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

const uint8_t scrolls[100] PROGMEM = {
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
	SCROLLX______X,
	SCROLLX______X,
	SCROLL________,
	SCROLL________,
	SCROLL________,
	SCROLL___X____,
	SCROLL___X____,
	SCROLL___X____,
	SCROLL___XX___,
	SCROLL___XX___,
	SCROLL___XX___,
	SCROLL___X____,
	SCROLL___X____,
	SCROLL___X___X,
	SCROLL___X___X,
	SCROLL___X___X,
	SCROLL___X____,
	SCROLL____X___,
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
	SCROLLXXXXXXXX,
	SCROLL________,
	SCROLLXXXXXXXX,
	SCROLL________,
	SCROLLXXXXXXXX,
	SCROLL________,
	SCROLLXXXXXXXX,
	SCROLL________,
	SCROLLXXXXXXXX,
	SCROLL________,
	SCROLLXXXXXXXX
};

// ============================================================================

int main(void) {

	// ---- Initialization ----
	max7219_init();

	// ---- Main Loop ----
	uint8_t scrolls_index = 0;
	uint8_t screen_buffer[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t buffer_row;
	for (;;) {
		for (uint8_t screen_row = 8; screen_row >= 1; screen_row--) {
			buffer_row = screen_row - 1;
			// Output the buffer
			max7219_row(screen_row, screen_buffer[buffer_row]);

			// Scroll the row down
			if (buffer_row > 0) {
				screen_buffer[buffer_row] = screen_buffer[buffer_row - 1];
			} else {
				screen_buffer[buffer_row] = pgm_read_byte(&scrolls[scrolls_index++]);
				if (scrolls_index >= sizeof(scrolls) - 1) scrolls_index = 0;
			}
		}
		_delay_ms(150);
	}

	return 0;
}
