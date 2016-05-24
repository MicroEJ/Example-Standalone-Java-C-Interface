/*
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
package com.microej.examples.nativequeue.api;

/**
 *
 * @author IS2T
 * this class is not meant to be visible outside of this package
 */

class NativeQueueService {

	//WARNING: keep these values synchronized with constants defined in LLNativeQueueService.h C header file.
	public static final int QUEUE_SERVICE_OK = 0;
	public static final int QUEUE_INVALID_QUEUE = -1;
	public static final int QUEUE_READ_FAILED = -2;
	public static final int QUEUE_WRITE_FAILED = -3;

	// No visible constructor for utility class
	private NativeQueueService() {
	}

	public static String toStringError(int returnCode){
		String result = "UNKNOWN_CODE";

		if(returnCode >= 0){
			result = "QUEUE_SERVICE_OK";
		}
		else {
			switch (returnCode)
			{
			case QUEUE_INVALID_QUEUE 	: result = "QUEUE_INVALID_QUEUE"; break;
			case QUEUE_READ_FAILED 		: result = "QUEUE_READ_FAILED"; break;
			case QUEUE_WRITE_FAILED 	: result = "QUEUE_WRITE_FAILED"; break;
			}
		}
		return result;
	}

	/**
	 *
	 * @param fromQueueId the queue ID
	 * @return the ItemSize of the queue or {@link NativeQueueService#QUEUE_INVALID_QUEUE} if queue does not exist
	 */
	public static native int getItemSize(int queueId);

	/**
	 *
	 * @param fromQueueId the queue ID
	 * @return the number of Items in the queue or {@link NativeQueueService#QUEUE_INVALID_QUEUE} if queue does not exist
	 */
	public static native int getItemsCount(int fromQueueId);

	/**
	 *
	 * @param fromQueueId the queue ID
	 * @return the max number of Items in the queue or {@link NativeQueueService#QUEUE_INVALID_QUEUE} if queue does not exist
	 */
	public static native int getMaxItems(int fromQueueId);

	/**
	 *
	 * @param fromQueueId the queue ID
	 * @param dataAsByteArray (must be in the immortals pool, size must match value returned by getItemSize(fromQueueId))
	 * @return {@link #QUEUE_SERVICE_OK} if read succeeded, a negative value otherwise
	 */
	public static native int read(int fromQueueId, byte [] itemDataAsByteArray);

	/**
	 *
	 * @param toQueueId the queue ID
	 * @param dataAsByteArray (must be in the immortals pool, size must match value returned by getItemSize(toQueueId))
	 * @return {@link #QUEUE_SERVICE_OK} if write succeeded, a negative value otherwise
	 */
	public static native int write(int toQueueId, byte [] itemDataAsByteArray);

}
