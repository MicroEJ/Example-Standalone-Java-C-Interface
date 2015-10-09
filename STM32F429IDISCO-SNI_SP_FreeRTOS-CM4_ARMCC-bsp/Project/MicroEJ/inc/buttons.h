/*
 * C
 *
 * Copyright 2012-2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _BUTTONS
#define _BUTTONS

#include "LLINPUT_DEVICE.h"

typedef struct BUTTONS{
	struct LLINPUT_DEVICE header;
	// can add some fields
} BUTTONS;

void enableButtonsInterrupts(void);
void disableButtonsInterrupts(void);
BUTTONS* buttonsInitialize(void);

#endif
