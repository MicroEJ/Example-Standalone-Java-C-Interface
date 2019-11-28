/*
 * C
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
#include "LLNativeQueueService.h"

#include <string.h> //for memcmp()

//TODO implement tests for each API

//jboolean LLQueue_init(queue_service_descriptor_t* queue, const xQueueHandle queueHandle, const jint itemSize, const jint maxItems );
void testLLQueue_init()
{
	//check valid ranges for input params
	// return_value == false : if ( NULL == queue )
	// return_value == false : if ( NULL != queue->queueHandle ) //already existing queue ?
	// return_value == false : if ( itemSize ! in [0,???] )
	// return_value == false : if ( maxItems ! in [0,???] )
}

//jint LLQueue_getItemSize(const queue_service_descriptor_t* queue, jint* result);
void testLLQueue_getItemSize()
{
	//check valid ranges for input params
	// return_value == QUEUE_INVALID_QUEUE : if ( NULL == queue )
	// *result > 0
}

//jint LLQueue_getItemsCount(const queue_service_descriptor_t* queue, jint* result);
void testLLQueue_getItemsCount()
{
	//check valid ranges for input params
	// return_value == QUEUE_INVALID_QUEUE : if ( NULL == queue )
	// *result >= 0}

//jint LLQueue_getMaxItems(const queue_service_descriptor_t* queue, jint* result);
void testLLQueue_getMaxItems()
{
	//check valid ranges for input params
	// return_value == QUEUE_INVALID_QUEUE : if ( NULL == queue )
	// *result >= 0}
}

//jint LLQueue_read(queue_service_descriptor_t* fromQueue, volatile jbyte* itemDataAsByteArray);
void testLLQueue_read()
{
	//check valid ranges for input params
	// return_value == QUEUE_INVALID_QUEUE : if ( NULL == fromQueue )
	// return_value == QUEUE_INVALID_QUEUE : if ( NULL == fromQueue->queueHandle )
	// return_value == QUEUE_READ_FAILED : if ( NULL == itemDataAsByteArray )

	//check result
	// return_value == QUEUE_READ_FAILED : if ( 0 == LLQueue_getItemsCount(fromQueue) ) //empty queue
	// return_value == QUEUE_SERVICE_OK : if ( 0 < LLQueue_getItemsCount(fromQueue) ) //NOT empty queue
	// 0 == memcmp (*itemDataAsByteArray,*originalDataAsByteArray,LLQueue_getItemSize(fromQueue)) //after having called LLQueueWrite(fromQueue,originalDataAsByteArray)
}

//jint LLQueue_write(const queue_service_descriptor_t* toQueue, volatile jbyte* itemDataAsByteArray);
void testLLQueue_write()
{
	//check valid ranges for input params
	// return_value == QUEUE_INVALID_QUEUE : if ( NULL == fromQueue )
	// return_value == QUEUE_INVALID_QUEUE : if ( NULL == fromQueue->queueHandle )
	// return_value == QUEUE_WRITE_FAILED : if ( NULL == itemDataAsByteArray )

	//check result
	// return_value == QUEUE_WRITE_FAILED : if ( LLQueue_getItemsCount(toQueue) ==  LLQueue_getMaxItems(toQueue) ) //queue full

	// ? check available memory after call == available memory before call - itemSize
}


void testLLQueue_FIFO_behavior()
{
	//ensure that elements go out in the same order as they came in
}
