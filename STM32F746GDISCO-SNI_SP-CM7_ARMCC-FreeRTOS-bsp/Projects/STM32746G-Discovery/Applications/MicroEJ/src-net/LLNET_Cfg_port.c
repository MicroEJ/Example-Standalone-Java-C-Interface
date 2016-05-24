/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "LLNET_Cfg.h"

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rng.h"

// RNG handler
static RNG_HandleTypeDef RngHandle;
// RNG initialization state
static uint8_t LLNET_RNG_initialized = 0;

/**
 * Returns a 32 bit random number.
 */
uint32_t LLNET_getRandomNumber(){
  if(!LLNET_RNG_initialized){
    // Enable Random Number Generator (RNG)
    __RNG_CLK_ENABLE();
    // Configure the RNG peripheral
    RngHandle.Instance = RNG;
    if (HAL_RNG_Init(&RngHandle) != HAL_OK)
    {
      // An error occurred during RNG initialization
      while(1);
    }
    LLNET_RNG_initialized = 1;
  }

  return HAL_RNG_GetRandomNumber(&RngHandle);

}
