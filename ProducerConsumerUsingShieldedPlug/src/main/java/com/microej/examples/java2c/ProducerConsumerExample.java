/*
 *
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

import java.util.Timer;


public class ProducerConsumerExample {

	//ShieldedPlug related IDs - see src/main/resources/database-definition.xml file for relevant values
	public static final int DATABASE_ID = 0;
	public static final int DATABASE_FIELD_ID_ACCELEROMETER = 0;

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		AccelerometerDataConsumer consumer = new AccelerometerDataConsumer();
		new Thread(consumer).start();

		Timer producerTimer = new Timer("AccelerometerDataProducerTimer");
		AccelerometerDataProducerTask producerTask = new AccelerometerDataProducerTask(4);
		producerTimer.scheduleAtFixedRate(producerTask, 0, 1200);

	}

}
