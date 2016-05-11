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

#include <stdint.h>

#include "max7219led8x8.h"
#include "max7219led8x8s.h"
#include "scheduler.h"

// ----------------------------------------------------------------------------

void max7219s_init(void) {
	max7219_init();
	scheduler_init();
}

void max7219s_start(void) {
	scheduler_start(20);
}

void max7219s_buffer_out(void) {
	max7219_buffer_out();
}

// ============================================================================
