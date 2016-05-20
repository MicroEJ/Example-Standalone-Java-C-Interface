/*
 * C
 *
 * Copyright 2013-2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef IO_TASK_H
#define IO_TASK_H

/* Includes ------------------------------------------------------------------*/

#include "microej.h"

/* API -----------------------------------------------------------------------*/

/**
 * @brief  Create and start the IO Expander task that call IOExpander16_interrupt() when it is notified
 * @retval 0 if an error occurred, a non null value otherwise
 */
uint8_t IO_TASK_create_task(void);

#endif

