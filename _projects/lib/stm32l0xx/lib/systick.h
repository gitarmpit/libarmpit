#ifndef _SYSTICK_H
#define _SYSTICK_H 

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

extern volatile uint32_t uwTick;
extern uint8_t Systick_Tick_ms;

void      SysTick_Init(void);
void      SysTick_IncTick(void);
uint32_t  SysTick_GetTick(void);
void      SysTick_Delay(uint32_t delay_ms);

#ifdef __cplusplus
}
#endif


#endif
