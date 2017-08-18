/*
 *
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.example.java2c;

import com.microej.example.nativequeue.api.QueueService;


public class ProducerConsumerExample {

	// No visible constructor for Utility class
	private ProducerConsumerExample()
	{
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		QueueService accelerometerQueue = new QueueService(getAccelerometerQueuePtr());

		AccelerometerDataProducer accelerometerDataProducer = new AccelerometerDataProducer(accelerometerQueue, 3, 2200);
		new Thread(accelerometerDataProducer).start();

		AccelerometerDataConsumer accelerometerDataConsumer = new AccelerometerDataConsumer(accelerometerQueue);
		new Thread(accelerometerDataConsumer).start();
	}

	//SNI related native - retrieves pointer to queue
	public static native int getAccelerometerQueuePtr();
}