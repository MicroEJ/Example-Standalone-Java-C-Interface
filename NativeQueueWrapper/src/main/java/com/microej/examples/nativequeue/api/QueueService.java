/*
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
package com.microej.examples.nativequeue.api;

import java.io.IOException;

public class QueueService {

	private final int queuePtr; //store pointer to queue as a java int
	private final Object readMonitor = new Object();
	private final Object writeMonitor = new Object();

	public QueueService(int queuePtr) {
		super();
		this.queuePtr = queuePtr;
	}

	public synchronized int getItemsCount() throws IOException{
		int result = NativeQueueService.getItemsCount(this.queuePtr);
		if( result < 0 ) {
			throw new IOException(NativeQueueService.toStringError(result));
		}
		return result;
	}

	public synchronized int getItemSize() throws IOException{
		int result = NativeQueueService.getItemSize(this.queuePtr);
		if( result < 0 ) {
			throw new IOException(NativeQueueService.toStringError(result));
		}
		return result;
	}

	public synchronized int getMaxItems() throws IOException{
		int result = NativeQueueService.getMaxItems(this.queuePtr);
		if( result < 0 ) {
			throw new IOException(NativeQueueService.toStringError(result));
		}
		return result;
	}

	public void read(byte[] output) throws IOException{
		synchronized (readMonitor) {
			int errorCode = NativeQueueService.read(queuePtr, output);
			if( errorCode == NativeQueueService.QUEUE_READ_FAILED ){
				errorCode = NativeQueueService.read(queuePtr, output);
			}
			if( errorCode != NativeQueueService.QUEUE_SERVICE_OK ) {
				throw new IOException(NativeQueueService.toStringError(errorCode));
			}
		}
	}

	public void write(byte[] input) throws IOException{
		synchronized (writeMonitor) {
			int errorCode = NativeQueueService.write(queuePtr, input);
			if( errorCode == NativeQueueService.QUEUE_WRITE_FAILED ){
				errorCode = NativeQueueService.write(queuePtr, input);
			}
			if( errorCode != NativeQueueService.QUEUE_SERVICE_OK ) {
				throw new IOException(NativeQueueService.toStringError(errorCode));
			}
		}
	}

}
