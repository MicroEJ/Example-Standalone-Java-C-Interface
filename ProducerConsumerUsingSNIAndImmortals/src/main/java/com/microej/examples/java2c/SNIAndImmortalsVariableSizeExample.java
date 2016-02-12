/*
 *
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

import com.microej.examples.nativequeue.api.QueueService;


public class SNIAndImmortalsVariableSizeExample {

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		QueueService messengerQueue = new QueueService(MessengerData.getQueuePtr());
		
		MessengerDataConsumer messengerConsumer = new MessengerDataConsumer(messengerQueue);
		new Thread(messengerConsumer).start();

	}

}
