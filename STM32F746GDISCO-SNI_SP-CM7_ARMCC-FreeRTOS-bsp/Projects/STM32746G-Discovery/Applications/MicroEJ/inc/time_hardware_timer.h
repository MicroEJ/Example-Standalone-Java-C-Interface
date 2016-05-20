/*
 * Copyright 2012-2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef __TIME_HARDWARE_TIMER_H
#define __TIME_HARDWARE_TIMER_H

#include <stdint.h>

/**
 * Provide functions to get time using an hardware timer
 */
void time_hardware_timer_initialize(void);
int64_t time_hardware_timer_getCurrentTime(void);
int64_t time_hardware_timer_getTimeNanos(void);

#endif /* __TIME_HARDWARE_TIMER_H */
