/*
 *
 * Java
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
package com.microej.example.java2c;

import java.util.Timer;


public class ProducerConsumerExample {

	//ShieldedPlug related IDs - see src/main/resources/database-definition.xml file for relevant values
	public static final int DATABASE_ID = 0;
	public static final int DATABASE_FIELD_ID_ACCELEROMETER_1 = 0;
	public static final int DATABASE_FIELD_ID_ACCELEROMETER_2 = 1;
	public static final int DATABASE_FIELD_ID_ACCELEROMETER_3 = 2;

	// No visible constructor for Utility class
	private ProducerConsumerExample()
	{
	}

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
