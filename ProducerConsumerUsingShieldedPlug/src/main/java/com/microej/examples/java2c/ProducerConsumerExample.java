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
	public static final int DATABASE_FIELD_ID_ACCELEROMETER_1 = 0;
	public static final int DATABASE_FIELD_ID_ACCELEROMETER_2 = 1;
	public static final int DATABASE_FIELD_ID_ACCELEROMETER_3 = 2;

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		AccelerometerDataConsumer consumer1 = new AccelerometerDataConsumer(DATABASE_FIELD_ID_ACCELEROMETER_1);
		new Thread(consumer1).start();
		AccelerometerDataConsumer consumer2 = new AccelerometerDataConsumer(DATABASE_FIELD_ID_ACCELEROMETER_2);
		new Thread(consumer2).start();
		AccelerometerDataConsumer consumer3 = new AccelerometerDataConsumer(DATABASE_FIELD_ID_ACCELEROMETER_3);
		new Thread(consumer3).start();

		Timer producerTimer = new Timer("AccelerometerDataProducerTimer");
		AccelerometerDataProducerTask producerTask = new AccelerometerDataProducerTask(DATABASE_FIELD_ID_ACCELEROMETER_3);
		producerTimer.scheduleAtFixedRate(producerTask, 0, 1200);

	}

}
