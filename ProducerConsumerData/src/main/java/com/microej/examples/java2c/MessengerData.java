/**
 *
 * Java
 *
 * Copyright 2015 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

public class MessengerData {


	public static final int MESSENGER_MAX_DATA_SIZE = 255;

	byte [] data = new byte [MESSENGER_MAX_DATA_SIZE];

	public MessengerData(byte[] data) {
		this.data = data;
	}

	
	@Override
	public String toString() {
		return new String(data);
	}

	//ShieldedPlug related IDs - see src/main/resources/database-definition.xml file for relevant values
	public static final int DATABASE_ID = 1;
	public static final int DATABASE_FIELD_ID_MESSAGE = 0;

}
