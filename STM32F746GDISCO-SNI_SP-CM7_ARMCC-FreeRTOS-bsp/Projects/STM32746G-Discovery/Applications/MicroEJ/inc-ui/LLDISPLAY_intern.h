/*
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _LLDISPLAY_DISCO_INTERN
#define _LLDISPLAY_DISCO_INTERN

/* Includes ------------------------------------------------------------------*/

#include "stm32f7xx.h"
#include "LLDISPLAY_configuration.h"
#include "stm32746g_discovery_lcd.h"
#include "LLDISPLAY_EXTRA.h"
#include "LLDISPLAY_EXTRA_drawing.h"

/* Defines -------------------------------------------------------------------*/

#define WIDTH	(480)
#define HEIGHT	(272)

#define LTDC_IER_FLAG LTDC_IER_RRIE // LTDC_IER_RRIE | LTDC_IER_LIE

/* API -----------------------------------------------------------------------*/

// functions according LLDISPLAY_BPP
uint32_t LLDISPLAY_BPP_get_LTDC_COLOR_MODE(void);
uint32_t LLDISPLAY_BPP_get_DMA2D_COLOR_MODE(void);
uint32_t LLDISPLAY_BPP_get_lcd_format(void);

#endif
