/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 *
 */
#include "LLNativeQueueService.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

typedef struct queue_registry_entry_t {
	xQueueHandle queueHandle;
	jint itemSize;
	jint maxItems;
	jint javaThreadId;
} queue_registry_entry_t;

const struct queue_registry_entry_t UNINITIALIZED_registry_entry =
{
		NULL,
		0,
		0,
		0
};

static queue_registry_entry_t queue_registry[MAX_QUEUES_IN_REGISTRY];

static xSemaphoreHandle queue_registry_mutex;

//== helper methods declarations
jboolean _LLQueue_init(void);
inline jboolean _LLQueue_isValidQueueId(jint QueueId);
void _LLQueue_pauseCurrentJavaThread(jint fromQueueId);
void _LLQueue_resumePendingJavaThread(jint fromQueueId);
jint _LLQueue_isRegistered(jint queueId, jint itemSize, jint maxItems);
jint _LLQueue_read(jint fromQueueId, jbyte* itemDataAsByteArray, jboolean fromJava);
jint _LLQueue_write(jint toQueueId, jbyte* itemDataAsByteArray, jboolean fromJava);
jint _LLQueue_registerQueue(jint queueId, xQueueHandle queueHandle, jint itemSize, jint maxItems);
jint _LLQueue_unregisterQueue(jint queueId, xQueueHandle queueHandle);
static void _LLQueue_queue_registry_mutex_lock(void);
static void _LLQueue_queue_registry_mutex_unlock(void);

//== regular queue API
jboolean _LLQueue_init(void)
{
	jboolean succeeded = JFALSE;
	//may have already been called before
	if ( NULL == queue_registry_mutex )
	{
		queue_registry_mutex = xSemaphoreCreateMutex();
		if ( NULL == queue_registry_mutex )
		{
			printf("%s failed to create queue_registry_mutex",__PRETTY_FUNCTION__);
		}
		else
		{
			succeeded = JTRUE;
		}
	}
	return succeeded;
}

// each function shall check queueId parameter
jint LLQueue_createQueue(jint queueId, jint itemSize, jint maxItems)
{
	jint result = QUEUE_CREATE_FAILED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		_LLQueue_init();
		_LLQueue_queue_registry_mutex_lock();
		if ( QUEUE_REGISTERED != _LLQueue_isRegistered(queueId, itemSize, maxItems) )
		{
			xQueueHandle newQueue = xQueueCreate(maxItems,itemSize);
			_LLQueue_registerQueue(queueId,newQueue,itemSize,maxItems);
		}
		_LLQueue_queue_registry_mutex_unlock();
		result = QUEUE_CREATE_OK;
	}
	else
	{
		result = QUEUE_INVALID_ID;
	}
	return result;
}

jint LLQueue_destroyQueue(jint queueId)
{
	jint errorCode = QUEUE_DELETE_FAILED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		_LLQueue_queue_registry_mutex_lock();
		if ( NULL != queue_registry[queueId].queueHandle )
		{
			vQueueDelete(queue_registry[queueId].queueHandle);
			_LLQueue_unregisterQueue(queueId,queue_registry[queueId].queueHandle);
		}
		_LLQueue_queue_registry_mutex_unlock();
		errorCode = QUEUE_DELETE_OK;
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}
	return errorCode;
}

jint LLQueue_getItemSize(jint queueId,  jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		if ( NULL != result )
		{
			*result = 0;
			*result = queue_registry[queueId].itemSize;
			errorCode = QUEUE_READ_OK;
		}
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}
	return errorCode;
}

jint LLQueue_getItemsCount(jint queueId,  jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		if ( NULL != result )
		{
			*result = 0;
			if ( NULL != queue_registry[queueId].queueHandle )
			{
				*result = uxQueueMessagesWaiting(queue_registry[queueId].queueHandle);
			}
			errorCode = QUEUE_READ_OK;
		}
		else
		{
			printf("%s result == NULL \n",__PRETTY_FUNCTION__);
		}
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}

	return errorCode;
}

jint LLQueue_getMaxItems(jint queueId, jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		if ( NULL != result )
		{
			*result = 0;
			*result = queue_registry[queueId].maxItems;
			errorCode = QUEUE_READ_OK;
		}
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}

	return errorCode;
}

jint LLQueue_read(jint fromQueueId, jbyte* itemDataAsByteArray){
	jint errorCode = QUEUE_READ_FAILED;
	if ( _LLQueue_isValidQueueId(fromQueueId) )
	{
		errorCode =_LLQueue_read(fromQueueId,itemDataAsByteArray,JFALSE);
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}

	return errorCode;
}

jint LLQueue_write(jint toQueueId, jbyte* itemDataAsByteArray)
{
	jint errorCode = QUEUE_WRITE_FAILED;
	if ( _LLQueue_isValidQueueId(toQueueId) )
	{
		errorCode = _LLQueue_write(toQueueId, itemDataAsByteArray, JFALSE);
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}

	return errorCode;		
}

