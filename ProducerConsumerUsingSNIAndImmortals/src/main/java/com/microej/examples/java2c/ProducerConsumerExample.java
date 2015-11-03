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

	final static int QUEUE_ID = 7;
	final static int DATA_SIZE = 4;

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		AccelerometerDataConsumer consumer = new AccelerometerDataConsumer(QUEUE_ID,DATA_SIZE);
		new Thread(consumer).start();

/*		AccelerometerDataProducer producer = new AccelerometerDataProducer(3,1200);
		new Thread(producer).start();

		AccelerometerDataProducerUsingTimerTask producerUsingTimerTask = new AccelerometerDataProducerUsingTimerTask(4,1200);
		producerUsingTimerTask.start();*/

	}
	
}
