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
	life_board_buffer[Y] & (1 << (~X & 0x7)) : \
	(0 != 0) )
#define LIFE_CELL_SET(X, Y) life_board_buffer2[Y] |= (1 << (~X & 0x7))
#define LIFE_CELL_CLR(X, Y) life_board_buffer2[Y] &= ~(1 << (~X & 0x7))
#define LIFE_BOARD_ROW(Y) life_board_buffer[y]

// Alternative implementation for LIFE_CELL_ISSET
/*
#define LIFE_CELL_VAL(X, Y) life_cell_val(X, Y) != 0
uint8_t life_cell_val(uint8_t cx, uint8_t cy) {
	if (cx >= 0 && cx <= 7 && cy >= 0 && cy <= 7) {
		return life_board_buffer[cy] & (1 << (~cx & 0x7));
		// Could substitute "(7 - cx)" and "(~cx & 0x7)"
	} else {
		return 0;
	}
}
*/

void life_board_init(life_board buffer) {
	for (uint8_t y = 0; y <= 7; y++) {
		life_board_buffer[y] = buffer[y];
	}
}

void life_board_out(void) {
	for (uint8_t y = 0; y <= 7; y++) {
		max7219b_row(LIFE_BOARD_ROW(y), y);
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
	for (uint8_t y = 0; y <= 7; y++) {
		life_board_buffer[y] = life_board_buffer2[y];
	}
}

/*
void life_board_rand(void) {
	srand(3);
	for (uint8_t y = 0; y <= 7; y++) {
		life_board_buffer[y] = rand() & 0xff;
	}
}
*/

// ----------------------------------------------------------------------------

int main(void) {

	// ---- Initialization ----
	max7219_init();

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
	
	// max7219bs_init_start();
	scheduler_init(max7219bs_scheduler_userfunc);
	scheduler_reinit(SCHEDULER_TCCR0B_1024, SCHEDULER_OCR0A_MIN);	// Adjust, if necessary
	scheduler_start();
	
	// ---- Main Loop ----

#define LIFE_BOARDS_NUM sizeof(life_boards) / sizeof(*life_boards)
// Ref: https://en.wikibooks.org/wiki/C_Programming/Pointers_and_arrays

	for (;;) {
		for (uint8_t i = 0; i < LIFE_BOARDS_NUM; i++) {	// sizeof(life_boards)
			life_board_init(*life_boards[i]);
			for (uint8_t c = 0; c < 40; c++) {
				life_board_out();
				_delay_ms(100);
				life_board_turn();
				life_board2_copy();
			}
			life_board_init(life_board_solid);
			life_board_out();
			_delay_ms(1000);
		}
	}

	return 0;
}

// ============================================================================
