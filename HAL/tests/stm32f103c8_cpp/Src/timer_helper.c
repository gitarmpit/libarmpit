#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"

static void CalculateTimerValues (int isAPB1, uint32_t us, uint16_t* presc, uint16_t* arr)
{
    LL_RCC_ClocksTypeDef RCC_Clocks;
    LL_RCC_GetSystemClocksFreq(&RCC_Clocks);

    uint32_t timClk = isAPB1 ? RCC_Clocks.PCLK1_Frequency : RCC_Clocks.PCLK2_Frequency;

    uint64_t count64 = (uint64_t)timClk  * (uint64_t)us / 1000000llu;

    uint32_t max = 0xffff;

    if (count64 == 0)
    {
        count64 = 1;
    }

    *presc = 1;

    while (count64 >= max)
    {
        count64 /= 2;
        (*presc) *= 2;
    }

    *arr = (uint16_t)count64 - 1;
    --(*presc);

}

void TIM_SetUpdatePeriod_us (TIM_TypeDef *timer, uint32_t us)
{
   uint32_t addr = (uint32_t)timer;
   int isAPB1 = (addr < APB2PERIPH_BASE);
   uint16_t presc, arr;
   CalculateTimerValues(isAPB1, us, &presc, &arr);

   LL_TIM_SetPrescaler(TIM2, presc);
   LL_TIM_SetAutoReload(TIM2, arr);
}

static void TIM_SetUpCounter(TIM_TypeDef *tim, uint32_t periph, IRQn_Type IRQn, uint32_t us)
{
    LL_APB1_GRP1_EnableClock(periph);
    LL_TIM_SetCounterMode(tim, LL_TIM_COUNTERMODE_UP);

    TIM_SetUpdatePeriod_us(tim, us);

    LL_TIM_EnableIT_UPDATE(tim);

    NVIC_SetPriority(IRQn, 0);
    NVIC_EnableIRQ(IRQn);

    LL_TIM_EnableCounter(tim);

}

void TIM_SetupCounterTIM1(uint32_t period_us)
{
    TIM_SetUpCounter(TIM1, LL_APB2_GRP1_PERIPH_TIM1, TIM1_UP_IRQn, period_us);
}

void TIM_SetupCounterTIM2(uint32_t period_us)
{
    TIM_SetUpCounter(TIM2, LL_APB1_GRP1_PERIPH_TIM2, TIM2_IRQn, period_us);
}

void TIM_SetupCounterTIM3(uint32_t period_us)
{
    TIM_SetUpCounter(TIM3, LL_APB1_GRP1_PERIPH_TIM3, TIM3_IRQn, period_us);
}
