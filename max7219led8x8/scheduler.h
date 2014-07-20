/*
 * Scheduler - Tinusaur Scheduler
 *
 * @file: scheduler.h
 * @created: 2014-07-18
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

// --------------------------------------------------------------------

void scheduler_init(void);
void scheduler_start(uint8_t max);
void scheduler_userfunc(void);

// --------------------------------------------------------------------

#endif
