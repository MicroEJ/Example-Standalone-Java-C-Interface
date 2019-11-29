.. Copyright 2016-2019 MicroEJ Corp. All rights reserved.
.. Use of this source code is governed by a BSD-style license that can be found with this software.

Overview
========

This helper project contains a Java class and its C struct counterpart meant to hold the data to be exchanged between Java threads and C tasks.

Requirements
============

These examples have been tested on :

- MicroEJ SDK 5.1
- BSP specific toolchain (Keil MicroVision (™) v5)
- With a ST STM32F746G-DISCO board platform that contains :

  - EDC-1.2

Usage
=====

Creating Java objects from bytes retrieved from a C native function call
------------------------------------------------------------------------

Whatever the data kind, the Java constructor takes a byte array as an argument so as to easily convert raw data retrieved from either `ShieldedPlug <../ProducerConsumerUsingShieldedPlug>`__ or `SNI <../ProducerConsumerUsingQueues>`__ to a regular Java object

Dependencies
============

*All dependencies are retrieved transitively by Ivy resolver*.

Source
======

See the following files :

-  `AccelerometerData.java <src/main/java/com/microej/example/java2c/AccelerometerData.java>`__
-  `accelerometer-data.h <src/main/c/accelerometer-data.h>`__
-  `accelerometer-data.c <src/main/c/accelerometer-data.c>`__

Note that the **sensorID** field in addition to the actual sensor data ({x,y,z} triplet) is useful for knowing from which sensor the data comes from

Restrictions
============

None.
