#include <stdint.h>
#include "system_control.h"

void SystemInit() 
{
}

#define APPLICATION_ADDRESS        0x08040000

typedef void (*pFunction)(void);

__attribute__( ( always_inline ) ) static inline void __set_MSP(uint32_t topOfMainStack)
{
  __asm volatile ("MSR msp, %0\n" : : "r" (topOfMainStack) : "sp");
}

int boot_main(void)
{

    pFunction appEntry;
    uint32_t appStack;

    /* Get the application stack pointer (First entry in the application vector table) */
    appStack = (uint32_t) *((volatile uint32_t*)APPLICATION_ADDRESS);

    /* Get the application entry point (Second entry in the application vector table) */
    appEntry = (pFunction) *(volatile uint32_t*) (APPLICATION_ADDRESS + 4);

    /* Reconfigure vector table offset register to match the application location */
    SCB_VTOR = APPLICATION_ADDRESS;

    /* Set the application stack pointer */
    __set_MSP(appStack);

    /* Start the application */
    appEntry();

    while(1);

}
