package com.is2t.examples.java2C;

public class DataConsumerExample {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		while (true) {
			System.out.println("-- main alive --");
			try {
				Thread.sleep(2000);
			} catch (InterruptedException e) {
				// It is OK to ignore because this is just a basic example
			}
		}
		//System.out.println("-- main DONE --");
	}
	
}
