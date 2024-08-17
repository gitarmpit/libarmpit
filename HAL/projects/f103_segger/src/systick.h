#ifndef _SYSTICK_H
#define _SYSTICK_H 

#include "stm32f103xb.h"
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif


extern __IO uint32_t uwTick;

void      SysTick_Init(void);
void      SysTick_IncTick(void);
uint32_t  SysTick_GetTick(void);
void      SysTick_Delay(uint32_t delay_ms);

#ifdef __cplusplus
}
#endif


#endif
