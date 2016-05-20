/*
 * C99
 *
 * Copyright 2014-2015 IS2T.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef FAULT_HANDLERS_H
#define FAULT_HANDLERS_H

void HardFault_Handler(void);
void MemFault_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);

#endif // FAULT_HANDLERS_H
