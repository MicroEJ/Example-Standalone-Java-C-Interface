/*
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _LLDISPLAY_DISCO_CONFIGURATION
#define _LLDISPLAY_DISCO_CONFIGURATION

/* Defines -------------------------------------------------------------------*/

/**
 * Configure the LLDISPLAY number of bits per pixel (BPP).
 *
 * There are 2 available BPP modes:
 * 	- 16:		16 BPP, in 5-6-5 mode
 * 	- 24:		24 BPP, in 8-8-8 mode
 *
 * Only one BPP can be used at any time. This define allows to select the wanted
 * implementation:
 * 	- '16':		use 16 BPP
 * 	- '24':		use 24 BPP
 *
 * According the chosen mode, there are some actions which must be done together:
 *
 * (1)	In MicroEJ, select the wanted display stack during the platform creation. In the display
 * 		configuration properties file of the configuration project
 * 		"[platform configuration project]/display/display.properties",
 * 		specify the expected BPP value and format (RGB565 or RGB888)
 *
 * (2)	Recompile the platform.
 *
 * (3)	Launch again the Java application against the new platform.
 *
 * (4)	Configure the define LLDISPLAY_BPP (below) with the wanted mode.
 *
 * (5)	Recompile your BSP.
 *
 *	The implementation of the two modes (see LLDISPLAY_565.c and LLDISPLAY_888.c)
 *	are very simple. Some common defines, structures and functions signatures are
 *	available in LLDISPLAY_intern.h.
 */
#define LLDISPLAY_BPP 16


#endif
