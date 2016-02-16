/*
 *
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

public class MessengerData {

	public static final int MESSENGER_MAX_DATA_SIZE = 255;

	private byte senderId;
	private byte contentLength;
	private byte [] content = new byte [MESSENGER_MAX_DATA_SIZE];
	
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
		StringBuilder stringBuilder = new StringBuilder();
		stringBuilder.append("ID : ");
		stringBuilder.append(senderId);
		stringBuilder.append(" {");
		stringBuilder.append(new String(this.content));
		stringBuilder.append("}");
		return stringBuilder.toString();
	}

	//SNI related native - retrieves pointer to queue
	native public static int getQueuePtr();

}
