/*
 * Scheduler - Tinusaur Scheduler
 *
 * @file: scheduler.c
 * @created: 2014-07-18
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// --------------------------------------------------------------------

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "max7219led8x8.h"
#include "max7219led8x8s.h"
#include "scheduler.h"

// --------------------------------------------------------------------

//		Timer Pre-scaling
//		CS02 CS01 CS00 Description
//		---- ---- ---- --------------------------------------------------------
//		0    0    0    No clock source (Timer/Counter stopped)
//		0    0    1    clk I/O / (No prescaling)
//		0    1    0    clk I/O / 8 (From prescaler)
//		0    1    1    clk I/O / 64 (From prescaler)
//		1    0    0    clk I/O / 256 (From prescaler)
//		1    0    1    clk I/O / 1024 (From prescaler)
//		1    1    0    External clock source on T0 pin. Clock on falling edge.
//		1    1    1    External clock source on T0 pin. Clock on rising edge.

// --------------------------------------------------------------------

void scheduler_init(void) {
	// Setup Timer
	TCCR0A |= (1 << WGM01);	// set timer in CTC mode
	TIMSK |= (1 << OCIE0A); // set Bit 4 – OCIE0A: Timer/Counter0 Output Compare Match A Interrupt Enable
}

void scheduler_start(uint8_t max) {
	// IMPORTANT: Requires TIMER0_COMPA_vect to be setup.
	sei(); //  Enable global interrupts
	OCR0A = max;	// set value for OCR0A - Output Compare Register A
	// Prescale and start timer: 1/1024-th
    TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);
}

// Define interrupt vector
ISR(TIMER0_COMPA_vect)
{
	scheduler_userfunc();
   	// Note: No need to clear flags in TIFR - done automatically
}

// --------------------------------------------------------------------
