/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 *
 */
#ifndef LL_NATIVE_QUEUE_SERVICE_H
#define LL_NATIVE_QUEUE_SERVICE_H

#include <sni.h>
#include "FreeRTOS.h"
#include "queue.h"

typedef struct queue_service_descriptor_t {
	xQueueHandle queueHandle;
	jint itemSize;
	jint maxItems;
	jint pendingJavaThreadId;
} queue_service_descriptor_t;

//Used for operations return value
//WARNING: keep these values synchronized with the constants defined in
//com.microej.examples.nativequeue.api.QueueOperationReturnCode Java class.
typedef enum queue_operation_return_code_t {
	QUEUE_SERVICE_OK = 0,
	QUEUE_INVALID_QUEUE = 1,
	QUEUE_READ_FAILED = 2,
	QUEUE_WRITE_FAILED = 3,
	_QUEUE_FORCE_REPRESENTATION_AS_INT_32 = INT32_MAX // will force representation as int 32, which is typedef'ed by jint
} queue_operation_return_code_t;


//== regular queue API
jboolean LLQueue_init(queue_service_descriptor_t* queue, const xQueueHandle queueHandle, const jint itemSize, const jint maxItems );
jint LLQueue_getItemSize(const queue_service_descriptor_t* queue, jint* result);
jint LLQueue_getItemsCount(const queue_service_descriptor_t* queue, jint* result);
jint LLQueue_getMaxItems(const queue_service_descriptor_t* queue, jint* result);
jint LLQueue_read(queue_service_descriptor_t* fromQueue, volatile jbyte* itemDataAsByteArray);
jint LLQueue_write(const queue_service_descriptor_t* toQueue, volatile jbyte* itemDataAsByteArray);


//== SNI wrappers
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemSize(const jint queueId, jint* result);
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemsCount(const jint queueId, jint* result);
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getMaxItems(const jint queueId, jint* result);
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_read(const jint fromQueueId, volatile jbyte* itemDataAsByteArray);
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_write(const jint toQueueId, volatile jbyte* itemDataAsByteArray);

#endif // LL_NATIVE_QUEUE_SERVICE_H
