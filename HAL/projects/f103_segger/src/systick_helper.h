#ifndef _SYSTICK_HELPER_H
#define _SYSTICK_HELPER_H 

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_ll_utils.h"
//#include <stdint.h>

extern __IO uint32_t uwTick;

void      SysTick_Init();
void      SysTick_IncTick();
uint32_t  SysTick_GetTick(void);
void      SysTick_Delay(uint32_t delay_ms);

#ifdef __cplusplus
}
#endif


#endif
