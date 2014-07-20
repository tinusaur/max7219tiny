/*
 * MAX7219Led8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
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

// ---------------------	// Vcc,	Pin 1 on LED8x8 Board
// ---------------------	// GND,	Pin 2 on LED8x8 Board
#define MAX7219_DIN		PB0	// DI,	Pin 3 on LED8x8 Board
#define MAX7219_CS		PB1	// CS,	Pin 4 on LED8x8 Board
#define MAX7219_CLK		PB2	// CLK,	Pin 5 on LED8x8 Board

#include "../max7219led8x8/max7219led8x8.h"
#include "../max7219led8x8/max7219led8x8f.h"

// --------------------------------------------------------------------

void MAX7219_ext_char(uint8_t c) {
	for(uint8_t i = 1; i < 9; i++)
		MAX7219_row(i, pgm_read_byte(&max7219led8x8_font[c - 32][i - 1]));
}

void MAX7219_ext_charxy(uint8_t c, uint8_t x, uint8_t y);	// TODO: Implement this.

// ============================================================================

int main(void) {

	// ---- Initialization ----
	MAX7219_init();

	// ---- Main Loop ----
	while (1) {
		for (uint8_t c = ' '; c <= '_'; c++)
		{
			MAX7219_ext_char(c);
			_delay_ms(1000);
		}
		_delay_ms(400);
	}

	return 0;
}
