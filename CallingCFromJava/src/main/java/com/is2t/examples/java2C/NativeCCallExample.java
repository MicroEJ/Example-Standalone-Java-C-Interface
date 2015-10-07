/**
 *
 * Java
 *
 * Copyright 2015 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.is2t.examples.java2C;

public class NativeCCallExample {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		final int aValue = 120;
		System.out.println("Calling : someCFunctionReturningTwiceAValue(" + aValue +")");
		final int resultOfCallToSomeCFunction = someCFunctionReturningTwiceAValue(aValue);		
		System.out.println("Result  : " + resultOfCallToSomeCFunction);
	}
	
	static native int someCFunctionReturningTwiceAValue(int n);

}