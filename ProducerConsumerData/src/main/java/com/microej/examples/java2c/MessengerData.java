/**
 *
 * Java
 *
 * Copyright 2015 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

import java.lang.Byte;

public class MessengerData {

	public static final int MESSENGER_MAX_DATA_SIZE = 255;

	byte senderId;
	byte contentLength;
	byte [] content = new byte [MESSENGER_MAX_DATA_SIZE];
	
	public MessengerData(byte[] data) {
		this.senderId = data[0];
		this.contentLength = data[1];
		if ( this.contentLength > 0 )
		{
			//Arrays.copyOfRange is not readily available in the stripped down java libraries that come with MicroEJ
			//Note however that one can use the collectionsutil EClassPath project available from developer.microej.com to augment the class path with an implementation of the Arrays util class
			System.arraycopy(data, 2, this.content, 0, this.contentLength);	
		}
	}

	
	@Override
	public String toString() {
		return "ID : " + senderId + " {" + new String(this.content) + "}";
	}

	//ShieldedPlug related IDs - see src/main/resources/database-definition.xml file for relevant values
	public static final int DATABASE_ID = 1;
	public static final int DATABASE_FIELD_ID_MESSAGE = 0;

}
