#include "common.h"

uint32_t CORE_FREQ = 20000000;
uint32_t BUS_FREQ =  10000000;
uint32_t TPM_FREQ =  10000000;
uint32_t LPTMR_FREQ = 32768;  //IRC Slow
uint32_t XTAL_FREQ = 32768;



// extern "C" void SystemInit()
void SystemInit()
{
    // __asm volatile("mov r0, r0");
}


#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif
void delay_ms2 (int ms) {
    uint64_t ms64 = (uint64_t)ms * CORE_FREQ/10000;
    while (ms64--) { __nop();  }
}

void delay_ms (uint16_t ms) {
    uint64_t ms64 = (uint64_t)ms * CORE_FREQ/10000;
    while (ms64--) { __nop(); __nop(); __nop(); __nop(); __nop(); }
}

#ifdef __GNUC__
#pragma GCC pop_options
#endif

#ifndef SEMIHOSTING
#include <sys/stat.h>

extern int  __end__;

__attribute__ ((used))
 caddr_t _sbrk ( int incr )
{
    static unsigned char *heap = NULL;
    unsigned char *prev_heap;

    if (heap == NULL) {
        heap = (unsigned char *)&__end__;
    }
    prev_heap = heap;

    heap += incr;

    return (caddr_t) prev_heap;
}

#endif



