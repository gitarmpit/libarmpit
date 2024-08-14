#include "systick.h"

static void (*g_handler)(void);

#ifdef __cplusplus
extern "C"
{
#endif

void SysTick_Handler(void) 
{
    if (g_handler)
    {
        g_handler();
    }
}

#ifdef __cplusplus
}
#endif

#if defined(STM32) 
void SystickEnable (uint32_t count, BOOL enableInterrupt, BOOL isDIV8, void (*handler)(void))
#else 
void SystickEnable (uint32_t count, BOOL enableInterrupt, void (*handler)(void))
#endif
{
    SYST_LOAD = (count - 1) & 0xffffff;
    
    SYST_VAL = 0;
    if (enableInterrupt)
    {
        SYST_CSR |= SYST_CSR_TICKINT;
    }
    else
    {
        SYST_CSR &= ~SYST_CSR_TICKINT;
    }

    SYST_CSR &= ~SYST_CSR_CLKSOURCE_CLEARMASK;

#if defined(STM32) 
    if (isDIV8)
    {
        SYST_CSR |= SYST_CSR_CLKSOURCE_AHB_DIV8;
    }
    else
    {
        SYST_CSR |= SYST_CSR_CLKSOURCE_AHB;
    }
#else
        SYST_CSR |= SYST_CSR_CLKSOURCE_CORE;
#endif

    SYST_CSR |= SYST_CSR_ENABLE;

    g_handler = handler;

}

void SystickSetReloadValue (uint32_t count)
{
    SYST_LOAD = count;
    SYST_VAL = 0;
}

void SystickDisable(void)
{
    SYST_CSR &= ~SYST_CSR_ENABLE;
}

int32_t SystickGetValue()
{
    return SYST_VAL;
}
