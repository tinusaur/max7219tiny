/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @created 2014-07-12
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

#ifndef MAX7219LED8X8_H
#define MAX7219LED8X8_H

// ----------------------------------------------------------------------------

#include <stdint.h>
#include <avr/io.h>

// ----------------------------------------------------------------------------

#define MAX7219_CLK		PB0	// CLK,	pin 5 on the MAX7219 Board
#define MAX7219_CS		PB1	// CS,	pin 4 on the MAX7219 Board
#define MAX7219_DIN		PB2	// DIN,	pin 3 on the MAX7219 Board

// ----------------------------------------------------------------------------

void max7219_byte(uint8_t data);
void max7219_word(uint8_t address, uint8_t data);
void max7219_init(uint8_t seg_num);
void max7219_row(uint8_t address, uint8_t data);

// ----------------------------------------------------------------------------

// Buffered output functions.
// NOTE: The output MUST be buffered since there is no reading function
//       from the LED MAX7219 controlled.

void max7219b_init(uint8_t *buffer, uint8_t buffer_size);
void max7219b_out(void);
void max7219b_set(uint8_t x, uint8_t y);
void max7219b_clr(uint8_t x, uint8_t y);
void max7219b_col(uint8_t x, uint8_t data);
uint8_t max7219b_get(uint8_t y);
void max7219b_left(void);
// void max7219b_right(void);

// ----------------------------------------------------------------------------

// Buffered output scheduler functions.
// NOTE: It is much more convenient if the buffer is sent out to the controller
//       automatically and the application just sets pixels in the buffer.

void max7219bs_scheduler_userfunc(uint32_t scheduler_tick);

// ============================================================================

#endif
