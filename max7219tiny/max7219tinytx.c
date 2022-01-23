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

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "max7219tiny.h"
#include "max7219tinytx.h"

// ----------------------------------------------------------------------------

extern uint8_t *__max7219_buffer;
extern uint8_t __max7219_buffer_size;

// ----------------------------------------------------------------------------

const uint8_t *__max7219tx_font_src;
uint8_t __max7219tx_font_charbase;

// ----------------------------------------------------------------------------

void max7219tx_init(const uint8_t *font_src, uint8_t char_base) {
	__max7219tx_font_src = font_src;
	__max7219tx_font_charbase = char_base;
}

// NOTE: The font is 8x6.
void max7219tx_char(uint8_t x, uint8_t c) {
	for (uint8_t i = 0; i <= 7; i++) {
		uint8_t index = c - __max7219tx_font_charbase;	// Convert ASCII to index.
		uint16_t offset = (index << 2) + (index << 1);	// Fast multiply by 6.
		max7219b_col(x + i, pgm_read_byte(&__max7219tx_font_src[offset + i]));
	}
}

// ============================================================================
