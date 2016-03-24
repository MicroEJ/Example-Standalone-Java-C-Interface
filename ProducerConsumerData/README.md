[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](For demonstration purpose only.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This helper project contains two Java classes and their C struct counterparts meant to hold the data to be exchanged between Java threads and C tasks.

# Data kinds
For fixed size data, see the following files :

* [AccelerometerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/AccelerometerData.java)
* [accelerometer-data.h](/ProducerConsumerData/src/main/c/accelerometer-data.h)
* [accelerometer-data.c](/ProducerConsumerData/src/main/c/accelerometer-data.c)

Note that the **sensorID** field in addition to the actual sensor data ({x,y,z} triplet) is useful for knowing from which sensor the data comes from
 
For variable size data, see the following files :

* [MessengerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/MessengerData.java)
* [messenger-data.h](/ProducerConsumerData/src/main/c/messenger-data.h)
* [messenger-data.c](/ProducerConsumerData/src/main/c/messenger-data.c)

Note that the **senderID** field in addition to the actual message data is useful for knowing who sent the data

# Creating Java objects from bytes retrieved from a C native function call

Whatever the data kind, the Java constructor takes a byte array as an argument so as to easily convert raw data retrieved from either [ShieldedPlug](/ProducerConsumerUsingShieldedPlug) or [SNI](/ProducerConsumerUsingSNIAndImmortals) to a regular Java object