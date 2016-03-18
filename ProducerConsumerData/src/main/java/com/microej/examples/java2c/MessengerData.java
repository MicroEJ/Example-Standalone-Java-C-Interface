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

	private final byte senderId;
	private final byte contentLength;
	private final byte [] content = new byte [MESSENGER_MAX_DATA_SIZE];

	public MessengerData(byte[] data) {
		this.senderId = data[0];
		this.contentLength = data[1];
		if ( this.contentLength > 0 )
		{
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

}
