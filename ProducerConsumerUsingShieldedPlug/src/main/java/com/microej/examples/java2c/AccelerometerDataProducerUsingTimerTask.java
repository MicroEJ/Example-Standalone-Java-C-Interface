/**
 *
 * Java
 *
 * Copyright 2015 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

import com.is2t.sp.ShieldedPlug;
import com.microej.examples.java2c.AccelerometerData;

import java.util.Timer;
import java.util.TimerTask;

public class AccelerometerDataProducerUsingTimerTask {

	private static Timer producerTimer = new Timer("AccelerometerDataProducerTimer");
	
	private byte sensorID;
	private long productionPeriodInMilliseconds;
	private ShieldedPlug database; 
	
	public AccelerometerDataProducerUsingTimerTask(int sensorID,long productionPeriodInMilliseconds) {
		super();
		this.sensorID = (byte)sensorID;
		this.productionPeriodInMilliseconds = productionPeriodInMilliseconds;
		database = ShieldedPlug.getDatabase(AccelerometerData.DATABASE_ID);
	}

	public void start() {
		producerTimer.scheduleAtFixedRate(new AccelerometerDataProducerTask(), 0, productionPeriodInMilliseconds);
	}

	private class AccelerometerDataProducerTask extends TimerTask {

		@Override
		public void run() {
			AccelerometerData accelerometerData = AccelerometerData.generateRandomData(sensorID);
			database.write(AccelerometerData.DATABASE_FIELD_ID_ACCELEROMETER, accelerometerData.toByteArray());
			System.out.println("+" + accelerometerData.toString());
		}		
	}

}
