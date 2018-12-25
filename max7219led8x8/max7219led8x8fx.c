/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/MAX7219LED8x8 project.
 *
 * Copyright (c) 2018 Neven Boyanov, The Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Retain in your source code the link http://tinusaur.org to the Tinusaur project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// ============================================================================

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "max7219led8x8.h"
#include "max7219led8x8fx.h"

// ----------------------------------------------------------------------------

extern uint8_t *__max7219_buffer;
extern uint8_t __max7219_buffer_size;

// ----------------------------------------------------------------------------

void max7219fx_delay_cs(uint8_t d) { // d = delay in centisecond
	while (d--) _delay_ms(10); // Should be "--d" to compensate the overhead.
}

// ----------------------------------------------------------------------------

const uint8_t *max7219fx_font_src;
uint8_t max7219fx_font_char_base;

// ----------------------------------------------------------------------------

void max7219fx_init(const uint8_t *fron_src, uint8_t char_base) {
	max7219fx_font_src = fron_src;
	max7219fx_font_char_base = char_base;
}

void max7219fx_char(uint8_t x, uint8_t c) {
	for (uint8_t i = 0; i <= 7; i++) {
		uint8_t index = c - max7219fx_font_char_base;	// Convert ASCII to index.
		uint16_t offset = (index << 2) + (index << 1);	// Fast multiply by 6.
		max7219b_col(x + i, pgm_read_byte(&max7219fx_font_src[offset + i]));
	}
}

void max7219fx_bmp(uint8_t x, const uint8_t buffer[], uint16_t offset, uint8_t w) {
	for (uint8_t i = 0; i <= w; i++) {
		max7219b_col(x + i, pgm_read_byte(&buffer[offset + i]));
	}
}

void max7219fx_left(uint8_t n, uint8_t d) { // d = delay in centisecond
	while (n--) {
		max7219b_left();
		max7219fx_delay_cs(d);
	}
}

void max7219fx_inv(void) {
	for (uint8_t i = __max7219_buffer_size; i > 0; i--) {
		__max7219_buffer[i] ^= 0xff;
	}
}

void max7219fx_flash(uint8_t n, uint8_t d) { // d = delay in centisecond
	n = n << 2;
	while (n--) {
		max7219fx_inv();
		max7219fx_delay_cs(d);
	}
}

// ============================================================================
