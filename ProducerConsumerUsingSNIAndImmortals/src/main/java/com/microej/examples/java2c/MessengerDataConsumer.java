/**
 *
 * Java
 *
 * Copyright 2015 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

import java.io.IOException;

import com.microej.examples.nativequeue.api.QueueOperationReturnCode;
import com.microej.examples.nativequeue.api.QueueService;

import ej.bon.Immortals;


public class MessengerDataConsumer implements Runnable{

	//ensure no one else accesses this
	private QueueService queueService;
	private int queueId;
	private int dataSize;
	private final byte[] data;

	
	public MessengerDataConsumer(int queueId) {
		this.queueId = queueId;
		this.dataSize = MessengerData.MESSENGER_MAX_DATA_SIZE;

		this.queueService = new QueueService(this.queueId);

		//need to make the byte [] immortal to bypass SNI API constraints on parameters that specify that
		//"the native functions cannot access Java objects field nor methods"
		this.data = (byte[]) Immortals.setImmortal(new byte[this.dataSize]);
	}


	@Override
	public void run() {
		for (;;)
		{
			//use the QueueService to retrieve data
			try {
				final int readStatus = this.queueService.read(this.data);
				if ( QueueOperationReturnCode.QUEUE_READ_OK == readStatus)
				{
					MessengerData dataWrapper = new MessengerData(data);
					System.out.println("-" + dataWrapper.toString());
				}
				else
				{
					System.out.println(this.getClass().getName() + " failure reading data " + QueueOperationReturnCode.toString(readStatus));
				}
			}
			catch ( IOException e)
			{
				e.printStackTrace();
			}
		}
	}

}
