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

#ifndef MAX7219LED8X8FX_H
#define MAX7219LED8X8FX_H

// ----------------------------------------------------------------------------

#include <stdint.h>

#include "max7219led8x8.h"

// ----------------------------------------------------------------------------

void max7219fx_init(const uint8_t *fron_src, uint8_t char_base);
void max7219fx_char(uint8_t x, uint8_t c);
void max7219fx_bmp(uint8_t x, const uint8_t buffer[], uint16_t offset, uint8_t w);
void max7219fx_left(uint8_t n, uint8_t d);
void max7219fx_inv(void);
void max7219fx_flash(uint8_t n, uint8_t d);

// ============================================================================

#endif
