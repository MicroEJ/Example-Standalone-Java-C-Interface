/* 
 * Copyright 2015 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _TOUCH_HELPER
#define _TOUCH_HELPER

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* API -----------------------------------------------------------------------*/

/*
 * Notify to an event handler a touch has been pressed.
 * @param x the pointer X coordinate
 * @param y the pointer Y coordinate
 */
void TOUCH_HELPER_pressed(int32_t x, int32_t y);

/*
 * Notify to an event handler a touch has moved.
 * @param x the pointer X coordinate
 * @param y the pointer Y coordinate
 */
void TOUCH_HELPER_moved(int32_t x, int32_t y);

/*
 * Notify to an event handler a touch has been released.
 */
void TOUCH_HELPER_released(void);

#endif
