/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @created 2014-07-12
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

#ifndef MAX7219LED8X8_H
#define MAX7219LED8X8_H

// ----------------------------------------------------------------------------

#include <stdint.h>
#include <avr/io.h>

// ----------------------------------------------------------------------------

#ifndef MAX7219_DIN
#define MAX7219_DIN		PB0	// DI,	Pin 3 on LED8x8 Board
#endif
#ifndef MAX7219_CS
#define MAX7219_CS		PB1	// CS,	Pin 4 on LED8x8 Board
#endif
#ifndef MAX7219_CLK
#define MAX7219_CLK		PB2	// CLK,	Pin 5 on LED8x8 Board
#endif

// ----------------------------------------------------------------------------

void max7219_byte(uint8_t data);
void max7219_word(uint8_t address,  uint8_t data);
void max7219_init(void);
void max7219_row(uint8_t address,uint8_t data);
void max7219_buffer_out(void);
void max7219_buffer_set(uint8_t x, uint8_t y);
void max7219_buffer_clr(uint8_t x, uint8_t y);
void max7219_buffer_row(uint8_t row, uint8_t y);

// ============================================================================

#endif
