#include "common.h"

uint32_t CORE_FREQ = 20000000;
uint32_t BUS_FREQ =  10000000;
uint32_t TPM_FREQ =  10000000;
uint32_t LPTMR_FREQ = 32768;  //IRC Slow
uint32_t XTAL_FREQ = 32768;


#define NOP() __asm volatile("nop")

__attribute__( ( always_inline ) ) static inline void __nop()
{
  __asm volatile ("nop");
}

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




