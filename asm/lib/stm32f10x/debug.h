#ifndef _DEBUG_H
#define _DEBUG_H 

#include "../common.h"

#if defined(__GNUC__) && defined(__ASSEMBLY__)

#define DWT_CTRL   0xE0001000
#define DWT_CYCCNT 0xE0001004


#define DWT_CTRL_CYCCNTENA 1<<0
#define DWT_CTRL_NOCYCCNT 1<<25

#else

#ifdef __cplusplus
extern "C"
{
#endif

void Debug_EnableCYCCNT(BOOL enable);
void Debug_Delay_cycles (uint32_t cycles);
void delay_us (uint32_t us);

#ifdef __cplusplus
}
#endif

#endif


#endif
