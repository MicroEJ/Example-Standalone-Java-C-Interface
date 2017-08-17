/*
 *
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
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