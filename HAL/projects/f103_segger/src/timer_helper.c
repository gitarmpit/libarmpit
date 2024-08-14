#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_tim.h"

void (*tim1_ptr)(void) = 0;
void (*tim2_ptr)(void) = 0;
void (*tim3_ptr)(void) = 0;

void TIM1_UP_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM1) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM1);
    if (tim1_ptr) {
      tim1_ptr();
    }
  }
}

void TIM2_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM2);
    if (tim2_ptr) {
      tim2_ptr();
    }
  }
}

void TIM3_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM3) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM3);
    if (tim3_ptr) {
      tim3_ptr();
    }
    // Timer_Callback();
  }
}

static uint32_t GetTIMx_CLK(int isAPB1) {

  LL_RCC_ClocksTypeDef RCC_Clocks;
  LL_RCC_GetSystemClocksFreq(&RCC_Clocks);

  uint8_t apb_presc;
  if (isAPB1) {
    apb_presc = 1 << (APBPrescTable[LL_RCC_GetAPB1Prescaler() >> RCC_CFGR_PPRE1_Pos]);
  } else {
    apb_presc = 1 << (APBPrescTable[LL_RCC_GetAPB2Prescaler() >> RCC_CFGR_PPRE2_Pos]);
  }

  uint32_t timx_clk = RCC_Clocks.HCLK_Frequency / apb_presc;
  if (apb_presc > 1) {
    timx_clk *= 2;
  }

  return timx_clk;
}


static void CalculateTimerValues(int isAPB1, uint32_t us, uint16_t* presc, uint16_t* arr) {

  uint32_t timClk = GetTIMx_CLK(isAPB1);

  timClk /= 2;

  uint64_t timerCount = (uint64_t)timClk * (uint64_t)us / 1000000llu;

  if (timerCount == 0) {
    timerCount = 1;
  }

  const uint32_t maxVal = 0xFFFF;

  *presc = 1;

  if (timerCount > maxVal) {
    *presc     = (uint16_t)(timerCount / maxVal) + 1;
    timerCount = timerCount / *presc + 1;
  }

  *arr = (uint16_t)(timerCount - 1);
  --(*presc);
}

void TIM_SetUpdatePeriod_us(TIM_TypeDef* timer, uint32_t us) {
  uint32_t addr = (uint32_t)timer;
  int isAPB1    = (addr < APB2PERIPH_BASE);
  uint16_t presc, arr;

  CalculateTimerValues(isAPB1, us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(timer, presc);
  LL_TIM_SetAutoReload(timer, arr);
}

static void TIM_SetUpCounter(TIM_TypeDef* tim, uint32_t periph, IRQn_Type IRQn, uint32_t us) {
  // LL_APB1_GRP1_EnableClock(periph);
  LL_TIM_SetCounterMode(tim, LL_TIM_COUNTERMODE_UP);

  TIM_SetUpdatePeriod_us(tim, us);

  LL_TIM_EnableIT_UPDATE(tim);

  NVIC_SetPriority(IRQn, 0);
  NVIC_EnableIRQ(IRQn);

  LL_TIM_EnableCounter(tim);
}

void TIM_SetupCounterTIM1(uint32_t period_us, void (*tim_ptr)(void)) {
  tim1_ptr = tim_ptr;
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  TIM_SetUpCounter(TIM1, LL_APB2_GRP1_PERIPH_TIM1, TIM1_UP_IRQn, period_us);
}

void TIM_SetupCounterTIM2(uint32_t period_us, void (*tim_ptr)(void)) {
  tim2_ptr = tim_ptr;
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  TIM_SetUpCounter(TIM2, LL_APB1_GRP1_PERIPH_TIM2, TIM2_IRQn, period_us);
}

void TIM_SetupCounterTIM3(uint32_t period_us, void (*tim_ptr)(void)) {
  tim3_ptr = tim_ptr;
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
  TIM_SetUpCounter(TIM3, LL_APB1_GRP1_PERIPH_TIM3, TIM3_IRQn, period_us);
}

void TIM_SetupPWM(TIM_TypeDef* tim, int channel, uint32_t perioud_us, uint32_t ds_us) {}

void TIM_SetupPWM_TIM2(int channel, uint32_t period_us, uint32_t ds_us) {
  TIM_SetupPWM(TIM2, channel, period_us, ds_us);
}
