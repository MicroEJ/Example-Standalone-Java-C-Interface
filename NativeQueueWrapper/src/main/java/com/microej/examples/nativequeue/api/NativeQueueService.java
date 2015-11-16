package com.microej.examples.nativequeue.api;

/**
 * 
 * @author IS2T
 * this class is not meant to be visible outside of this package
 */

class NativeQueueService {

	public static native int createQueue(int queueId, int itemSize, int maxItems);

	/**
	 * 
	 * @param fromQueueId the queue ID
	 * @param result the ItemSize of the queue (0 if error happened)
	 * @return {@link QueueOperationReturnCode#QUEUE_INVALID_ID} if queue does not exist
	 */
	public static native int getItemSize(int queueId, int[] result);

	/**
	 * 
	 * @param fromQueueId the queue ID
	 * @param result the number of Items in the queue (0 if error happened)
	 * @return {@link QueueOperationReturnCode#QUEUE_INVALID_ID} if queue does not exist
	 */
	public static native int getItemsCount(int fromQueueId, int[] result);

	/**
	 * 
	 * @param fromQueueId the queue ID
	 * @param result the max number of Items in the queue (0 if error happened)
	 * @return {@link QueueOperationReturnCode#QUEUE_INVALID_ID} if queue does not exist
	 */
	public static native int getMaxItems(int fromQueueId, int[] result);

	/**
	 * 
	 * @param fromQueueId the queue ID
	 * @param dataAsByteArray (must be in the immortals pool, size must match value returned by getItemSize(fromQueueId) )
	 * @return true if read succeeded, false otherwise
	 */
	public static native int read(int fromQueueId, byte [] itemDataAsByteArray);

	/**
	 * 
	 * @param toQueueId the queue ID
	 * @param dataAsByteArray (must be in the immortals pool, size must match value returned by getItemSize(toQueueId))
	 * @return true if write succeeded, false otherwise
	 */
	public static native int write(int toQueueId, byte [] itemDataAsByteArray);

}