//== helper methods implementations
jboolean _LLQueue_isValidQueueId(jint queueId)
{
	//check that queueId in [0,MAX_QUEUES_IN_REGISTRY]
	jboolean result = ( queueId >= 0 && queueId < MAX_QUEUES_IN_REGISTRY );
	return result;
}

jint _LLQueue_registerQueue(jint queueId, xQueueHandle queueHandle, jint itemSize, jint maxItems)
{
	jint result = QUEUE_UNREGISTERED;
	queue_registry[queueId].queueHandle = queueHandle;
	printf("%s queue_registry[queueId].queueHandle : %d\n",__PRETTY_FUNCTION__, (void*) queue_registry[queueId].queueHandle );

	queue_registry[queueId].itemSize = itemSize;
	queue_registry[queueId].maxItems = maxItems;
	result = QUEUE_REGISTERED;
	return result;
}

jint _LLQueue_isRegistered(jint queueId, jint itemSize, jint maxItems)
{
	jint result = QUEUE_UNREGISTERED;
	if ( 
		NULL != queue_registry[queueId].queueHandle &&
		itemSize == queue_registry[queueId].itemSize &&
		maxItems == queue_registry[queueId].maxItems
	)
	{
		result = QUEUE_REGISTERED;
	}
	return result;
}

jint _LLQueue_unregisterQueue(jint queueId, xQueueHandle queueHandle)
{
	jint result = QUEUE_REGISTERED;
	queue_registry[queueId] = UNINITIALIZED_registry_entry;
	result = QUEUE_UNREGISTERED;
	return result;
}

void _LLQueue_pauseCurrentJavaThread(jint fromQueueId)
{
	xQueueHandle currentQueue = queue_registry[fromQueueId].queueHandle;
	if ( NULL != currentQueue )
	{
		queue_registry[fromQueueId].javaThreadId = SNI_getCurrentJavaThreadID();
		SNI_suspendCurrentJavaThread(queue_registry[fromQueueId].javaThreadId);
	}
}

void _LLQueue_resumePendingJavaThread(jint fromQueueId)
{
	xQueueHandle currentQueue = queue_registry[fromQueueId].queueHandle;
	if ( NULL != currentQueue )
	{
		SNI_resumeJavaThread(queue_registry[fromQueueId].javaThreadId );
	}
}

jint _LLQueue_read(jint fromQueueId, jbyte* itemDataAsByteArray, jboolean fromJava)
{
	jint result = QUEUE_READ_FAILED;
	xQueueHandle currentQueue = queue_registry[fromQueueId].queueHandle;
	if ( NULL != currentQueue )
	{
		jboolean sizeCheckPassed = JFALSE;
		if ( JTRUE == fromJava)
		{
			if ( SNI_getArrayLength(itemDataAsByteArray) ==  queue_registry[fromQueueId].itemSize )
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
				_LLQueue_pauseCurrentJavaThread(fromQueueId);
			}
			else {
				result = QUEUE_READ_OK;
			}
		}
	}
	return result;
}

jint _LLQueue_write(jint toQueueId, jbyte* itemDataAsByteArray, jboolean fromJava)
{
	jint result = QUEUE_WRITE_FAILED;

	xQueueHandle currentQueue = queue_registry[toQueueId].queueHandle;
	if ( NULL != currentQueue )
	{
		jboolean sizeCheckPassed = JFALSE;
		if ( JTRUE == fromJava)
		{
			jint arrayLength =  SNI_getArrayLength(itemDataAsByteArray);
			if ( arrayLength == queue_registry[toQueueId].itemSize )
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
			portBASE_TYPE dataSent = xQueueSend(currentQueue, itemDataAsByteArray, 10);
			if (!dataSent)
			{
				result = QUEUE_WRITE_FAILED;
			}
			else
			{
				result = QUEUE_WRITE_OK;
			}
		}
	}
	_LLQueue_resumePendingJavaThread(toQueueId);
	return result;
}

static void _LLQueue_queue_registry_mutex_lock(void)
{
	xSemaphoreTake(queue_registry_mutex, portMAX_DELAY);
}

static void _LLQueue_queue_registry_mutex_unlock(void)
{
	xSemaphoreGive(queue_registry_mutex);
}

//== SNI wrappers
/*jboolean Java_com_microej_examples_nativequeue_api_NativeQueueService_init(void)
{
	return LLQueue_init();
}*/

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_createQueue(jint queueId, jint itemSize, jint maxItems)
{
	return LLQueue_createQueue(queueId,itemSize,maxItems);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_destroyQueue(jint queueId)
{
	return LLQueue_destroyQueue(queueId);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemSize(jint queueId,  jint* result)
{
	return LLQueue_getItemSize(queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemsCount(jint queueId, jint* result)
{
	return LLQueue_getItemsCount(queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getMaxItems(jint queueId, jint* result)
{
	return LLQueue_getMaxItems(queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_read(jint fromQueueId, jbyte* itemDataAsByteArray)
{	
	return _LLQueue_read(fromQueueId,itemDataAsByteArray,JTRUE);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_write(jint toQueueId, jbyte* itemDataAsByteArray)
{
	return _LLQueue_write(toQueueId,itemDataAsByteArray,JTRUE);
}

