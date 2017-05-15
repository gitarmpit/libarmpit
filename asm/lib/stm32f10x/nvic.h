#ifndef _NVIC_H
#define _NVIC_H

#include "common.h"

#if defined(__GNUC__) && defined(__ASSEMBLY__)

#define NVIC_ISER   0xE000E100
#define NVIC_ICER   0xE000E180
#define NVIC_ISPR   0xE000E200
#define NVIC_ICPR   0xE000E280
#define NVIC_IABR   0xE000E300
#define NVIC_IPR    0xE000E400

#else

#include "irq.h"


#ifdef __cplusplus
extern "C"
{
#endif

void NVIC_SetEnable (IRQn_Type irqNo);
void NVIC_ClearEnable (IRQn_Type irqNo);
void NVIC_SetPending (IRQn_Type irqNo); 
void NVIC_ClearPending (IRQn_Type irqNo); 
int  NVIC_IsActive (IRQn_Type irqNo);

void NVIC_SetPriority (IRQn_Type irqNo, uint8_t priority); 
int  NVIC_GetPriority (IRQn_Type irqNo);


#ifdef __cplusplus
}
#endif

#endif

#endif
