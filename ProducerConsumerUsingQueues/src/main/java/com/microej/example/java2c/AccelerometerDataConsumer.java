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


public class AccelerometerDataConsumer implements Runnable{

	//Ensure no one else accesses this
	private final QueueService queueService;
	private final byte[] data;


	public AccelerometerDataConsumer(QueueService queueService) {
		this.queueService = queueService;

		//Make the byte [] immortal to access it from C functions.
		this.data = (byte[]) Immortals.setImmortal(new byte[AccelerometerData.ACCELEROMETER_DATA_SIZE]);
	}


	@Override
	public void run() {
		for (;;)
		{
			//Use the QueueService to retrieve data
			try {
				this.queueService.read(this.data);
				AccelerometerData dataWrapper = new AccelerometerData(data);
				System.out.println("-" + dataWrapper.toString());
			}
			catch ( IOException e)
			{
				e.printStackTrace();
			}
		}
	}

}
