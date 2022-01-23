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

#ifndef MAX7219TINYTX_H
#define MAX7219TINYTX_H

// ----------------------------------------------------------------------------

#include <stdint.h>

#include "max7219tiny.h"

// ----------------------------------------------------------------------------

void max7219tx_init(const uint8_t *font_src, uint8_t char_base);
void max7219tx_char(uint8_t x, uint8_t c);

// ============================================================================

#endif
