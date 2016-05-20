
/*
 *	H
 *
 *	Copyright 2016 IS2T. All rights reserved.
 *	IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _DAC_MANAGER_IMPL
#define _DAC_MANAGER_IMPL

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"

/* API -----------------------------------------------------------------------*/

/*
 * Fill the structure for the given DAC
 */
int32_t dac_manager_impl_get_data(int32_t dac_ID, hal_stm32_dac_data* dac_data);

/* EOF -----------------------------------------------------------------------*/

#endif // _DAC_MANAGER_IMPL
