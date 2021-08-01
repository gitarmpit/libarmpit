#ifndef _NVI_H
#define _NVI_H

#include "common.h"
#include "irq.h"

#define NVIC_ISER   (*(volatile uint32_t*)(0xE000E100))
#define NVIC_ICER   (*(volatile uint32_t*)(0xE000E180))
#define NVIC_ISPR   (*(volatile uint32_t*)(0xE000E200))
#define NVIC_ICPR   (*(volatile uint32_t*)(0xE000E280))
#define NVIC_IABR   (*(volatile uint32_t*)(0xE000E300))
#define NVIC_IPR    (*(volatile uint32_t*)(0xE000E400))


#ifdef __cplusplus
extern "C"
{
#endif

void NVIC_SetEnable (IRQn_Type irqNo);
void NVIC_ClearEnable (IRQn_Type irqNo);
void NVIC_SetPending (IRQn_Type irqNo); 
void NVIC_ClearPending (IRQn_Type irqNo); 
int NVIC_IsActive (IRQn_Type irqNo);

void NVIC_SetPriority (IRQn_Type irqNo, uint8_t priority); 
int NVIC_GetPriority (IRQn_Type irqNo);


#ifdef __cplusplus
}
#endif


#endif
