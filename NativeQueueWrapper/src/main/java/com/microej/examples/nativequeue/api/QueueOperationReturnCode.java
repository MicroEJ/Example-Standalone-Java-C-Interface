package com.microej.examples.nativequeue.api;

public final class QueueOperationReturnCode {
	public final static int QUEUE_INVALID_ID = 0;
	public final static int QUEUE_UNREGISTERED = 1;
	public final static int QUEUE_REGISTERED = 2;
	public final static int QUEUE_READ_OK = 3;
	public final static int QUEUE_READ_FAILED = 4;
	public final static int QUEUE_WRITE_OK = 5;
	public final static int QUEUE_WRITE_FAILED = 6;
	public final static int QUEUE_DELETE_OK = 7;
	public final static int QUEUE_DELETE_FAILED = 8;
	
	//this class is not meant to be instantiated
	private QueueOperationReturnCode(){
	}
	
	static public String toString(int returnCode){
		String result = "UNKNOWN_CODE";
		switch (returnCode)
		{
			case QUEUE_INVALID_ID 		: result = "QUEUE_INVALID_ID"; break;
			case QUEUE_UNREGISTERED 	: result = "QUEUE_UNREGISTERED"; break;
			case QUEUE_REGISTERED 		: result = "QUEUE_REGISTERED"; break;
			case QUEUE_READ_OK 			: result = "QUEUE_READ_OK"; break;
			case QUEUE_READ_FAILED 		: result = "QUEUE_READ_FAILED"; break;
			case QUEUE_WRITE_OK 		: result = "QUEUE_WRITE_OK"; break;
			case QUEUE_WRITE_FAILED 	: result = "QUEUE_WRITE_FAILED"; break;
			case QUEUE_DELETE_OK 		: result = "QUEUE_DELETE_OK"; break;
			case QUEUE_DELETE_FAILED 	: result = "QUEUE_DELETE_FAILED"; break;
		}
		return result;
	}
}