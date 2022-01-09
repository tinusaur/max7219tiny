/**
 * MAX7219tiny - Tinusaur MAX7219 Library for LED 8x8 Matrix
 * @author Neven Boyanov
 * This is part of the Tinusaur/MAX7219tiny project.
 * ----------------------------------------------------------------------------
 *  Copyright (c) 2022 Tinusaur (https://tinusaur.com). All rights reserved.
 *  Distributed as open source under the MIT License (see the LICENSE.txt file)
 *  Please, retain in your work a link to the Tinusaur project website.
 * ----------------------------------------------------------------------------
 * Source code available at: https://gitlab.com/tinusaur/max7219tiny
 */

// ============================================================================

#ifndef MAX7219TINY_H
#define MAX7219TINY_H

// ----------------------------------------------------------------------------

#include <stdint.h>
#include <avr/io.h>

#include "tinyavrlib/scheduler.h"

// ----------------------------------------------------------------------------
//               ATtiny85              |         |
//             +----------+       Vcc--|         |   GAMEx3 Gen-4
// -----(RST)-->      Vcc +--(+)  GND--| LED 8x8 |  --------------
// ------------+ PB3  PB2 +-->----DIN--|  Matrix |   PB2-->--CLK
// ------------+ PB4  PB1 +-->----CS---| MAX7219 |   PB1-->--DIN
//        (-)--+ GND  PB0 +-->----CLK--|         |   PB0-->--CS
//             +----------+            |         |
// ----------------------------------------------------------------------------

// PINOUT DEFINITIONS FOR Tinusaur Shield GAMEx3 Gen-1
#define MAX7219_CLK		PB0	// CLK,	pin 5 on the MAX7219 Board
#define MAX7219_CS		PB1	// CS,	pin 4 on the MAX7219 Board
#define MAX7219_DIN		PB2	// DIN,	pin 3 on the MAX7219 Board

// PINOUT DEFINITIONS FOR Tinusaur Shield GAMEx3 Gen-4
// #define MAX7219_CLK		PB2	// CLK,	pin 5 on the MAX7219 Board
// #define MAX7219_CS		PB0	// CS,	pin 4 on the MAX7219 Board
// #define MAX7219_DIN		PB1	// DIN,	pin 3 on the MAX7219 Board

// ----------------------------------------------------------------------------

void max7219_byte(uint8_t data);
void max7219_word(uint8_t address, uint8_t data);
void max7219_init(void);
void max7219_row(uint8_t address, uint8_t data);

// ----------------------------------------------------------------------------

// Buffered output functions.
// NOTE: The output MUST be buffered since there is no reading function
//       from the LED MAX7219 controlled.

void max7219b_init(uint8_t *buffer, uint8_t buffer_size);
void max7219b_scheduler(void);
void max7219b_out(void);
void max7219b_set(uint8_t x, uint8_t y);
void max7219b_clr(uint8_t x, uint8_t y);
void max7219b_col(uint8_t x, uint8_t data);
uint8_t max7219b_get(uint8_t y);
void max7219b_left(void);
// void max7219b_right(void);	// TODO: To implement

// ============================================================================

#endif
