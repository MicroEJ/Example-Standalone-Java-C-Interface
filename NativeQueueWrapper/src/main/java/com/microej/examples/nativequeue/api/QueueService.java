package com.microej.examples.nativequeue.api;

import java.io.IOException;

public class QueueService {

	private int queueId;
	
	public QueueService(int queueId) {
		super();
		this.queueId = queueId;
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
