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

	// No visible constructor for utility class
	private NativeCCallExample() {
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		final int aValue = 120;
		System.out.println("Calling : someCFunctionReturningTwiceAValue(" + aValue +")");
		final int resultOfCallToSomeCFunction = someCFunctionReturningTwiceAValue(aValue);
		System.out.println("Result  : " + resultOfCallToSomeCFunction);
	}

	public static native int someCFunctionReturningTwiceAValue(int n);

}