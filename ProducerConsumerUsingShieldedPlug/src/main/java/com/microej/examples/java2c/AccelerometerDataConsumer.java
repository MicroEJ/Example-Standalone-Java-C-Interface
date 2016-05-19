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

import ej.sp.EmptyBlockException;
import ej.sp.ShieldedPlug;

public class AccelerometerDataConsumer implements Runnable{

	private final int accelerometerID;

	public AccelerometerDataConsumer(int accelerometerID) {
		super();
		this.accelerometerID = accelerometerID;
	}


	@Override
	public void run() {

		ShieldedPlug database = ShieldedPlug.getDatabase(ProducerConsumerExample.DATABASE_ID);
		database.setReader(accelerometerID, new AccelerometerDataUnmarshaller());

		while (true) {
			try {
				// wait for a change on accelerometer data
				database.waitFor(accelerometerID);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			try {
				// read the AccelerometerData
				AccelerometerData accelerometerData = (AccelerometerData) database.readObject(accelerometerID);
				System.out.println("-" + accelerometerData.toString());
			} catch (EmptyBlockException e) {
				e.printStackTrace();
			}

		}
	}

}
