/*
 * MAX7219Led8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix
 *
 * @file: max7219led8x8.c
 * @created: 2014-07-18
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// --------------------------------------------------------------------

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "max7219led8x8.h"
#include "max7219led8x8s.h"
#include "../max7219led8x8/scheduler.h"

// --------------------------------------------------------------------

void max7219s_init(void) {
	MAX7219_init();
	scheduler_init();
}

void max7219s_start(void) {
	scheduler_start(20);
}

void max7219s_buffer_out(void) {
	MAX7219_buffer_out();
}

// --------------------------------------------------------------------
