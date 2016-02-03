package com.microej.examples.nativequeue.api;

import java.io.IOException;

import ej.bon.Immortals;

public class QueueService {

	private final int queuePtr;
	int[] itemSizeReferenceHolder = new int[1];
	int[] maxItemsReferenceHolder = new int[1];
	int[] itemsCountReferenceHolder = new int[1];
	
	public QueueService(int queuePtr) {
		super();
		this.queuePtr = queuePtr;

		Immortals.setImmortal( this.queuePtr);
		Immortals.setImmortal(itemSizeReferenceHolder);
		Immortals.setImmortal(maxItemsReferenceHolder);
		Immortals.setImmortal(itemsCountReferenceHolder);
	}

	synchronized public int getItemsCount() throws IOException{
		int errorCode = NativeQueueService.getItemsCount(this.queuePtr, itemsCountReferenceHolder);
		if( errorCode != NativeQueueService.QUEUE_SERVICE_OK ) {
			throw new IOException(NativeQueueService.toStringError(errorCode));
		}
		return itemsCountReferenceHolder[0];
	}

	synchronized public int getItemSize() throws IOException{
		int errorCode = NativeQueueService.getItemSize(this.queuePtr, itemSizeReferenceHolder);
		if( errorCode != NativeQueueService.QUEUE_SERVICE_OK ) {
			throw new IOException(NativeQueueService.toStringError(errorCode));
		}
		return itemSizeReferenceHolder[0];
	}

	synchronized public int getMaxItems() throws IOException{
		int errorCode = NativeQueueService.getMaxItems(this.queuePtr, maxItemsReferenceHolder);
		if( errorCode != NativeQueueService.QUEUE_SERVICE_OK ) {
			throw new IOException(NativeQueueService.toStringError(errorCode));
		}
		return maxItemsReferenceHolder[0];
	}

	synchronized public void read(byte[] output) throws IOException{
		int errorCode = NativeQueueService.read(queuePtr, output);
		if( errorCode == NativeQueueService.QUEUE_READ_FAILED ){
			errorCode = NativeQueueService.read(queuePtr, output);
		}
		if( errorCode != NativeQueueService.QUEUE_SERVICE_OK ) {
			throw new IOException(NativeQueueService.toStringError(errorCode));
		}
	}

	synchronized public void write(byte[] input) throws IOException{
		int errorCode = NativeQueueService.write(queuePtr, input);
		if( errorCode == NativeQueueService.QUEUE_WRITE_FAILED ){
			errorCode = NativeQueueService.write(queuePtr, input);
		}
		if( errorCode != NativeQueueService.QUEUE_SERVICE_OK ) {
			throw new IOException(NativeQueueService.toStringError(errorCode));
		}
	}

}
