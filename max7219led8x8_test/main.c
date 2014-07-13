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

// --------------------------------------------------------------------

#define F_CPU 1000000UL

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

// --------------------------------------------------------------------

// ---------------------	// Vcc,	Pin 1 on LED8x8 Board
// ---------------------	// GND,	Pin 2 on LED8x8 Board
#define MAX7219_DIN		PB0	// DI,	Pin 3 on LED8x8 Board
#define MAX7219_CS		PB1	// CS,	Pin 4 on LED8x8 Board
#define MAX7219_CLK		PB2	// CLK,	Pin 5 on LED8x8 Board

#include "../max7219led8x8/max7219led8x8.h"

// --------------------------------------------------------------------

int main(void) {

	// ---- Initialization ----
	MAX7219_init();

	// ---- Main Loop ----
	while (1) {
		for (uint8_t y = 0; y <= 7; y++) {
			for (uint8_t x = 0; x <= 7; x++) {
				MAX7219_buffer_out();	// Output the buffer
				MAX7219_buffer_set(x, y);	// Set pixel
				_delay_ms(10);
			}
		}
		for (uint8_t y = 0; y <= 7; y++) {
			for (uint8_t x = 0; x <= 7; x++) {
				MAX7219_buffer_out();	// Output the buffer
				MAX7219_buffer_clr(x, y);	// Set pixel
				_delay_ms(10);
			}
		}
	}

	return 0;
}
