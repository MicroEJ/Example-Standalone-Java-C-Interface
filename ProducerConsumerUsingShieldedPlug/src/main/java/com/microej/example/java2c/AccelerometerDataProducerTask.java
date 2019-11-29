/*
 *
 * Java
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
package com.microej.example.java2c;

import java.util.TimerTask;

import ej.sp.ShieldedPlug;

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
