[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](For demonstration purpose only.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This example shows the minimum steps required to create a Java program that makes a blocking call to a C function via the SNI interface, starting from an empty MicroEJ workspace.

The following steps will be taken :

* Building a Java Platform
* Building the Java client code
* Running the application on simulator
* Running the application on target

# Usage

## Requirements
* JRE 7 (or later) x86.
* MicroEJ 3.1 or later.
* BSP specific toolchain (Keil MicroVision (&trade;) v5 or later with v4 legacy pack)

## Adapting this example to your BSP toolchain

This document shows the process for a STM32F429I-Disco (&trade;) target using a Keil MicroVision (&trade;) toolchain.
The BSP toolchain specific steps are marked *(BSP specific)*
Please refer to the relevant "Build a Java Platform" guide in the "Getting started" section of MicroEJ Resource Center for indications on how to adapt this process to your specific target.


# Building a Java Platform

We need to generate and build a Java Platform (JPF), i.e. the set of object files and libraries required to execute Java code on the target platform linked by an executable C program.

## Creation (BSP Specific)

* Select **File > New > Java Platform** menu item
	* Click **Browse** button next to the **Java Platform Architecture** field and select **ARM Cortex-M4 ARMCC**
	* Check the **Create a platform from an example or a template.** option
		* Select **STM32F429I-Disco > Basic**
	* Click **Next**
		* Specify a name such as "**CallingCFromJava**" in the relevant field
	* Click Finish. This will lead to the generation of a few projects in our workspace :
		* [CallingCFromJava-bsp](CallingCFromJava-bsp)
		* [CallingCFromJava-configuration](CallingCFromJava-configuration)
		* [CallingCFromJava-javaExample](CallingCFromJava-javaExample)
	* Delete the [CallingCFromJava-javaExample](CallingCFromJava-javaExample) project

## Modules selection
* From the **Content** tab of the generated example.platform file
	* In the **modules** frame
		* Click on the **Show/Hide modules** button
		* Select the **Simple Native Interface API** option in the modules checkbox tree

## Build
* From the **Overview** tab of the generated example.platform file
	* Click on the **Build platform** hyperlink. This will generate the following project :
		* [CallingCFromJava-1.0.0](CallingCFromJava-1.0.0)
* Click on **Window > Preferences > MicroEJ**
	* In the **MicroEJ Repository** frame
		* Click on the **Refresh** button 

## Running the application on simulator
Using MicroEJ, you may deploy and run your application on an embedded target (if the hardware and related BSP are available) or you may run your application on a Java simulator mimicking the behavior of your embedded target.

## Building for the simulator
Here, we will launch a MicroEJ "Run Configuration" that will compile the Java code from the CallingCFromJava project for the JPF we created previously

* Select **Run > Run Configurations...** menu item
* Select **MicroEJ Application** group
* Select the [NativeCCallExample_Sim_Build.launch](/CallingCFromJava/launches/NativeCCallExample_Sim_Build.launch) configuration
	* In **Execution** tab
		* In **Target** frame
			* Click the **Browse** button next to the JPF Field and select your platform
		* In **Execution** frame
			* Notice that "Execute on SimJPF" radio button option is checked
	* Click on "Run"

### Getting a java.lang.UnsatisfiedLinkError exception

The result of the previous step shall lead to this error message
	
		=============== [ Initialization Stage ] ===============
		=============== [ Launching SimJPF ] ===============
		Calling : someCFunctionReturningTwiceAValue(120)
		Exception in thread "main" java.lang.UnsatisfiedLinkError: No HIL client implementor found (timeout)
			at java.lang.Throwable.fillInStackTrace(Throwable.java:79)
			at java.lang.Throwable.<init>(Throwable.java:30)
			at java.lang.Error.<init>(Error.java:10)
			at java.lang.LinkageError.<init>(LinkageError.java:10)
			at java.lang.UnsatisfiedLinkError.<init>(UnsatisfiedLinkError.java:10)
			at com.microej.examples.java2c.NativeCCallExample.main(NativeCCallExample.java:19)
			at java.lang.MainThread.run(Thread.java:836)
			at java.lang.Thread.runWrapper(Thread.java:372)
		=============== [ Completed Successfully ] ===============
		
		SUCCESS

This is perfectly normal since in [NativeCCallExample.java](CallingCFromJava/src/main/java/com/microej/examples/java2c/NativeCCallExample.java) we declared **someCFunctionReturningTwiceAValue** as a native function, when running the simulator, the Hardware In the Loop (HIL) engines expects to find some Java implementation emulating the behavior of the native function. 

### Adding a mock of the native function to the JPF
Since our Java application relies on native C functions, on an embedded target, we would need to provide a C implementation. But given that we are running it on a Java simulator, we can emulate those functions using a Java mock.

The [CallingCFromJavaMock](CallingCFromJavaMock) project provides the mocks required for running the Java application on simulator (see [/CallingCFromJavaMock/.../NativeCCallExample.java](/CallingCFromJavaMock/src/main/java/com/microej/examples/java2c/NativeCCallExample.java)).

Note that the mock method in [/CallingCFromJavaMock/.../NativeCCallExample.java](/CallingCFromJavaMock/src/main/java/com/microej/example/java2c/NativeCCallExample.java) has the same fully qualified name (package-name.class-name.method-name) as the one declaring the native in [/.../NativeCCallExample.java](/CallingCFromJava/src/main/java/com/microej/example/java2c/NativeCCallExample.java). This allows the linker to find which method simulates the native function.

* Open the [CallingCFromJavaMock.jardesc](/CallingCFromJavaMock/CallingCFromJavaMock.jardesc) jar description file
* Update the export destination file so that it has the following value
	`CallingCFromJava-1.0.0\source\mocks\dropins\CallingCFromJavaMock.jar`
* Click on **Finish**
* Retry running the [NativeCCallExample_Sim_Build.launch](/CallingCFromJava/launches/NativeCCallExample_Sim_Build.launch) configuration.

		=============== [ Initialization Stage ] ===============
		=============== [ Launching SimJPF ] ===============
		Calling : someCFunctionReturningTwiceAValue(120)
		Result  : 240
		=============== [ Completed Successfully ] ===============
		
		SUCCESS


## Running the application on target

### Building for the target
Here, we will launch a MicroEJ "Run Configuration" that will compile the Java code from the CallingCFromJava project for the JPF we created previously

* Select **Run > Run Configurations...** menu item
* Select **MicroEJ Application** group
* Select the [NativeCCallExample_Emb_Build](/CallingCFromJava/launches/NativeCCallExample_Emb_Build.launch) configuration
	* In **Execution** tab
		* In **Target** frame
			* Click the **Browse** button next to the JPF Field and select your platform
		* In **Execution** frame
			* Notice that "Execute on EmbJPF" radio button option is checked
	* Click on "Run"

### Opening the generated C project (BSP specific)
* From the **Project Explorer** view
	* Navigate to the [CallingCFromJava-bsp/Project/MicroEJ/MDK-ARM](CallingCFromJava-bsp/Project/MicroEJ/MDK-ARM) folder
	* Double-click on the [Project.uvproj](CallingCFromJava-bsp/Project/MicroEJ/MDK-ARM/Project.uvproj) file (this will open the BSP project in the MicroVision IDE)

### Getting a linker error (BSP specific)
* From the MicroVision IDE
	* Select **Project > Build Target** menu item (or press F7 keyboard shortcut)
	* A linker error message shall appear :

		```.\STM32F429i-DISCO\STM32429I-DISCO.axf: Error: L6218E: Undefined symbol Java_com_microej_examples_java2C_NativeCCallExample_someCFunctionReturningTwiceAValue (referred from javaapp.o).```

This is perfectly normal since in [NativeCCallExample.java](CallingCFromJava/src/main/java/com/microej/examples/java2C/NativeCCallExample.java) we declared **someCFunctionReturningTwiceAValue** as a native function, when building the MicroEJ project, the generated linker configuration file expects to find a C function definition matching the qualified name of the function. 


## Fixing the linker error
### C Native function implementation
* Select **File > New > Source Folder** menu item
	* Set the **Folder Name** field to "src/main/c"
* Right-Click on the folder that you just created
	* Select **New > File** context menu item
	* Set the **File Name** field to "NativeCCallExample.c"
	* Copy and paste the following code inside the generated [NativeCCallExample.c](CallingCFromJava/src/main/c/com/microej/examples/java2c/NativeCCallExample.c). Notice the C function follows the strict SNI naming convention mentioned earlier.

			#include <sni.h>
			jint Java_com_microej_examples_java2C_NativeCCallExample_someCFunctionReturningTwiceAValue(jint aValue) {
				return aValue*2;
			}


* Right-click on the file that you just created
	* Select **Properties** context menu item
		* Copy the value of the **Resource > Location** field into the clipboard

#### Adding the C file to the BSP IDE project structure (BSP specific)
* Select the root node of your project
	* Right-Click and select **Add Group** this will add a group called "New Group"
	* Select this group and hit **F2** key so as to rename it to "JavaNatives"
	* Right-Click on the **JavaNatives** group and select **Add Existing Files to group 'JavaNatives'...**
	* Navigate to the [NativeCCallExample.c](CallingCFromJava/src/main/c/com/microej/examples/java2C/NativeCCallExample.c) file (its location is in the clipboard ..\\..\\..\\..\\CallingCFromJava\\src\\main\\c\\com\\microej\\examples\\java2c\\NativeCCallExample.c)
	* Click **Add**
	* Click **Close**

#### Getting a clean link (BSP specific)
* Select **Project > Build Target** menu item (or press F7 keyboard shortcut)
	
		*** Using Compiler 'V5.05 update 2 (build 169)', folder: 'C:\Keil_v5\ARM\ARMCC\Bin'
		Build target 'STM32F429i-DISCO'
		compiling NativeCCallExample.c...
		linking...
		Program Size: Code=58744 RO-data=2844 RW-data=288 ZI-data=52912  
		FromELF: creating hex file...
		Finished: 0 information, 1 warning and 0 error messages.
		".\STM32F429i-DISCO\STM32429I-DISCO.axf" - 0 Error(s), 0 Warning(s).
		Build Time Elapsed:  00:00:03

### Flashing the board (BSP specific)
* Connect your board
* Select **Flash > Download** menu item (or press F8 keyboard shortcut).

### Checking the behavior
* Set up a terminal on the board serial port and press the reset input. You shall get the following output :

		VM START
		Calling : someCFunctionReturningTwiceAValue(120)
		Result  : 240
		VM END (exit code = 0)

# Additional references

## Asynchronous communication use case
The Use Case shown in this document only covers a most basic usage of the Java to C interface feature provided by SNI. The native C function is invoked synchronously, meaning the thread of the calling Java method is suspended until the C function returns.
Shall you need a more elaborate example of how to use SNI in an asynchronous context, you may refer to the [ProducerConsumerUsingQueues](/ProducerConsumerUsingQueues) example project.

# Requirements
* JRE 7 (or later) x86.
* MicroEJ SDK 4.0 or later.
* BSP specific toolchain (Keil MicroVision (&trade;) v5 or later.
* A platform with at least:
	* EDC-1.2.0 or higher

# Dependencies
_All dependencies are retrieved transitively by Ivy resolver_.

# Source
N/A

# Restrictions
None.
