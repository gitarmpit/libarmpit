#include "debug.h"
#include "rcc.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef STM32F1
#define FUDGE 13
#else
#define FUDGE 9
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
            g_fudge = FUDGE;
            if (g_fudge > g_ahb_clock_mhz)
            {
                g_fudge = g_ahb_clock_mhz;
            }
        }
        else
        {
            DWT_CTRL &= ~DWT_CTRL_CYCCNTENA;
        }

    }
}

#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif
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

void delay(uint32_t ms)
{
    delay_us(ms * 1000);
}


#ifdef __GNUC__
#pragma GCC pop_options
#endif

#ifdef __cplusplus
}
#endif
