/*
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */ 


/* Includes ------------------------------------------------------------------*/

#include "LLDISPLAY_intern.h"
#if LLDISPLAY_BPP == 24

/* API -----------------------------------------------------------------------*/

uint32_t LLDISPLAY_BPP_get_LTDC_COLOR_MODE(void)
{
	return LTDC_PIXEL_FORMAT_RGB888;
}

uint32_t LLDISPLAY_BPP_get_DMA2D_COLOR_MODE(void)
{
	return DMA2D_RGB888;
}

uint32_t LLDISPLAY_BPP_get_lcd_format(void)
{
	return LLDISPLAY_EXTRA_IMAGE_RGB888;
}

/* EOF -----------------------------------------------------------------------*/

#endif

