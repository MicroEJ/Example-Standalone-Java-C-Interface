[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](For demonstration purpose only.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This helper project contains a Java class and its C struct counterpart meant to hold the data to be exchanged between Java threads and C tasks.

# Usage

## Creating Java objects from bytes retrieved from a C native function call

Whatever the data kind, the Java constructor takes a byte array as an argument so as to easily convert raw data retrieved from either [ShieldedPlug](/ProducerConsumerUsingShieldedPlug) or [SNI](/ProducerConsumerUsingQueues) to a regular Java object

# Requirements

* JRE 7 (or later) x86.
* MicroEJ SDK 4.0 or later.
* BSP specific toolchain (Keil MicroVision (&trade;) v5 or later.
* A platform with at least:
	* EDC-1.2.0 or higher
	
# Dependencies

_All dependencies are retrieved transitively by Ivy resolver_.

# Source
See the following files :

* [AccelerometerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/AccelerometerData.java)
* [accelerometer-data.h](/ProducerConsumerData/src/main/c/accelerometer-data.h)
* [accelerometer-data.c](/ProducerConsumerData/src/main/c/accelerometer-data.c)

Note that the **sensorID** field in addition to the actual sensor data ({x,y,z} triplet) is useful for knowing from which sensor the data comes from

# Restrictions
None.