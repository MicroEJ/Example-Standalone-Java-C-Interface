.. Copyright 2016-2019 MicroEJ Corp. All rights reserved.
.. Use of this source code is governed by a BSD-style license that can be found with this software.

Overview
========

Although MicroEJ lets you leverage Java in an embedded context, there are still some cases where your Java threads need to synchronize and exchange data with OS tasks written in C/C++.

This repository contains examples showing how to enable communication between programs written in C and programs written in Java using MicroEJ APIs.

Requirements
============

These examples have been tested on :

- MicroEJ SDK 5.1
- BSP specific toolchain (Keil MicroVision (™) v5)
- With a ST STM32F746G-DISCO board platform that contains :

  - EDC-1.2

Available examples
==================

For calling C functions from a Java class, MicroEJ provides the SNI API.
The following examples illustrate how to use this API :

- `CallingCFromJava <CallingCFromJava>`__ provides an example of invocation of a C function from a Java method
- `CallingCFromJavaMock <CallingCFromJavaMock>`__ is a helper project required in order to be able to run the Java application on simulator

The SNI library also provides APIs for tasks/threads synchronisation via function calls and callbacks.

For data exchange issues between tasks/threads, MicroEJ provides a few options :

- Shielded Plug API (suitable for sharing structured data, with built-in wait/release synchronisation APIs)
- Immortals API and SNI API (suitable for sharing access to raw data, but you need to provide your own synchronisation mechanism)

The following examples, based on the consumer/producer problem, illustrate how to use those APIs :

-  `Producer Consumer with ShieldedPlug <ProducerConsumerUsingShieldedPlug>`__ using Shielded Plug API as the delivery mechanism
-  `Producer Consumer with SNI and Immortals <ProducerConsumerUsingQueues>`__ using SNI and Immortals APIs to wrap the native FreeRTOS message queue API as the underlying delivery mechanism

Each example comes with a launch configuration. These launch configurations require that the target platform (common to each configuration) is built.

Building the Java Platform (JPF)
================================

We need to generate and build a Java Platform (JPF), i.e. the set of object files and libraries required to execute Java code on the target platform linked by an executable C program.

The following assumes that you have downloaded and imported the STM32F746G-DISCO platform from `MicroEJ SDK Getting Started <http://developer.microej.com/getting-started-sdk-stm-5.html>`__ web page.

Creation
--------

-  Select **File > New > Other… > MicroEJ > Platform** menu item

   -  Click **Browse** button next to the **Architecture** field and select **ARM Cortex-M7 ARMCC**
   -  Check the **Create from a platform reference implementation** option

      -  Select **STM32F746G-Disco > MultiApp FreeRTOS**

   -  Click **Next**

      -  Set the name field to “**JavaCInterface**”
      -  Set the version field to “**1.0.0**”

   -  Click **Finish**. This will lead to the generation of a few projects in our workspace :

      -  `STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-bsp <STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-bsp>`__
      -  `STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-configuration <STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-configuration>`__
      -  `STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-fp <STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-fp>`__

Build
-----

-  From the **Overview** tab of the generated `STM32F746GDISCO.platform <STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-configuration/STM32F746GDISCO.platform>`__ file

   -  Click on the **Build platform** hyperlink. This will generate the following project :

      -  `STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-1.0.0 <STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-1.0.0>`__

API Selection Criteria
======================

Here are a few things to consider when choosing between Shielded Plug and an SNI and Immortals based solution

-  Performance

   -  Generally speaking, Shielded Plug (SP for short) will copy data from/to a database block and will therefore take more processing time and memory than an SNI-based solution relying on a shared memory area with less data copying.
   -  SP is more suitable for asynchronous (post) processing of published data

-  Data Integrity

   -  Given that with SP there is no buffering of data, it makes it more suitable for sampling scenarios where losing a value once in a while is not critical .

-  API convenience

   -  SP requires that you describe your data in an xml based configuration file.
   -  However, the SP synchronization API is quite simple
   -  Using SNI and Immortals, it is easier to pass data directly to/from C native functions

In summary, for simple use cases, Shielded Plug shall suffice, but for more intensive and more constrained environments, SNI and Immortals may be a better fit.

Additional references
---------------------

B-ON Library Specification
~~~~~~~~~~~~~~~~~~~~~~~~~~

Among other things, the **Beyond Profile Specification** document contains detailed information on:

-  sharing memory areas between Java and C/C++ using **immortal objects**

**immortal objects** : such objects are never garbage collected and keep the same physical memory location forever. This makes them useful for specifying shared memory areas between C and Java.

The document can be downloaded from `www.e-s-r.net/specifications/index.html <http://www.e-s-r.net/specifications/index.html>`__

SNI Library Specification
~~~~~~~~~~~~~~~~~~~~~~~~~

The **Simple Native Interface for GreenThread Context Profile Specification** document contains detailed information on :

-  how to map Java primitive types to portable C types for methods/functions parameters and return types
-  naming conventions for C functions implementing Java SNI native implementations

   -  these have to be followed to generate valid linker configuration files

-  sharing memory areas between Java and C/C++ using **immortal objects**
-  controlling (as in suspend and resume) Java Thread execution from C code

The document can be downloaded from `www.e-s-r.net/specifications/index.html <http://www.e-s-r.net/specifications/index.html>`__

Shielded Plug Library Specification
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **Shielded Plug Profile Specification** document contains detailed information on :

-  the concepts underlying the Shielded Plug API
-  how to map Java primitive types to portable C types for methods/functions parameters and return types
-  how to create dedicated readers/writers that can translate Shielded Plug Blocks into Java Objects
-  the reference API for both C and Java languages

The document can be downloaded from `www.e-s-r.net/specifications/index.html <http://www.e-s-r.net/specifications/index.html>`__

Simulation
~~~~~~~~~~

The **Simulation** section of the **Device Developer’s Guide** document contains useful info for :

-  creating Java mocks simulating native code using the same naming conventions as SNI
-  using the Hardware In the Loop (HIL) engine as a replacement for the SNI/GreenThread interface used on target environments

To access the document from MicroEJ:

-  Select **Help > MicroEJ Resource Center** menu item

   -  Select **Manual > Device Developer’s Guide**