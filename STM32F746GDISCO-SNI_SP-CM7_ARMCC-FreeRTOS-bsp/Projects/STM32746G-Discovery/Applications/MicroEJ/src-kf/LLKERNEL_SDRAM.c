/*
 * C
 *
 * Copyright 2013-2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "LLKERNEL_impl.h"
#include "BESTFIT_ALLOCATOR.h"
#include <stdio.h>
#include <string.h>
#include "bsp_util.h"

// declare MicroEJ KF Working buffer
BSP_DECLARE_BUFFER(KERNEL_WORKING_BUFFER)

BESTFIT_ALLOCATOR allocatorInstance;
uint32_t KERNEL_allocationInitialized;

/* API -----------------------------------------------------------------------*/

void* LLKERNEL_IMPL_allocate(int32_t size){
//  return NULL;
	if(KERNEL_allocationInitialized == 0){
		// lazy init
		BESTFIT_ALLOCATOR_new(&allocatorInstance);
		BESTFIT_ALLOCATOR_initialize(&allocatorInstance, KERNEL_WORKING_BUFFER_START, KERNEL_WORKING_BUFFER_END);
		KERNEL_allocationInitialized = 1;
	}

        void* allocationStart = BESTFIT_ALLOCATOR_allocate(&allocatorInstance, size);
#ifdef ALLOCATOR_DEBUG
	printf("LLKERNEL_IMPL_allocate %d %p \n", size, allocationStart);
#endif
        return (void*)allocationStart;
}

void LLKERNEL_IMPL_free(void* block){
//  return;
    BESTFIT_ALLOCATOR_free(&allocatorInstance, block);
#ifdef ALLOCATOR_DEBUG
    printf("LLKERNEL_IMPL_free %p \n", block);
#endif
}

