/*
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @file: max7219led8x8.c
 * @created: 2014-07-12
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

#ifndef MAX7219LED8X8_H
#define MAX7219LED8X8_H

// --------------------------------------------------------------------

// ---------------------	// Vcc,	Pin 1 on LED8x8 Board
// ---------------------	// GND,	Pin 2 on LED8x8 Board
#ifndef MAX7219_DIN
#define MAX7219_DIN		PB0	// DI,	Pin 3 on LED8x8 Board
#endif
#ifndef MAX7219_CS
#define MAX7219_CS		PB1	// CS,	Pin 4 on LED8x8 Board
#endif
#ifndef MAX7219_CLK
#define MAX7219_CLK		PB2	// CLK,	Pin 5 on LED8x8 Board
#endif

// --------------------------------------------------------------------

void max7219_byte(uint8_t data);
void max7219_word(uint8_t address,uint8_t dat);
void max7219_init(void);
void max7219_row(uint8_t address,uint8_t dat);
void max7219_buffer_out(void);
void max7219_buffer_set(uint8_t x, uint8_t y);
void max7219_buffer_clr(uint8_t x, uint8_t y);
void max7219_buffer_row(uint8_t row, uint8_t y);

// --------------------------------------------------------------------

#endif
