# Overview

This project implements a wrapper service around OS Queue API.

It currently supports FreeRTOS only.

## Project Setup

The project is made up of java and C source files. Both need to be referenced from your application.

### Java sources

Add this project to your project build Path :

* Right-Click on your project and select **Properties** from the context menu
* Select **Java Build Path**
* Select the **Projects** tab
* Click on **Add** Button
* Select **NativeQueueWrapper**
* Click on **Ok**
* Click on **Ok** 

### C sources

* The files in src/main/c shall be added to your BSP project.

## API Usage

Refer to the [AccelerometerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataConsumer.java) source file for sample API usage 