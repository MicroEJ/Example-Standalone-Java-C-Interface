#ifndef LL_NATIVE_QUEUE_SERVICE_H
#define LL_NATIVE_QUEUE_SERVICE_H

#include <sni.h>

#define MAX_QUEUES_IN_REGISTRY 10

//used for operations return value
typedef enum queue_operation_return_code_t {
	QUEUE_INVALID_ID = 0,
	QUEUE_CREATE_OK = 1,
	QUEUE_CREATE_FAILED = 2,
	QUEUE_UNREGISTERED = 3,
	QUEUE_REGISTERED = 4,
	QUEUE_DELETE_OK = 5,
	QUEUE_DELETE_FAILED = 6,
	QUEUE_READ_OK = 7,
	QUEUE_READ_FAILED = 8,
	QUEUE_WRITE_OK = 9,
	QUEUE_WRITE_FAILED = 10,
	_QUEUE_FORCE_REPRESENTATION_AS_INT_32 = INT32_MAX // will force representation as int 32, which is typedef'ed by jint
} queue_operation_return_code_t;

//== regular queue API
jint LLQueue_createQueue(jint queueId, jint itemSize, jint maxItems);
jint LLQueue_destroyQueue(jint queueId);
jint LLQueue_getItemSize(jint queueId, jint* result);
jint LLQueue_getItemsCount(jint queueId, jint* result);
jint LLQueue_getMaxItems(jint queueId, jint* result);
jint LLQueue_read(jint fromQueueId, jbyte* itemDataAsByteArray);
jint LLQueue_write(jint toQueueId, jbyte* itemDataAsByteArray);

//== SNI wrappers
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_createQueue(jint queueId, jint itemSize, jint maxItems);
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_destroyQueue(jint queueId);
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemSize(jint queueId, jint* result);
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemsCount(jint queueId, jint* result);
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getMaxItems(jint queueId, jint* result);
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_read(jint fromQueueId, jbyte* itemDataAsByteArray);
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_write(jint toQueueId, jbyte* itemDataAsByteArray);

#endif // LL_NATIVE_QUEUE_SERVICE_H
