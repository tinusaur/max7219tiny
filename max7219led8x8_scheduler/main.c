/*
 * MAX7219Led8x8 - Library with Scheduletr Test
 *
 * @file: main.c
 * @created: 2014-07-18
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// ============================================================================

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

// --------------------------------------------------------------------

// ---------------------	// Vcc,	Pin 1 on LED8x8 Board
// ---------------------	// GND,	Pin 2 on LED8x8 Board
#define MAX7219_DIN		PB0	// DI,	Pin 3 on LED8x8 Board
#define MAX7219_CS		PB1	// CS,	Pin 4 on LED8x8 Board
#define MAX7219_CLK		PB2	// CLK,	Pin 5 on LED8x8 Board

#include "../max7219led8x8/max7219led8x8.h"
#include "../max7219led8x8/max7219led8x8s.h"
#include "../max7219led8x8/scheduler.h"

// ============================================================================

#define LED1_PORT PB4

void scheduler_userfunc(void) {
	PORTB ^= (1 << LED1_PORT);
	max7219s_buffer_out();	// Output the buffer
}

// ============================================================================

int main(void)
{
	// ---- Initialization ----
	max7219s_init();
	// Setup LED, DDRB - Data Direction Register, Port B
	DDRB |= (1 << LED1_PORT); // Set port as LED output

	// ---- Main Loop ----
	max7219s_start();
	while (1) {
		uint8_t xp = 0, yp = 0;
		for (uint8_t y = 0; y <= 7; y++) {
			for (uint8_t x = 0; x <= 7; x++) {
				if (x == 0 && y == 0) xp = yp = 7;
				MAX7219_buffer_set(x, y);	// Set pixel
				_delay_ms(50);
				MAX7219_buffer_clr(x, y);	// Clear pixel
			}
		}
	}
	
	return (0);
}
