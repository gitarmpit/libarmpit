#include "rcc.h"
#include "system_control.h"
#include "debug.h"


#ifdef __cplusplus
extern "C"
{
#endif


//In all the examples system init is done as the first step in main (starting clocks, setting flash latency etc) 
//Can also do it here. (Makes no difference?)  This location is pointed to by the assembly start-up files provided by ARM
void SystemInit(void)
{
    FPU_Enable(TRUE);
    SCB_EnableBusFault();
    SCB_EnableDivByZeroTrap();
    SCB_EnableMemoryFault();
    SCB_EnableUnalignedTrap();
    SCB_EnableUsageFault();
    Debug_EnableCYCCNT(TRUE);
}


void HardFault_Handler(void) 
{
    static volatile SCB_SHCSR_def shcsr;
    static volatile SCB_CFSR_def cfsr;
    SCB_GetFaulStatus (&shcsr, &cfsr);
    while(1);
}

void UsageFault_Handler(void) 
{
    static volatile SCB_SHCSR_def shcsr;
    static volatile SCB_CFSR_def cfsr;
    SCB_GetFaulStatus (&shcsr, &cfsr);
    while(1);
}

void BusFault_Handler(void) 
{
    static volatile SCB_SHCSR_def shcsr;
    static volatile SCB_CFSR_def cfsr;
    SCB_GetFaulStatus (&shcsr, &cfsr);
    while(1);
}

void MemManage_Handler(void) 
{
    static volatile SCB_SHCSR_def shcsr;
    static volatile SCB_CFSR_def cfsr;
    SCB_GetFaulStatus (&shcsr, &cfsr);
    while(1);
}


#ifdef __cplusplus
}
#endif
