/**
 *
 * Java
 *
 * Copyright 2015 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;


public class ProducerConsumerExample {

	final static int ACCELEROMETER_DATA_QUEUE_ID = 7;
	final static int ACCELEROMETER_DATA_SIZE = 4;

	final static int MESSENGER_DATA_QUEUE_ID = 8;
	final static int MESSENGER_DATA_SIZE = 255;

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		AccelerometerDataConsumer accelerometerDataConsumer = new AccelerometerDataConsumer(ACCELEROMETER_DATA_QUEUE_ID,ACCELEROMETER_DATA_SIZE);
		new Thread(accelerometerDataConsumer).start();

		MessengerDataConsumer messengerConsumer = new MessengerDataConsumer(MESSENGER_DATA_QUEUE_ID,MESSENGER_DATA_SIZE);
		new Thread(messengerConsumer).start();

	}
	
}
