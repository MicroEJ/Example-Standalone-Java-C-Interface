/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef __MICROEJ_ERRNO_H
#define __MICROEJ_ERRNO_H
#define ERRNO
#include <FreeRTOS.h>
#include <task.h>
#include <stdint.h>
#define set_errno(err) (vTaskSetThreadLocalStoragePointer(NULL, 0, (void*)err))
#define errno ((int32_t)pvTaskGetThreadLocalStoragePointer(NULL, 0))

#endif
