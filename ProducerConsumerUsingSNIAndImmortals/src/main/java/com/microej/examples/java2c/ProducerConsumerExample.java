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


public class ProducerConsumerExample {

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		QueueService accelerometerQueue = new QueueService(AccelerometerData.getQueuePtr());
		QueueService messengerQueue = new QueueService(MessengerData.getQueuePtr());

		AccelerometerDataProducer accelerometerDataProducer = new AccelerometerDataProducer(accelerometerQueue, 1100, 3);
		new Thread(accelerometerDataProducer).start();

		AccelerometerDataConsumer accelerometerDataConsumer = new AccelerometerDataConsumer(accelerometerQueue);
		new Thread(accelerometerDataConsumer).start();

		MessengerDataConsumer messengerConsumer = new MessengerDataConsumer(messengerQueue);
		new Thread(messengerConsumer).start();

	}

}
