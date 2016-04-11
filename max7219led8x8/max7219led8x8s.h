/*
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @file: max7219led8x8s.h
 * @created: 2014-07-18
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

#ifndef MAX7219LED8X8S_H
#define MAX7219LED8X8S_H

// --------------------------------------------------------------------

void max7219s_init(void);
void max7219s_start(void);
void max7219s_buffer_out(void);

void max7219s_buffer_set(uint8_t x, uint8_t y);
void max7219s_buffer_clr(uint8_t x, uint8_t y);

// --------------------------------------------------------------------

#endif
