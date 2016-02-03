/*
 *
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

import java.io.IOException;

import com.microej.examples.nativequeue.api.QueueService;

import ej.bon.Immortals;


public class MessengerDataConsumer implements Runnable{

	//ensure no one else accesses this
	private final QueueService queueService;
	private final byte[] data;

	
	public MessengerDataConsumer(QueueService queueService) {
		this.queueService = queueService;

		//need to make the byte [] immortal to bypass SNI API constraints on parameters that specify that
		//"the native functions cannot access Java objects field nor methods"
		this.data = (byte[]) Immortals.setImmortal(new byte[MessengerData.MESSENGER_MAX_DATA_SIZE]);
	}


	@Override
	public void run() {
		for (;;)
		{
			//use the QueueService to retrieve data
			try {
				this.queueService.read(this.data);
				MessengerData dataHolder = new MessengerData(data);
				System.out.println("-" + dataHolder.toString());
			}
			catch ( IOException e)
			{
				e.printStackTrace();
			}
		}
	}

}
