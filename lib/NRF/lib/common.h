#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <stddef.h>
#ifdef SEMIHOSTING
#include <stdio.h>
#define _printf  printf 
#else
inline int _printf (const char* arg, ...) { (void)arg; return 0; }
#endif

typedef int BOOL;

#define TRUE 1 
#define FALSE 0


// Macros to extract selected bitmask from a register
#define RANGE_MASK(e, s) ((1<<(e-s+1))-1)
#define EXTRACT_VALUE(v, e, s) ( (v >> s) & (RANGE_MASK(e, s)) )
#define UNUSED(arg)  ((void)arg)

#define NOP() __asm volatile("nop")

__attribute__( ( always_inline ) ) static inline void __nop()
{
  __asm volatile ("nop");
}


extern uint32_t CORE_FREQ;
extern uint32_t BUS_FREQ;
extern uint32_t TPM_FREQ;
extern uint32_t LPTMR_FREQ;
extern uint32_t XTAL_FREQ;

#ifdef __cplusplus
 extern "C" {
#endif

void delay_ms (uint16_t ms);

#ifdef __cplusplus
    }
#endif


#endif
