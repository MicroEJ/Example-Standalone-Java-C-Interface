package com.microej.examples.nativequeue.api;

import java.io.IOException;

public class QueueService {

	private int queueId;
	
	public QueueService(int queueId) {
		super();
		this.queueId = queueId;
	}
	
	synchronized public int createQueue(int itemSize, int maxItems)
	{
		int errorCode = NativeQueueService.createQueue(this.queueId, itemSize, maxItems);
		return errorCode;
	}

	synchronized public int destroyQueue()
	{
		int errorCode = NativeQueueService.destroyQueue(this.queueId);
		return errorCode;
	}

	synchronized public int getItemsCount(int[] itemsCountReferenceHolder) throws IOException{
		int errorCode = NativeQueueService.getItemsCount(this.queueId, itemsCountReferenceHolder);
		return errorCode;
	}

	synchronized public int getItemSize(int[] itemSizeReferenceHolder) throws IOException{
		int errorCode = NativeQueueService.getItemSize(this.queueId, itemSizeReferenceHolder);
		return errorCode;
	}

	synchronized public int getMaxItems(int[] maxItemsReferenceHolder) throws IOException{
		int errorCode = NativeQueueService.getMaxItems(this.queueId, maxItemsReferenceHolder);
		return errorCode;
	}

	synchronized public int read(byte[] output) throws IOException{
		int errorCode = NativeQueueService.read(queueId, output);
		if( QueueOperationReturnCode.QUEUE_READ_FAILED == errorCode ){
			errorCode = NativeQueueService.read(queueId, output);
		}
		return errorCode;
	}

	synchronized public int write(byte[] input) throws IOException{
		int errorCode = NativeQueueService.write(queueId, input);
		if( QueueOperationReturnCode.QUEUE_WRITE_FAILED == errorCode ){
			errorCode = NativeQueueService.write(queueId, input);
		}
		return errorCode;
	}

}
