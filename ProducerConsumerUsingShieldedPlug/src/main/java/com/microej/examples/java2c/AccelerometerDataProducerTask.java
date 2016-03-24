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

import java.util.TimerTask;

import com.is2t.sp.ShieldedPlug;

public class AccelerometerDataProducerTask extends TimerTask {

	private final byte sensorID;
	private final ShieldedPlug database;

	public AccelerometerDataProducerTask(int sensorID) {
		super();
		this.sensorID = (byte)sensorID;
		database = ShieldedPlug.getDatabase(ProducerConsumerExample.DATABASE_ID);
	}

	@Override
	public void run() {
		AccelerometerData accelerometerData = AccelerometerData.generateRandomData(sensorID);
		database.write(sensorID, accelerometerData.toByteArray());
		System.out.println("+" + accelerometerData.toString());
	}

}
