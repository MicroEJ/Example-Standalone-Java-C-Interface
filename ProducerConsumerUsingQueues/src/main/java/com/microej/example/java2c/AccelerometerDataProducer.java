/*
 *
 * Java
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
package com.microej.example.java2c;

import java.io.IOException;

import com.microej.example.nativequeue.api.QueueService;

import ej.bon.Immortals;


public class AccelerometerDataProducer implements Runnable{

	//Ensure no one else accesses this
	private final QueueService queueService;

	private final byte sensorID;
	private final int productionPeriodInMilliseconds;

	private final byte[] data;

	public AccelerometerDataProducer(QueueService queueService, int sensorID, int productionPeriodInMilliseconds) {
		this.queueService = queueService;
		this.sensorID = (byte)sensorID;
		this.productionPeriodInMilliseconds = productionPeriodInMilliseconds;

		//Make the byte [] immortal to access it from C functions.
		this.data = (byte[]) Immortals.setImmortal(new byte[AccelerometerData.ACCELEROMETER_DATA_SIZE]);
	}


	@Override
	public void run() {

		try {
			System.out.println("ItemSize 	: " + this.queueService.getItemSize() );
			System.out.println("ItemsCount	: " + this.queueService.getItemsCount() );
			System.out.println("MaxItems	: " + this.queueService.getMaxItems() );
		} catch (IOException e) {
			e.printStackTrace();
		}

		while(true)
		{
			//Use the QueueService to post data
			try {
				AccelerometerData accelerometerData = AccelerometerData.generateRandomData(sensorID);
				System.arraycopy(accelerometerData.toByteArray(), 0, this.data, 0, AccelerometerData.ACCELEROMETER_DATA_SIZE);
				this.queueService.write(this.data);
				System.out.println("+" + accelerometerData.toString());
			}
			catch ( IOException e)
			{
				e.printStackTrace();
			}
			try {
				Thread.sleep(productionPeriodInMilliseconds);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}
