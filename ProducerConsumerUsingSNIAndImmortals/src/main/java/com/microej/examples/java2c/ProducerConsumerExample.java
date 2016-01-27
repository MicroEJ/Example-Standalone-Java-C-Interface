/*
 *
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;


public class ProducerConsumerExample {

	final static int ACCELEROMETER_DATA_QUEUE_ID = 7;

	final static int MESSENGER_DATA_QUEUE_ID = 8;
	/**
	 * @param args
	 */
	public static void main(String[] args) {

		AccelerometerDataProducer accelerometerDataProducer = new AccelerometerDataProducer(ACCELEROMETER_DATA_QUEUE_ID, 1100, 3);
		new Thread(accelerometerDataProducer).start();

		AccelerometerDataConsumer accelerometerDataConsumer = new AccelerometerDataConsumer(ACCELEROMETER_DATA_QUEUE_ID);
		new Thread(accelerometerDataConsumer).start();

		MessengerDataConsumer messengerConsumer = new MessengerDataConsumer(MESSENGER_DATA_QUEUE_ID);
		new Thread(messengerConsumer).start();

	}
	
}
