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
		AccelerometerDataProducer producer = new AccelerometerDataProducer(3,1200);
		new Thread(consumer).start();
		new Thread(producer).start();
	}
	
}
