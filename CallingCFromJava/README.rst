.. Copyright 2016-2019 MicroEJ Corp. All rights reserved.
.. Use of this source code is governed by a BSD-style license that can be found with this software.

Overview
========

This example shows the minimum steps required to create a Java program that
makes a blocking call to a C function via the SNI interface, starting from an
empty MicroEJ workspace.

The following steps will be taken :

-  Building a Java Platform
-  Building the Java client code
-  Running the application on simulator
-  Running the application on target

Requirements
============

This example has been tested on :

- MicroEJ SDK 5.1.
- BSP specific toolchain (Keil MicroVision (™) v5)
- With a ST STM32F746G-DISCO board platform that contains :

  - EDC-1.2

Usage
=====

Adapting this example to your BSP toolchain
-------------------------------------------

This document shows the process for a STM32F746G-Disco (™) target using a Keil
MicroVision (™) toolchain. The BSP toolchain specific steps are marked *(BSP
specific)* Please refer to the relevant “Build a Java Platform” guide in the
“Getting started” section of MicroEJ Resource Center for indications on how to
adapt this process to your specific target.

Modules selection
-----------------

-  From the **Content** tab of the generated example.platform file

   -  In the **modules** frame

      -  Check the **Java to C Interface / SNI API** option in the modules
         checkbox tree

Running the application on simulator
------------------------------------

Using MicroEJ, you may deploy and run your application on an embedded target
(if the hardware and related BSP are available) or you may run your application
on a Java simulator mimicking the behavior of your embedded target.

Building for the simulator
--------------------------

Here, we will launch a MicroEJ “Run Configuration” that will compile the Java
code from the CallingCFromJava project for the JPF we created previously

-  Select **Run > Run Configurations…** menu item
-  Select **MicroEJ Application** group
-  Select the `NativeCCallExample_Sim_Build.launch
   <launches/NativeCCallExample_Sim_Build.launch>`__ configuration

   -  In **Execution** tab

      -  In **Target** frame

         -  Click the **Browse** button next to the **Platform** Field and
            select your platform

      -  In **Execution** frame

         -  Notice that “Execute on Simulator” radio button option is checked

   -  Click on “Run”

Getting a java.lang.UnsatisfiedLinkError exception
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The result of the previous step shall lead to this error message

::

       =============== [ Initialization Stage ] ===============
       =============== [ Launching Simulator ] ===============
       Calling : someCFunctionReturningTwiceAValue(120)
       Exception in thread "main" java.lang.UnsatisfiedLinkError: No HIL client implementor found (timeout)
           at java.lang.Throwable.fillInStackTrace(Throwable.java:79)
           at java.lang.Throwable.<init>(Throwable.java:30)
           at java.lang.Error.<init>(Error.java:10)
           at java.lang.LinkageError.<init>(LinkageError.java:10)
           at java.lang.UnsatisfiedLinkError.<init>(UnsatisfiedLinkError.java:10)
           at com.microej.example.java2c.NativeCCallExample.main(NativeCCallExample.java:19)
           at java.lang.MainThread.run(Thread.java:836)
           at java.lang.Thread.runWrapper(Thread.java:372)
       =============== [ Completed Successfully ] ===============

       SUCCESS

This is perfectly normal since in `NativeCCallExample.java
<src/main/java/com/microej/example/java2c/NativeCCallExample.java>`__ we
declared **someCFunctionReturningTwiceAValue** as a native function, when
running the simulator, the Hardware In the Loop (HIL) engines expects to find
some Java implementation emulating the behavior of the native function.

Adding a mock of the native function to the JPF
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Since our application relies on native C functions, on an embedded target, we
would need to provide a C implementation. But given that we are running it on
the simulator, we can emulate those functions using a mock.

The `CallingCFromJavaMock <../CallingCFromJavaMock>`__ project provides the
mocks required for running the Java application on simulator (see
`/CallingCFromJavaMock/…/NativeCCallExample.java
<../CallingCFromJavaMock/src/main/java/com/microej/example/java2c/NativeCCallExample.java>`__).

Note that the mock method in `/CallingCFromJavaMock/…/NativeCCallExample.java
<../CallingCFromJavaMock/src/main/java/com/microej/example/java2c/NativeCCallExample.java>`__
has the same fully qualified name (package-name.class-name.method-name) as the
one declaring the native in `…/NativeCCallExample.java
<src/main/java/com/microej/example/java2c/NativeCCallExample.java>`__.
This allows the linker to find which method simulates the native function.

-  Open the `CallingCFromJavaMock.jardesc
   <../CallingCFromJavaMock/CallingCFromJavaMock.jardesc>`__ jar description
   file

-  Update the export destination file so that it has the following value
   ``STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-1.0.0\source\mocks\dropins\CallingCFromJavaMock.jar``

-  Click on **Finish**

-  Retry running the `NativeCCallExample_Sim_Build.launch
   <launches/NativeCCallExample_Sim_Build.launch>`__ configuration.

   ::

        =============== [ Initialization Stage ] ===============
        =============== [ Launching Simulator ] ===============
        Calling : someCFunctionReturningTwiceAValue(120)
        Result  : 240
        =============== [ Completed Successfully ] ===============

        SUCCESS

