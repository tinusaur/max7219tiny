/**
 * MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @created 2014-07-18
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/MAX7219LED8x8 project.
 *
 * Copyright (c) 2016 Neven Boyanov, Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Please, as a favor, retain the link http://tinusaur.org to The Tinusaur Project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// ============================================================================

#ifndef MAX7219LED8X8S_H
#define MAX7219LED8X8S_H

// ----------------------------------------------------------------------------

#include <stdint.h>

// ----------------------------------------------------------------------------

void max7219s_init(void);
void max7219s_start(void);
void max7219s_buffer_out(void);

void max7219s_buffer_set(uint8_t x, uint8_t y);
void max7219s_buffer_clr(uint8_t x, uint8_t y);

// ============================================================================

#endif
