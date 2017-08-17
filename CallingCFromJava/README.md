[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](For demonstration purpose only.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This example shows the minimum steps required to create a Java program that makes a blocking call to a C function via the SNI interface, starting from an empty MicroEJ workspace.

The following steps will be taken :

* Running the application on simulator
* Running the application on target

## Requirements
* JRE 7 (or later) x86.
* MicroEJ 4.0 or later.
* BSP specific toolchain (Keil MicroVision (&trade;) v5 or later.

# Running the application on simulator

Using MicroEJ, you may deploy and run your application on an embedded target (if the hardware and related BSP are available) or you may run your application on a Java simulator mimicking the behavior of your embedded target.

## Building for the simulator
Here, we will launch a MicroEJ "Run Configuration" that will compile the Java code from the CallingCFromJava project for the JPF we created previously.

* Select **Run > Run Configurations...** menu item
* Select **MicroEJ Application** group
* Select the [NativeCCallExample_Sim_Build.launch](/CallingCFromJava/launches/NativeCCallExample_Sim_Build.launch) configuration
	* In **Execution** tab
		* In **Target** frame
			* Click the **Browse** button next to the JPF Field and select your platform
		* In **Execution** frame
			* Notice that "Execute on SimJPF" radio button option is checked
	* Click on "Run"

## Getting a java.lang.UnsatisfiedLinkError exception

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

This is perfectly normal since in [NativeCCallExample.java](CallingCFromJava/src/main/java/com/microej/example/java2C/NativeCCallExample.java) we declared **someCFunctionReturningTwiceAValue** as a native function, when running the simulator, the Hardware In the Loop (HIL) engines expects to find some Java implementation emulating the behavior of the native function. 

## Adding a mock of the native function to the JPF
Since our Java application relies on native C functions, on an embedded target, we would need to provide a C implementation. But given that we are running it on a Java simulator, we can emulate those functions using a Java mock.

The [CallingCFromJavaMock](CallingCFromJavaMock) project provides the mocks required for running the Java application on simulator (see [/CallingCFromJavaMock/.../NativeCCallExample.java](/CallingCFromJavaMock/src/main/java/com/microej/example/java2c/NativeCCallExample.java)).

* Open the [CallingCFromJavaMock.jardesc](/CallingCFromJavaMock/CallingCFromJavaMock.jardesc) jar description file
* Update the export destination file so that it has the following value
	`STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-1.0.0\source\mocks\dropins\CallingCFromJavaMock.jar`
* Click on **Finish**
* Retry running the [NativeCCallExample_Sim_Build.launch](/CallingCFromJava/launches/NativeCCallExample_Sim_Build.launch) configuration.

		=============== [ Initialization Stage ] ===============
		=============== [ Launching SimJPF ] ===============
		Calling : someCFunctionReturningTwiceAValue(120)
		Result  : 240
		=============== [ Completed Successfully ] ===============
		
		SUCCESS


# Running the application on target

## Building for the target
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

## Opening the generated C project (BSP specific)
* From the **Project Explorer** view
	* Navigate to the [/STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-bsp/Projects/STM32746G-Discovery/Applications/MicroEJ/MDK-ARM](/STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-bsp/Projects/STM32746G-Discovery/Applications/MicroEJ/MDK-ARM) folder
	* Double-click on the [Project.uvprojx](/STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-bsp/Projects/STM32746G-Discovery/Applications/MicroEJ/MDK-ARM/Project.uvprojx) file (this will open the BSP project in the MicroVision IDE)

## Getting a linker error (BSP specific)
* From the MicroVision IDE
	* Select **Project > Build Target** menu item (or press F7 keyboard shortcut)
	* A linker error message shall appear :
	
		```STM32746G_DISCOVERY\STM32746G_DISCOVERY.axf: Error: L6218E: Undefined symbol Java_com_microej_example_java2c_NativeCCallExample_someCFunctionReturningTwiceAValue (referred from microejapp.o).```


This is perfectly normal since in [NativeCCallExample.java](CallingCFromJava/src/main/java/com/microej/examples/java2C/NativeCCallExample.java) we declared **someCFunctionReturningTwiceAValue** as a native function, when building the MicroEJ project, the generated linker configuration file expects to find a C function definition matching the qualified name of the function. 

## Fixing the linker error
### C Native function implementation
* From the **Package Explorer** view
	* Select the [NativeCCallExample.c](CallingCFromJava/src/main/c/com/microej/examples/java2c/NativeCCallExample.c) file.
	* Double-click on the file
		* The file contents appear. Notice the C function follows the strict SNI naming convention mentioned earlier.

				#include <sni.h>
				jint Java_com_microej_examples_java2C_NativeCCallExample_someCFunctionReturningTwiceAValue(jint aValue) {
					return aValue*2;
				}
		
	* Right-click on the file
	* Select **Properties** context menu item
	* Copy the value of the **Resource > Location** field into the clipboard.

### Adding the C file to the BSP IDE project structure (BSP specific)
* Select the root node of your project
	* Right-Click and select **Add Group** this will add a group called "New Group"
	* Select this group and hit **F2** key so as to rename it to "JavaNatives"
	* Right-Click on the **JavaNatives** group and select **Add Existing Files to group 'JavaNatives'...**
	* Paste the contents of the clipboard in the "File Name" field.
	* Click **Add**
	* Click **Close**
		* The absolute path of the [NativeCCallExample.c](CallingCFromJava/src/main/c/com/microej/examples/java2c/NativeCCallExample.c) file will be converted to a path relative to the BSP project file.

### Getting a clean link (BSP specific)
* Select **Project > Build Target** menu item (or press F7 keyboard shortcut)
	
		*** Using Compiler 'V5.06 update 1 (build 61)', folder: 'C:\Keil_v5\ARM\ARMCC\Bin'
		Build target 'STM32746G_DISCOVERY'
		"STM32746G_DISCOVERY\STM32746G_DISCOVERY.axf" - 0 Error(s), 0 Warning(s).
		Build Time Elapsed:  00:00:01
		Load "STM32746G_DISCOVERY\\STM32746G_DISCOVERY.axf" 
		Erase Done.
		Programming Done.
		Application running ...
		Flash Load finished at 14:36:43

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

The Use Case shown in this document only covers a most basic usage of the Java to C interface feature provided by SNI. The native C function is invoked synchronously, meaning the all the Java threads managed by the MicroEJ Virtual Machine are suspended until the C function returns.

Shall you need a more elaborate example of how to use SNI in an asynchronous context, you may refer to the [ProducerConsumerUsingQueues](/ProducerConsumerUsingQueues) example project.