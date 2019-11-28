/*
 *
 * Java
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
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
