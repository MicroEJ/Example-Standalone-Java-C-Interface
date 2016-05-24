/*
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */ 


/* Includes ------------------------------------------------------------------*/

#include "LLDISPLAY_intern.h"

#if LLDISPLAY_BPP == 16

/* API -----------------------------------------------------------------------*/

uint32_t LLDISPLAY_BPP_get_LTDC_COLOR_MODE(void)
{
	return LTDC_PIXEL_FORMAT_RGB565;
}

uint32_t LLDISPLAY_BPP_get_DMA2D_COLOR_MODE(void)
{
	return DMA2D_RGB565;
}

uint32_t LLDISPLAY_BPP_get_lcd_format(void)
{
	return LLDISPLAY_EXTRA_IMAGE_RGB565;
}

/* EOF -----------------------------------------------------------------------*/

#endif
