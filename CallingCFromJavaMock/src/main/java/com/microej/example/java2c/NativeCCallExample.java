/*
 *
 * Java
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
package com.microej.example.java2c;

public class NativeCCallExample {

	// No visible constructor for Utility class
	private NativeCCallExample()
	{
	}

	public static int someCFunctionReturningTwiceAValue(int n)
	{
		return n*2;
	}

}