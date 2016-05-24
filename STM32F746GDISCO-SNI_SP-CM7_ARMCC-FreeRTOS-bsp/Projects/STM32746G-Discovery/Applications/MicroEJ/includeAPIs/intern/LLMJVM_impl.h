/* 
 * Copyright 2011-2014 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
/* 
 * AUTOGENERATED by COMP-ICETEA-ARMCC-ARM - 1.0.0 - DO NOT EDIT IT
 */
/* 
 * IceTea to C mapping support. Shall not be directly included by clients.
 */
typedef struct LLMJVM LLMJVM;
#include <stdint.h>
#define LLMJVM_CONCAT0(p,s) p ## s
#define LLMJVM_CONCAT(p,s) LLMJVM_CONCAT0(p,s)
#define LLMJVM_IMPL_initialize LLMJVM_CONCAT(LLMJVM_IMPL,_initialize)
#define LLMJVM_IMPL_vmTaskStarted LLMJVM_CONCAT(LLMJVM_IMPL,_vmTaskStarted)
#define LLMJVM_IMPL_scheduleRequest LLMJVM_CONCAT(LLMJVM_IMPL,_scheduleRequest__J)
#define LLMJVM_IMPL_idleVM LLMJVM_CONCAT(LLMJVM_IMPL,_idleVM)
#define LLMJVM_IMPL_wakeupVM LLMJVM_CONCAT(LLMJVM_IMPL,_wakeupVM)
#define LLMJVM_IMPL_ackWakeup LLMJVM_CONCAT(LLMJVM_IMPL,_ackWakeup)
#define LLMJVM_IMPL_getCurrentTaskID LLMJVM_CONCAT(LLMJVM_IMPL,_getCurrentTaskID)
#define LLMJVM_IMPL_shutdown LLMJVM_CONCAT(LLMJVM_IMPL,_shutdown)
#define LLMJVM_IMPL_setApplicationTime LLMJVM_CONCAT(LLMJVM_IMPL,_setApplicationTime__J)
#define LLMJVM_IMPL_getCurrentTime LLMJVM_CONCAT(LLMJVM_IMPL,_getCurrentTime__Z)
#define LLMJVM_IMPL_getTimeNanos LLMJVM_CONCAT(LLMJVM_IMPL,_getTimeNanos)
#define LLMJVM_schedule __icetea__virtual__com_is2t_microjvm_bsp_IGreenThreadMicroJvmScheduler___schedule
struct LLMJVM
{
	int32_t __icetea__id__;
};
LLMJVM LLMJVM_getSingleton_instance;

/* 
 * Get the unique instance of this type within the system.
 */
LLMJVM* LLMJVM_IMPL_getSingleton(void)
{
	return &LLMJVM_getSingleton_instance;
}
