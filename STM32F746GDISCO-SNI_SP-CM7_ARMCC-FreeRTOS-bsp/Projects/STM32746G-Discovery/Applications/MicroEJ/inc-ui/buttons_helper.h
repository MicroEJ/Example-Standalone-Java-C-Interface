/* 
 * Copyright 2015 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _BUTTONS_HELPER
#define _BUTTONS_HELPER

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* API -----------------------------------------------------------------------*/

/*
 * Initialize the buttons helper.
 */
void BUTTONS_HELPER_initialize(void);

/*
 * Notify to the event generator a button has been pressed.
 * @param buttonId the button ID, between 0 and 255
 */
void BUTTONS_HELPER_pressed(int32_t buttonId);

/**
 * Notify to the event generator a button has been repeated.
 * @param buttonId the button ID, between 0 and 255
 */
void BUTTONS_HELPER_repeated(int32_t buttonId);

/**
 * Notify to the event generator a button has been released.
 * @param buttonId the button ID, between 0 and 255
 */
void BUTTONS_HELPER_released(int32_t buttonId);

#endif
