[](Markdown)
[](Copyright 2015 IS2T. All rights reserved.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This example shows the minimum steps required to create a Java program that calls a C function via the SNI interface, starting from an empty MicroEJ workspace.

The following steps will be taken :

* Building a Java Platform
* Building the Java client code
* Building & Running the C target executable

## Requirements
* JRE 7 (or later) x86.
* MicroEJ 3.1 or later.
* BSP specific toolchain.

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
		* [**CallingCFromJava**-bsp](CallingCFromJava-bsp)
		* [**CallingCFromJava**-configuration](CallingCFromJava-configuration)
		* [**CallingCFromJava**-javaExample](CallingCFromJava-javaExample)
	* Delete the [**CallingCFromJava**-javaExample](CallingCFromJava-javaExample) project

## Modules selection
* From the **Content** tab of the generated example.platform file
	* In the **modules** frame
		* Click on the **Show/Hide modules** button
		* Select the **Simple Native Interface API** option in the modules checkbox tree

## Build
* From the **Overview** tab of the generated example.platform file
	* Click on the **Build platform** hyperlink. This will generate the following project :
		* [**CallingCFromJava**-1.0.0](CallingCFromJava-1.0.0)
* Click on **Window > Preferences > MicroEJ**
	* In the **MicroEJ Repository** frame
		* Click on the **Refresh** button 

# Building the Java client code
In this section, we will create the java application to invoke our C code

## MicroEJ project creation

This project will host the application specific code

* Select **File > New > Java Project** menu item
	* Set **Project Name** field to **"CallingCFromJava"**
	* In the **Runtime Environment** frame, select the following MicroEJ Libraries :
		* EDC 1.2 (selected by default)
		* SNI 1.2.0
* Click on **Next**
* Click on **Finish**

## Java Class creation
* Select **File > New > Source Folder** menu item
	* Set the **Folder name** field to /src/main/java
* Select **File > New > Class** menu item
	* Set the **Source folder** field to "CallingCFromJava/src/main/java"
	* Set the **Package** field to "com.is2t.examples.java2C"
	* Set the **Name** field to "NativeCCallExample"
	* Click on **Finish**
	* Copy and paste the following code inside the generated [NativeCCallExample.java](/CallingCFromJava/src/main/java/com/is2t/examples/java2C/NativeCCallExample.java) file 

			package com.is2t.examples.java2C;		
			public class NativeCCallExample {
			
				public static void main(String[] args) {
					final int aValue = 120;
					System.out.println("Calling : someCFunctionReturningTwiceAValue(" + aValue +")");
					final int resultOfCallToSomeCFunction = someCFunctionReturningTwiceAValue(aValue);		
					System.out.println("Result  : " + resultOfCallToSomeCFunction);
				}
				
				static native int someCFunctionReturningTwiceAValue(int n);
			}


## Compiling the Java code for the target

Here, we will create a MicroEJ "Run Configuration" that will compile the Java code we just created for the JPF we created previously

* Select **File > New > Folder** menu item
	* Set the **Enter or select the parent folder** field to ["CallingCFromJava"](/CallingCFromJava) (Project root)
	* Set the **Folder Name** field to ["launches"](/CallingCFromJava/launches)
* Select **Run Configurations...** from the **Run configurations** drop down list (or Press Ctrl+Shift+3 and type "Run Configurations...")
* Select **MicroEJ Application** group and click **New**
	* Set **Name** field to "NativeCCallExample_Build"
	* In **Main** tab
		* Set the **Project** field to "CallingCFromJava"
		* Click on **Select Main type...** and type NativeCCallExample
	* In **Execution** tab
		* In **Target** frame
			* Click the **Browse** button next to the JPF Field and select your platform
		* In **Execution** frame
			* Select **Execute on EmbJPF** radio button
			* Leave the **Output Folder** field set to "${project_loc:CallingCFromJava}"
	* In **Configuration** tab
		* Select the **Target > Deploy** Node
			* Set the **Means** field to "Copy at a location known by BSP Project"
	* In **Common** tab
		* In **Save as** frame
			* Select the **Shared file** radio button
			* Click on **Browse** and select [CallingCFromJava/launches](/CallingCFromJava/launches) folder	
	* Click on "Run"

# Building & Running the C target executable

## Opening the generated C project (BSP specific)
* From the **Project Explorer** view
	* Navigate to the [**CallingCFromJava**-bsp/Project/MicroEJ/MDK-ARM](CallingCFromJava-bsp/Project/MicroEJ/MDK-ARM) folder
	* Double-click on the [Project.uvproj](CallingCFromJava-bsp/Project/MicroEJ/MDK-ARM/Project.uvproj) file (this will open the BSP project in the MicroVision IDE)

## Getting a linker error (BSP specific)
* From the MicroVision IDE
	* Select **Project > Build Target** menu item (or press F7 keyboard shortcut)
	* A linker error message shall appear :

		```.\STM32F429i-DISCO\STM32429I-DISCO.axf: Error: L6218E: Undefined symbol Java_com_is2t_examples_java2C_NativeCCallExample_someCFunctionReturningTwiceAValue (referred from javaapp.o).```

This is perfectly normal since in [NativeCCallExample.java](/CallingCFromJava/src/main/java/com/is2t/examples/java2C/NativeCCallExample.java) we declared **someCFunctionReturningTwiceAValue** as a native function, when building the MicroEJ project, the generated linker configuration file expects to find a C function definition matching the qualified name of the function. 

## Fixing the linker error
### C Native function implementation
* Select **File > New > Source Folder** menu item
	* Set the **Folder Name** field to "src/main/c"
* Right-Click on the folder that you just created
	* Select **New > File** context menu item
	* Set the **File Name** field to "NativeCCallExample.c"
	* Copy and paste the following code inside the generated [NativeCCallExample.c](/CallingCFromJava/src/main/c/com/is2t/examples/java2C/NativeCCallExample.c)

			#include <sni.h>
			int Java_com_is2t_examples_java2C_NativeCCallExample_someCFunctionReturningTwiceAValue(int aValue) {
				return aValue*2;
			}

* Right-click on the file that you just created
	* Select **Properties** context menu item
		* Copy the value of the **Resource > Location** field into the clipboard

### Adding the C file to the BSP IDE project structure (BSP specific)
* Select the root node of your project
	* Right-Click and select **Add Group** this will add a group called "New Group"
	* Select this group and hit **F2** key so as to rename it to "JavaNatives"
	* Right-Click on the **JavaNatives** group and select **Add Existing Files to group 'JavaNatives'...**
	* Navigate to the [NativeCCallExample.c](/CallingCFromJava/src/main/c/com/is2t/examples/java2C/NativeCCallExample.c) file (its location is in the clipboard ..\\..\\..\\..\\CallingCFromJava\\bsp)
	* Click **Add**
	* Click **Close**

### Getting a clean link (BSP specific)
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
## SNI Library Specification

The **Simple Native Interface for GreenThread Context Profile Specification** document contains detailed information on :

* how to map Java primitive types to portable C types for methods/functions parameters and return types
* naming conventions for C functions implementing Java SNI native implementations
	* these have to be followed to generate valid linker configuration files
* sharing memory areas between Java C
* controlling (as in suspend and resume) Java Thread execution from C code

To access the document from MicroEJ :

* Select **Help > MicroEJ Resource Center** menu item
	* Select **Library Specification > SNI_GT 1.2**

## Asynchronous communication use case

The Use Case shown in this document only covers a most basic usage of the Java to C interface feature provided by SNI. The native C function is invoked synchronously, meaning the thread of the calling Java method is suspended until the C function returns.

Shall you need a more elaborate example of how to use SNI in an asynchronous context, you may refer to :

* the **javaExample** provided with the **Basic-SNI and SP** template when creating a new Java Platform (just ensure that you do not select the **Basic** template as is the case in this document).
* the [ProducerConsumerUsingShieldedPlug](/ProducerConsumerUsingShieldedPlug) example project