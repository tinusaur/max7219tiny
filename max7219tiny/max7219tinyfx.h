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

#ifndef MAX7219TINYFX_H
#define MAX7219TINYFX_H

// ----------------------------------------------------------------------------

#include <stdint.h>

#include "max7219tiny.h"

// ----------------------------------------------------------------------------

void max7219fx_init(void);
void max7219fx_bmp(uint8_t x, const uint8_t buffer[], uint16_t offset, uint8_t w);
void max7219fx_left(uint8_t n, uint8_t d);
void max7219fx_inv(void);
void max7219fx_flash(uint8_t n, uint8_t d);

// ============================================================================

#endif
