/*
 * C
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
#include "LLNativeQueueService.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"

const struct queue_service_descriptor_t UNINITIALIZED_registry_entry =
{
		NULL,
		0,
		0,
		0
};

//== helper methods declarations
void _LLQueue_pauseCurrentJavaThread(queue_service_descriptor_t* fromQueue);
void _LLQueue_resumePendingJavaThread(const queue_service_descriptor_t* fromQueue);
jint _LLQueue_read(queue_service_descriptor_t* fromQueue, volatile jbyte* itemDataAsByteArray, const jboolean fromJava);
jint _LLQueue_write(const queue_service_descriptor_t* toQueue, volatile jbyte* itemDataAsByteArray, const jboolean fromJava);


//== regular queue API
jboolean LLQueue_init(queue_service_descriptor_t* queue, const xQueueHandle queueHandle, const jint itemSize, const jint maxItems )
{
	jboolean result = JFALSE;
	if ( NULL != queue )
	{
		if ( NULL == queue->queueHandle )
		{
			queue->queueHandle = queueHandle;
			queue->itemSize = itemSize;
			queue->maxItems = maxItems;
			queue->pendingJavaThreadId = 0;
			result = JTRUE;
		}
		else
		{
			//queue->queueHandle should be null before initialization
			result = JFALSE;
		}
	}	
	return result;
}


//--------------------
// each function shall check queue parameter

jint LLQueue_getItemSize(const queue_service_descriptor_t* queue,  jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( NULL != queue )
	{
		*result = 0;
		*result = queue->itemSize;
		errorCode = QUEUE_SERVICE_OK;
	}
	else
	{
		errorCode = QUEUE_INVALID_QUEUE;
	}
	return errorCode;
}

jint LLQueue_getItemsCount(const queue_service_descriptor_t* queue, jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( NULL != queue )
	{
		*result = 0;
		if ( NULL != queue->queueHandle )
		{
			*result = uxQueueMessagesWaiting(queue->queueHandle);
		}
		errorCode = QUEUE_SERVICE_OK;
	}
	else
	{
		errorCode = QUEUE_INVALID_QUEUE;
	}

	return errorCode;
}

jint LLQueue_getMaxItems(const queue_service_descriptor_t* queue, jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( NULL != queue )
	{
		*result = queue->maxItems;
		errorCode = QUEUE_SERVICE_OK;
	}
	else
	{
		errorCode = QUEUE_INVALID_QUEUE;
	}

	return errorCode;
}

jint LLQueue_read(queue_service_descriptor_t* fromQueue, volatile jbyte* itemDataAsByteArray){
	return _LLQueue_read(fromQueue,itemDataAsByteArray,JFALSE);
}

jint LLQueue_write(const queue_service_descriptor_t* toQueue, volatile jbyte* itemDataAsByteArray)
{
	return _LLQueue_write(toQueue, itemDataAsByteArray, JFALSE);
}


//== helper methods implementations
void _LLQueue_pauseCurrentJavaThread(queue_service_descriptor_t* fromQueue)
{
	if ( NULL != fromQueue )
	{
		xQueueHandle currentQueue = fromQueue->queueHandle;
		if ( NULL != currentQueue )
		{
			fromQueue->pendingJavaThreadId = SNI_getCurrentJavaThreadID();
			SNI_suspendCurrentJavaThread(0);
		}
	}
}

void _LLQueue_resumePendingJavaThread(const queue_service_descriptor_t* fromQueue)
{
	if ( NULL != fromQueue )
	{
		xQueueHandle currentQueue = fromQueue->queueHandle;
		if ( NULL != currentQueue )
		{
			if ( 0 != fromQueue->pendingJavaThreadId )
			{
				SNI_resumeJavaThread(fromQueue->pendingJavaThreadId );
			}
		}
	}
}

jint _LLQueue_read(queue_service_descriptor_t* fromQueue, volatile jbyte* itemDataAsByteArray, const jboolean fromJava)
{
	jint result = QUEUE_INVALID_QUEUE;
	if ( NULL != fromQueue )
	{
		xQueueHandle currentQueue = fromQueue->queueHandle;
		if ( NULL != currentQueue )
		{
			result = QUEUE_READ_FAILED;
			if ( NULL != itemDataAsByteArray )
			{
				jboolean sizeCheckPassed = JFALSE;
				if ( JTRUE == fromJava)
				{
					jint arrayLength = SNI_getArrayLength(itemDataAsByteArray);
					if ( arrayLength == fromQueue->itemSize )
					{
						sizeCheckPassed = JTRUE;
					}
				}
				else
				{
					sizeCheckPassed = JTRUE;
				}

				if ( JTRUE == sizeCheckPassed )
				{
					portBASE_TYPE dataReceived = xQueueReceive(currentQueue, itemDataAsByteArray, 0);
					if(!dataReceived){
						if ( JTRUE == fromJava )
						{
							//the Java Thread waiting for data will be paused right after the end of this native method
							//it will be woken up when data is available
							_LLQueue_pauseCurrentJavaThread(fromQueue);
						}
					}
					else {
						result = QUEUE_SERVICE_OK;
					}
				}
			}
		}
	}
	return result;
}

jint _LLQueue_write(const queue_service_descriptor_t* toQueue, volatile jbyte* itemDataAsByteArray, const jboolean fromJava)
{
	jint result = QUEUE_INVALID_QUEUE;
	if ( NULL != toQueue )
	{
		xQueueHandle currentQueue = toQueue->queueHandle;
		if ( NULL != currentQueue )
		{
			result = QUEUE_WRITE_FAILED;
			if ( NULL != itemDataAsByteArray )
			{
				jboolean sizeCheckPassed = JFALSE;
				if ( JTRUE == fromJava)
				{
					jint arrayLength = SNI_getArrayLength(itemDataAsByteArray);
					if ( arrayLength == toQueue->itemSize )
					{
						sizeCheckPassed = JTRUE;
					}
				}
				else
				{
					sizeCheckPassed = JTRUE;
				}

				if ( JTRUE == sizeCheckPassed )
				{
					portBASE_TYPE dataSent = xQueueSend(currentQueue, (void * const) itemDataAsByteArray, 10);
					if (dataSent)
						result = QUEUE_SERVICE_OK;
					}
				}
			}
		//wake up any pending Java Thread (if any)
		_LLQueue_resumePendingJavaThread(toQueue);
	}
	return result;
}

//== SNI wrappers
jint Java_com_microej_example_nativequeue_api_NativeQueueService_getItemSize(const jint queueId)
{
	jint result;
	jint size;
	result = LLQueue_getItemSize((queue_service_descriptor_t*)queueId, &size);
	if(result < 0){
		return result;
	}
	else {
		return size;
	}
}

jint Java_com_microej_example_nativequeue_api_NativeQueueService_getItemsCount(const jint queueId)
{
	jint result;
	jint count;
	result = LLQueue_getItemsCount((queue_service_descriptor_t*)queueId, &count);
	if(result < 0){
		return result;
	}
	else {
		return count;
	}
}

jint Java_com_microej_example_nativequeue_api_NativeQueueService_getMaxItems(const jint queueId)
{
	jint result;
	jint maxItems;
	result = LLQueue_getMaxItems((queue_service_descriptor_t*)queueId, &maxItems);
	if(result < 0){
		return result;
	}
	else {
		return maxItems;
	}
}

jint Java_com_microej_example_nativequeue_api_NativeQueueService_read(const jint fromQueueId, volatile jbyte* itemDataAsByteArray)
{	
	return _LLQueue_read((queue_service_descriptor_t*)fromQueueId,itemDataAsByteArray,JTRUE);
}

jint Java_com_microej_example_nativequeue_api_NativeQueueService_write(const jint toQueueId, volatile jbyte* itemDataAsByteArray)
{
	return _LLQueue_write((queue_service_descriptor_t*)toQueueId,itemDataAsByteArray,JTRUE);
}
