#include "debug.h"
#include "rcc.h"

#ifdef __cplusplus
extern "C"
{
#endif


static uint8_t g_ahb_clock_mhz;
static uint8_t g_fudge;

void Debug_EnableCYCCNT(BOOL enable)
{
    
    if (!(DWT_CTRL & DWT_CTRL_NOCYCCNT))
    {
        if (enable)
        {
            SCB_EnableDWT(TRUE);
            DWT_CTRL |= DWT_CTRL_CYCCNTENA;
            g_ahb_clock_mhz = RCC_GetAHB_CLK() / 1000000;
            g_fudge = 46;
        }
        else
        {
            DWT_CTRL &= ~DWT_CTRL_CYCCNTENA;
        }

    }
}

#pragma GCC push_options
#pragma GCC optimize ("O3")

void Debug_Delay_cycles (uint32_t cycles)
{
    DWT_CYCCNT = 0;
    do
    {
    } while (DWT_CYCCNT < cycles);
}

void delay_us (uint32_t us)
{
    DWT_CYCCNT = 0;
    //uint32_t cycles = us < 2 ? 0 : (g_ahb_clock_mhz) * (us-2) + g_ahb_clock_mhz/2;
    uint32_t cycles = g_ahb_clock_mhz * us - g_fudge;

    do
    {
    } while (DWT_CYCCNT < cycles);
}

#pragma GCC pop_options


#ifdef __cplusplus
}
#endif
