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
#include "max7219tinyfx.h"

// ----------------------------------------------------------------------------

extern uint8_t *__max7219_buffer;
extern uint8_t __max7219_buffer_size;

// ----------------------------------------------------------------------------

void max7219fx_delay_cs(uint8_t d) { // d = delay in centisecond
	while (d--) _delay_ms(10); // Should be "--d" to compensate the overhead.
}

// ----------------------------------------------------------------------------

void max7219fx_init(void) {
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
	// for (int8_t i = __max7219_buffer_size - 1; i >= 0; i--) {
	for (uint8_t i = 0; i < __max7219_buffer_size; i++) {
		__max7219_buffer[i] ^= 0xff;
	}
}

void max7219fx_flash(uint8_t n, uint8_t d) { // d = delay in centisecond
	n = n << 1;
	while (n--) {
		max7219fx_inv();
		max7219fx_delay_cs(d);
	}
}

// ============================================================================
