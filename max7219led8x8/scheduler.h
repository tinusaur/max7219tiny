/**
 * Scheduler - Tinusaur Scheduler
 *
 * @created 2014-07-18
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur project.
 *
 * Copyright (c) 2016 Neven Boyanov, Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Please, as a favor, retain the link http://tinusaur.org to The Tinusaur Project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/max7219led8x8
 *
 */

// ============================================================================

#ifndef SCHEDULER_H
#define SCHEDULER_H

// ----------------------------------------------------------------------------

void scheduler_init(void);
void scheduler_start(uint8_t max);
void scheduler_userfunc(void);

// ============================================================================

#endif
