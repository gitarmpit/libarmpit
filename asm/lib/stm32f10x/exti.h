#ifndef _EXTI_H
#define _EXTI_H

#if defined(__GNUC__) && defined(__ASSEMBLY__)


#if defined (STM32F1) || defined (STM32L0)
#define EXTI_BASE 0x40010400  //APB2
#elif defined (STM32F2) || defined (STM32F4)
#define EXTI_BASE 0x40013C00  //APB2
#else 
#error "STM32 target device not defined"
#endif


#define EXTI_IMR   EXTI_BASE         //interrupt mask
#define EXTI_EMR   EXTI_BASE + 0x4   //event mask
#define EXTI_RTSR  EXTI_BASE + 0x8   //rising  edge
#define EXTI_FTSR  EXTI_BASE + 0xc   //falling edge
#define EXTI_SWIER EXTI_BASE + 0x10  //software int event
#define EXTI_PR    EXTI_BASE + 0x14  //pending


#else 

#include "common.h"

void EXTI_IsPending (uint8_t exti_line, BOOL clear_if_set);
void EXTI_ClearPending(uint8_t exti_line);
void EXTI_EnableEventFallingEdge(uint8_t exti_line, BOOL enable);
void EXTI_EnableEventRisingEdge(uint8_t exti_line, BOOL enable);
void EXTI_EnableInterruptFallingEdge(uint8_t exti_line, BOOL enable);
void EXTI_EnableInterruptRisingEdge(uint8_t exti_line, BOOL enable);



#endif


#endif
