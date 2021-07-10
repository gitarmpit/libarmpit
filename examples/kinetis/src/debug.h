#ifndef _DEBUG_H
#define _DEBUG_H 

#include "common.h"
#include "system_control.h"

#define DWT_CTRL   (*(volatile uint32_t*)(0xE0001000))
#define DWT_CYCCNT (*(volatile uint32_t*)(0xE0001004))


#define DWT_CTRL_CYCCNTENA 1<<0
#define DWT_CTRL_NOCYCCNT  1<<25

#ifdef __cplusplus
extern "C"
{
#endif

void Debug_EnableCYCCNT(BOOL enable);
void Debug_Delay_cycles (uint32_t cycles);
void delay_us (uint32_t us);
void delay(uint32_t arg);

#ifdef __cplusplus
}
#endif


#endif
