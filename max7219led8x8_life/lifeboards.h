/*
 * MAX7219Led8x8 Life - Boards for Conway's Game of Life
 *
 * @file: lifeboards.h
 * @created: 2014-07-19
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

#ifndef LIFEBOARDS_H
#define LIFEBOARDS_H

// ============================================================================

typedef uint8_t life_board[8];

// ----------------------------------------------------------------------------

// Ref: https://en.wikipedia.org/wiki/Conways_Game_of_Life
life_board life_oscillators_blinkers = {
	0b00000000,
	0b00000000,
	0b01110000,
	0b00000000,
	0b00000100,
	0b00000100,
	0b00000100,
	0b00000000
	};

// Ref: https://en.wikipedia.org/wiki/Conways_Game_of_Life
life_board life_oscillators_toad_beacon = {
	0b00000000,
	0b00000111,
	0b00001110,
	0b00000000,
	0b11000000,
	0b11000000,
	0b00110000,
	0b00110000
	};

// Ref: https://en.wikipedia.org/wiki/Conways_Game_of_Life
life_board life_oscillators_glider = {
	0b01000000,
	0b00100000,
	0b11100000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000
	};

// Ref: https://en.wikipedia.org/wiki/Conways_Game_of_Life
life_board life_oscillators_rpentomino = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000110,
	0b00001100,
	0b00000100,
	0b00000000,
	0b00000000
	};

// ----------------------------------------------------------------------------

// Ref: https://en.wikipedia.org/wiki/Oscillator_(cellular_automaton)
life_board life_oscillators_octagon  = {
	0b00011000,
	0b00100100,
	0b01000010,
	0b10000001,
	0b10000001,
	0b01000010,
	0b00100100,
	0b00011000
	};

// Ref: https://en.wikipedia.org/wiki/Oscillator_(cellular_automaton)
life_board life_oscillators_fumarole1  = {
	0b00000000,
	0b00011000,
	0b01100110,
	0b01000010,
	0b00100100,
	0b10100101,
	0b11000011,
	0b00000000
	};

// Ref: https://en.wikipedia.org/wiki/Oscillator_(cellular_automaton)
life_board life_oscillators_fumarole2  = {
	0b00011000,
	0b01000010,
	0b01000010,
	0b01000010,
	0b00100100,
	0b10100101,
	0b11000011,
	0b00000000
	};

// ----------------------------------------------------------------------------

// Ref: http://www.conwaylife.com/wiki/Mold
life_board life_oscillators_mold  = {
	0b00000000,
	0b00001100,
	0b00010010,
	0b01001010,
	0b00000100,
	0b01011000,
	0b00100000,
	0b00000000
	};

// Ref: http://www.conwaylife.com/wiki/Jam
life_board life_oscillators_jam  = {
	0b00000000,
	0b00001100,
	0b00010010,
	0b01001010,
	0b01000100,
	0b01000000,
	0b00001000,
	0b00110000
	};

// Ref: http://www.conwaylife.com/wiki/Trice_tongs
life_board life_oscillators_tricetongs  = {
	0b00100000,
	0b00111000,
	0b11000100,
	0b01010100,
	0b01000000,
	0b00110010,
	0b00000110,
	0b00000000
	};

// ----------------------------------------------------------------------------

#endif
