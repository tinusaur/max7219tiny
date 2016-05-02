/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
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

// ============================================================================

int main(void) {

	// ---- Initialization ----
	max7219_init();
	
	uint8_t data[8][8] = {
		{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, },
		{ 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, },
		{ 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, },
		{ 0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33, },
		{ 0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, },
		{ 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, },
		{ 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
	};

	// ---- Main Loop ----
	for (;;) {
		for (uint8_t i = 0; i <= 7; i++) {
			for (uint8_t y = 0; y <= 7; y++) {
				uint8_t d = data[i][y];
				for (uint8_t x = 0; x <= 7; x++) {
					max7219_buffer_out();	// Output the buffer
					if ((d & 1) == 1)
						max7219_buffer_set(x, y);	// Set pixel
					else
						max7219_buffer_clr(x, y);	// Clear pixel
					d = d >> 1;
					_delay_ms(20);
				}
			}
		}
	}

	return 0;
}

// ============================================================================
