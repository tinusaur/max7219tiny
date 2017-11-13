/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @created 2014-07-18
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/MAX7219LED8x8 project.
 *
 * Copyright (c) 2017 Neven Boyanov, Tinusaur Team. All Rights Reserved.
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

#include "lifeboards.h"

// ----------------------------------------------------------------------------

life_board life_board_sample1 = {
	0b11000000,
	0b11000000,
	0b00110000,
	0b00110000,
	0b00000000,
	0b00000000,
	0b00001110,
	0b00000000
	};

life_board life_board_solid = {
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111
	};

// ----------------------------------------------------------------------------

life_board life_board_buffer;
life_board life_board_buffer2;

// ----------------------------------------------------------------------------

#define LIFE_CELL_ISSET(X, Y) \
	( (X >= 0 && X <= 7 && Y >= 0 && Y <= 7) ? \
	life_board_buffer[X] & (1 << (~Y & 0x7)) : \
	(0 != 0) )
#define LIFE_CELL_SET(X, Y) life_board_buffer2[X] |= (1 << (~Y & 0x7))
#define LIFE_CELL_CLR(X, Y) life_board_buffer2[X] &= ~(1 << (~Y & 0x7))
#define LIFE_BOARD_ROW(x) life_board_buffer[x]

// Alternative implementation for LIFE_CELL_ISSET
/*
#define LIFE_CELL_VAL(X, Y) life_cell_val(X, Y) != 0
uint8_t life_cell_val(uint8_t cx, uint8_t cy) {
	if (cx >= 0 && cx <= 7 && cy >= 0 && cy <= 7) {
		return life_board_buffer[cx] & (1 << (cy~ & 0x7));
		// Could substitute "(7 - cy)" and "(~cy & 0x7)"
	} else {
		return 0;
	}
}
*/

void life_board_init(life_board buffer) {
	for (uint8_t x = 0; x <= 7; x++) {
		life_board_buffer[x] = buffer[x];
	}
}

void life_board_out(void) {
	for (uint8_t x = 0; x <= 7; x++) {
		max7219b_col(x, LIFE_BOARD_ROW(x));
	}
}

uint8_t life_cell_count(uint8_t cx, uint8_t cy) {
	uint8_t count = 0;
	for (int8_t dy = -1; dy <= 1; dy++) {
		for (int8_t dx = -1; dx <= 1; dx++) {
			if (!(dx == 0 && dy == 0)) {
				int8_t x = cx + dx;
				int8_t y = cy + dy;
				if (x >= 0 && x <= 7 && y >= 0 && y <= 7) {
					if (LIFE_CELL_ISSET(x, y)) count++;
				}
			}
		}
	}
	return count;
}

void life_board_turn(void) {
	for (uint8_t y = 0; y <= 7; y++) {
		for (uint8_t x = 0; x <= 7; x++) {
			uint8_t count = life_cell_count(x, y);
			if (LIFE_CELL_ISSET(x, y)) {
				if (count < 2)
					LIFE_CELL_CLR(x, y);
				else if (count == 2 || count == 3)
					LIFE_CELL_SET(x, y);
				else if (count > 3)
					LIFE_CELL_CLR(x, y);
			} else {
				if (count == 3)
					LIFE_CELL_SET(x, y);
				else
					LIFE_CELL_CLR(x, y);
			}
		}
	}
}

void life_board2_copy(void) {
	for (uint8_t x = 0; x <= 7; x++) {
		life_board_buffer[x] = life_board_buffer2[x];
	}
}

/*
void life_board_rand(void) {
	srand(3);
	for (uint8_t x = 0; x <= 7; x++) {
		life_board_buffer[x] = rand() & 0xff;
	}
}
*/

// ----------------------------------------------------------------------------

#define MAX7219_SEG_NUM 1	// Segments, number of 8x8 matrices
#define MAX7219_BUFFER_SIZE	MAX7219_SEG_NUM * 8

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

// ----------------------------------------------------------------------------

life_board *life_boards[] = {
	&life_oscillators_blinkers,
	&life_oscillators_toad_beacon,
	&life_oscillators_glider,
	&life_oscillators_rpentomino,
	
	&life_oscillators_octagon,
	&life_oscillators_fumarole1,
	&life_oscillators_fumarole2,
	
	&life_oscillators_mold,
	&life_oscillators_jam,
	&life_oscillators_tricetongs
};

#define LIFE_BOARDS_NUM sizeof(life_boards) / sizeof(*life_boards)
// Ref: https://en.wikibooks.org/wiki/C_Programming/Pointers_and_arrays

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
		for (uint8_t i = 0; i < LIFE_BOARDS_NUM; i++) {
			life_board_init(*life_boards[i]);
			for (uint8_t c = 0; c < 40; c++) {
				life_board_out();
				_delay_ms(50);
				life_board_turn();
				life_board2_copy();
			}
			life_board_init(life_board_solid);
			life_board_out();
			_delay_ms(500);
		}
	}

	// Return the mandatory for the "main" function int value. It is "0" for success.
	return 0;
}

// ============================================================================
