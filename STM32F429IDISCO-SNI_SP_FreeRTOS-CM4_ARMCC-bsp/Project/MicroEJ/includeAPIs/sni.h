/*
 * C header file
 * Copyright 2008-2014 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * Header file for Simple Native Interface (SNI), version 1.2
 */
#ifndef SNI_H
#define SNI_H

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef int8_t			jbyte;    /* 8 bits  */
typedef uint8_t         jboolean; /* 8 bits  */
typedef uint16_t        jchar;    /* 16 bits */
typedef int16_t         jshort;   /* 16 bits */
typedef int32_t         jint;     /* 32 bits */
typedef float           jfloat;   /* 32 bits */
typedef double          jdouble;  /* 64 bits */
typedef int64_t         jlong;    /* 64 bits */

//boolean values
#define JTRUE	(1)
#define JFALSE	(0)
#define JNULL   (0)

#define SNI_OK			(0)	//function succeeded
#define SNI_ERROR 		(-1)//an error was detected
#define SNI_INTERRUPTED	(1) //see SNI_suspendCurrentJavaThread   

#define   SNI_getArrayLength(array) (*((jint*)(((uint8_t*)(array))-sizeof(jint))))



/*
 * Creates and initializes a virtual machine.
 * This function MUST be called once before a call to 
 * <code>SNI_startVM()</code>.
 *
 * Returns null if an error occurred, otherwise returns a 
 * virtual machine instance.
 */
void* SNI_createVM(void);

/*
 * Starts the specified virtual machine and calls the 
 * <code>main</code> method of the Java application with
 * the given String arguments.
 * This function returns when the Java application ends. 
 *
 * The Java application ends when there are no more Java 
 * threads to run or when the Java method 
 * <code>System.exit(int)</code> is called. 
 *
 * Returns 0 when the virtual machine ends normally or 
 * a negative value when an error occurred .
 */
int32_t SNI_startVM(void* vm, int32_t argc, char** argv);

/*
 * Call this method after virtual machine execution
 * to get the Java application exit code. 
 *
 * Returns the value given to the <code>System.exit(exitCode)</code> 
 * or 0 if the Java application ended without calling
 * <code>System.exit(exitCode)</code>. 
 */
int32_t SNI_getExitCode(void* vm);

/*
 * Releases all the virtual machine resources. This method
 * must be called after the end of the execution of
 * the virtual machine.
 */
void SNI_destroyVM(void* vm);

/*
 * Returns the ID of the current Java thread.
 * This function must be called within the virtual machine task.
 *
 * Returns <code>SNI_ERROR</code> if this function is not called within the
 * virtual machine task.
 */ 
int32_t SNI_getCurrentJavaThreadID(void);

/*
 * Causes the current Java thread to pause its Java execution after the end 
 * of the current native method. This function is not blocking.
 * The current Java thread will resume its execution after the reception
 * of an external event or after <code>timeout</code> milliseconds.
 *
 * If a resume has been done on this thread before calling this function, the thread
 * is not paused. 
 *
 * The result of calling this method several times during the same native execution 
 * is unpredictable.
 * 
 * Parameter <code>timeout</code> is the duration in milliseconds of the pause. If 
 * <code>timeout</code> is zero, then time is not taken into consideration and the 
 * thread simply waits until resumed.
 *
 * Returns <code>SNI_OK</code> if the request is accepted (i.e. the thread will suspend its execution 
 * at the end of the current native).
 * Returns <code>SNI_ERROR</code> if the method is called outside of the VM Task.
 * Returns <code>SNI_INTERRUPTED</code> if a resume is pending; the current java thread will 
 * not suspend its execution.
 */
int32_t SNI_suspendCurrentJavaThread(int64_t timeout);

/*
 * Resume the given Java thread if it is suspended.
 * If the Java thread is not paused, this resume stays pending.
 * Next call of SNI_suspendCurrentJavaThread() will not suspend the thread.
 * 
 * Parameter <code>javaThreadID</code> is the ID of the Java thread to resume.
 *
 * Returns <code>SNI_ERROR</code> if the given Java thread ID is invalid, otherwise returns <code>SNI_OK</code>.
 */
int32_t SNI_resumeJavaThread(int32_t javaThreadID);

#ifdef __cplusplus
	}
#endif
#endif /* SNI_H */
