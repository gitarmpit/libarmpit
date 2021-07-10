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

void SystickEnable (uint32_t count, BOOL enableInterrupt, BOOL isDIV16, void (*handler)(void))
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


    if (isDIV16)
    {
        SYST_CSR &= ~SYST_CSR_CLKSOURCE_CORE;
    }
    else
    {
        SYST_CSR |= SYST_CSR_CLKSOURCE_CORE;
    }

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