Running the application on target
---------------------------------

Building for the target
~~~~~~~~~~~~~~~~~~~~~~~

Here, we will launch a MicroEJ “Run Configuration” that will compile the Java
code from the CallingCFromJava project for the platform we created previously

-  Select **Run > Run Configurations…** menu item
-  Select **MicroEJ Application** group
-  Select the `NativeCCallExample_Emb_Build
   <launches/NativeCCallExample_Emb_Build.launch>`__ configuration

   -  In **Execution** tab

      -  In **Target** frame

         -  Click the **Browse** button next to the **Platform** Field and
            select your platform

      -  In **Execution** frame

         -  Notice that “Execute on Device” radio button option is checked

   -  Click on “Run”

Opening the generated C project (BSP specific)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

-  From the **Project Explorer** view

   -  Navigate to the
      `STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-bsp/Projects/STM32746G-Discovery/Applications/MicroEJ/MDK-ARM
      <../STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-bsp/Projects/STM32746G-Discovery/Applications/MicroEJ/MDK-ARM>`__
      folder
   -  Double-click on the `Project.uvprojx
      <../STM32F746GDISCO-JavaCInterface-CM7hardfp_ARMCC5-bsp/Projects/STM32746G-Discovery/Applications/MicroEJ/MDK-ARM/Project.uvprojx>`__
      file (this will open the BSP project in the MicroVision IDE)

Getting a linker error (BSP specific)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

-  From the MicroVision IDE

   -  Select **Project > Build Target** menu item (or press F7 keyboard
      shortcut)

   -  A linker error message shall appear :

      ``.\standalone\standalone.axf: Error: L6218E: Undefined symbol
      Java_com_microej_example_java2C_NativeCCallExample_someCFunctionReturningTwiceAValue
      (referred from microejapp.o).``

This is perfectly normal since in `NativeCCallExample.java
<src/main/java/com/microej/example/java2C/NativeCCallExample.java>`__ we
declared **someCFunctionReturningTwiceAValue** as a native function, when
building the MicroEJ project, the generated linker configuration file expects
to find a C function definition matching the qualified name of the function.

Fixing the linker error
-----------------------

C Native function implementation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

-  Select **File > New > Source Folder** menu item

   -  Set the **Folder Name** field to “src/main/c”

-  Notice the C function in `NativeCCallExample.c
   <src/main/c/com/microej/example/java2c/NativeCCallExample.c>`__ follows the
   strict SNI naming convention mentioned earlier.

   ::

         #include <sni.h>
         jint Java_com_microej_example_java2C_NativeCCallExample_someCFunctionReturningTwiceAValue(jint aValue) {
            return aValue*2;
         }

-  Right-click on the file
   -  Select **Properties** context menu item

      -  Copy the value of the **Resource > Location** field into the clipboard

Adding the C file to the BSP IDE project structure (BSP specific)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-  Select the root node of your project

   -  Right-Click and select **Add Group** this will add a group called “New
      Group”
   -  Select this group and hit **F2** key so as to rename it to “JavaNatives”
   -  Right-Click on the **JavaNatives** group and select
      **Add Existing Filesto group ‘JavaNatives’…**
   -  Navigate to the `NativeCCallExample.c
      <src/main/c/com/microej/example/java2C/NativeCCallExample.c>`__ file
      (its location is in the clipboard
      ..\..\..\..\CallingCFromJava\src\main\c\com\microej\example\java2c\NativeCCallExample.c)
   -  Click **Add**
   -  Click **Close**

Getting a clean link (BSP specific)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-  Select **Project > Build Target** menu item (or press F7 keyboard shortcut)

   ::

        *** Using Compiler 'V5.05 update 2 (build 169)', folder: 'C:\Keil_v5\ARM\ARMCC\Bin'
        Build target 'STM32F749G-DISCO'
        compiling NativeCCallExample.c...
        linking...
        Program Size: Code=58744 RO-data=2844 RW-data=288 ZI-data=52912
        FromELF: creating hex file...
        Finished: 0 information, 1 warning and 0 error messages.
        ".\standalone\standalone.axf" - 0 Error(s), 0 Warning(s).
        Build Time Elapsed:  00:00:03

Flashing the board (BSP specific)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

-  Connect your board
-  Select **Flash > Download** menu item (or press F8 keyboard shortcut).

Checking the behavior
~~~~~~~~~~~~~~~~~~~~~

-  Set up a terminal on the board serial port and press the reset input.
   You shall get the following output :

   ::

        VM START
        Calling : someCFunctionReturningTwiceAValue(120)
        Result  : 240
        VM END (exit code = 0)

Additional references
=====================

Asynchronous communication use case
-----------------------------------

The Use Case shown in this document only covers a most basic usage of the Java
to C interface feature provided by SNI. The native C function is invoked
synchronously, meaning the thread of the calling Java method is suspended
until the C function returns. Shall you need a more elaborate example of how
to use SNI in an asynchronous context, you may refer to the
`ProducerConsumerUsingQueues <../ProducerConsumerUsingQueues>`__ example
project.

Dependencies
============

*All dependencies are retrieved transitively by Ivy resolver*.

Source
======

N/A

Restrictions
============

None.
