/**
 *
 * Java
 *
 * Copyright 2015 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.is2t.examples.java2c;


public class DataConsumerExample {

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		AccelerometerDataConsumer consumer = new AccelerometerDataConsumer();
		new Thread(consumer).start();

		AccelerometerDataProducer producer = new AccelerometerDataProducer(3,1200);
		new Thread(producer).start();

		AccelerometerDataProducerUsingTimerTask producerUsingTimerTask = new AccelerometerDataProducerUsingTimerTask(4,1200);
		producerUsingTimerTask.start();

	}
	
}
