package com.microej.examples.nativequeue.api;

public interface QueueOperationReturnCode {
	final static int QUEUE_INVALID_ID = 0;
	final static int QUEUE_UNREGISTERED = 1;
	final static int QUEUE_REGISTERED = 2;
	final static int QUEUE_READ_OK = 3;
	final static int QUEUE_READ_FAILED = 4;
	final static int QUEUE_WRITE_OK = 5;
	final static int QUEUE_WRITE_FAILED = 6;
}