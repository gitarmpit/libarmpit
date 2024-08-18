#include "timer_helper.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_tim.h"
#include "gpio_helper.h"
#include "system_init.h"

tim_handler tim1_handler;
tim_handler tim2_handler;
tim_handler tim3_handler;
tim_handler tim4_handler;
tim_handler tim5_handler;
tim_handler tim6_handler;
tim_handler tim7_handler;
tim_handler tim8_handler;
tim_handler tim9_handler;
tim_handler tim10_handler;
tim_handler tim11_handler;
tim_handler tim12_handler;
tim_handler tim13_handler;
tim_handler tim14_handler;

static void* tim1_ctx = 0;
static void* tim2_ctx = 0;
static void* tim3_ctx = 0;
static void* tim4_ctx = 0;
static void* tim5_ctx = 0;
static void* tim6_ctx = 0;
static void* tim7_ctx = 0;
static void* tim8_ctx = 0;
static void* tim9_ctx = 0;
static void* tim10_ctx = 0;
static void* tim11_ctx = 0;
static void* tim12_ctx = 0;
static void* tim13_ctx = 0;
static void* tim14_ctx = 0;

// 1, 10
void TIM1_UP_TIM10_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM1) && LL_TIM_IsEnabledIT_UPDATE(TIM1)) {
    LL_TIM_ClearFlag_UPDATE(TIM1);
    if (tim1_handler) {
      tim1_handler(tim1_ctx);
    }
  } else if (LL_TIM_IsActiveFlag_UPDATE(TIM10) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM10);
    if (tim10_handler) {
      tim10_handler(tim10_ctx);
    }
  }

}

// 2
void TIM2_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM2);
    if (tim2_handler) {
      tim2_handler(tim2_ctx);
    }
  }
}

// 3
void TIM3_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM3) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM3);
    if (tim3_handler) {
      tim3_handler(tim3_ctx);
    }
  }
}

// 4
void TIM4_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM4) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM4);
    if (tim4_handler) {
      tim4_handler(tim4_ctx);
    }
  }
}

// 5
void TIM5_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM5) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM5);
    if (tim5_handler) {
      tim5_handler(tim5_ctx);
    }
  }
}

// 6
void TIM6_DAC_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM6) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM6);
    if (tim6_handler) {
      tim6_handler(tim6_ctx);
    }
  }
}

// 7
void TIM7_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM7) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM7);
    if (tim7_handler) {
      tim7_handler(tim7_ctx);
    }
  }
}

// 8, 13
void TIM8_UP_TIM13_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM8) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM8);
    if (tim8_handler) {
      tim8_handler(tim8_ctx);
    }
  }
  else if (LL_TIM_IsActiveFlag_UPDATE(TIM13) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM13);
    if (tim13_handler) {
      tim13_handler(tim13_ctx);
    }
  }

}

// 9
void TIM1_BRK_TIM9_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM9) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM9);
    if (tim9_handler) {
      tim9_handler(tim9_ctx);
    }
  }
}

// 11
void TIM1_TRG_COM_TIM11_IRQHandler(void) 
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM11) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM11);
    if (tim11_handler) {
      tim11_handler(tim11_ctx);
    }
  }
}


// 12
void TIM8_BRK_TIM12_IRQHandler(void) 
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM12) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM12);
    if (tim12_handler) {
      tim12_handler(tim12_ctx);
    }
  }
}


// 14
void TIM8_TRG_COM_TIM14_IRQHandler(void) 
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM14) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM14);
    if (tim14_handler) {
      tim14_handler(tim14_ctx);
    }
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


static void CalculateTimerValues(uint32_t timClk, uint32_t us, uint16_t* presc, uint16_t* arr) {

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

static void CalculateTimerValues_ns(uint32_t timClk, uint32_t ns, uint16_t* presc, uint16_t* arr) {

  uint64_t timerCount = (uint64_t)timClk * (uint64_t)ns / 1000000000llu;

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


void TIM_SetUpdatePeriod_us(TIM_TypeDef* tim, uint32_t us) {
  uint16_t presc, arr;

  uint32_t timClk = GetTIMx_CLK(IS_APB1(tim));

  // Here the perioud means how many times per second the interrupt is generated
  // So technically it is a half a period
  // So if it fires every second, it is a two-second period 
  // Divide by two to make it behave just like the PWM period
  CalculateTimerValues(timClk, us, &presc, &arr);
  // CalculateTimerValues(timClk / 2, us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(tim, presc);
  LL_TIM_SetAutoReload(tim, arr);
}

void TIM_SetUpdatePeriod_ns(TIM_TypeDef* tim, uint32_t ns) {
  uint16_t presc, arr;

  uint32_t timClk = GetTIMx_CLK(IS_APB1(tim));

  // Here the period means how many times per second the interrupt is generated
  // So technically it is a half a period
  // So if it fires every second, it is a two-second period 
  // Divide by two to make it behave just like the PWM period
  CalculateTimerValues_ns(timClk, ns, &presc, &arr);
  // CalculateTimerValues(timClk / 2, us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(tim, presc);
  LL_TIM_SetAutoReload(tim, arr);
}

static void _TIM_EnableClock(BOOL isAPB1, uint32_t periph) {
  if (isAPB1) {
    LL_APB1_GRP1_EnableClock(periph);
  } else {
    LL_APB2_GRP1_EnableClock(periph);
  }
}

static void _TIM_SetupCounter(TIM_TypeDef* tim, BOOL isAPB1, uint32_t periph, IRQn_Type IRQn, uint32_t us) {

  _TIM_EnableClock(isAPB1, periph);

  LL_TIM_SetCounterMode(tim, LL_TIM_COUNTERMODE_UP);

  TIM_SetUpdatePeriod_us(tim, us);

  //LL_TIM_GenerateEvent_UPDATE(tim);

  
  LL_TIM_EnableIT_UPDATE(tim);
  LL_TIM_ClearFlag_UPDATE(tim);
  LL_TIM_EnableCounter(tim);

  __NVIC_ClearPendingIRQ(IRQn);

  NVIC_SetPriority(IRQn, 0);
  NVIC_EnableIRQ(IRQn);

  // LL_TIM_EnableARRPreload(tim);
}

/*
void TIM_SetupCounterTIM1(uint32_t period_us, tim_handler th, void* ctx) {
  tim1_handler = th;
  tim1_ctx     = ctx;
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  _TIM_SetUpCounter(TIM1, LL_APB2_GRP1_PERIPH_TIM1, TIM1_UP_TIM10_IRQn, period_us);
}

void TIM_SetupCounterTIM2(uint32_t period_us, tim_handler th, void* ctx) {
  tim2_handler = th;
  tim2_ctx     = ctx;
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  _TIM_SetUpCounter(TIM2, LL_APB1_GRP1_PERIPH_TIM2, TIM2_IRQn, period_us);
}

void TIM_SetupCounterTIM3(uint32_t period_us, tim_handler th, void* ctx) {
  tim3_handler = th;
  tim3_ctx     = ctx;
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
  _TIM_SetUpCounter(TIM3, LL_APB1_GRP1_PERIPH_TIM3, TIM3_IRQn, period_us);
}

void TIM_SetupCounterTIM4(uint32_t period_us, tim_handler th, void* ctx) {
  tim4_handler = th;
  tim4_ctx     = ctx;
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
  _TIM_SetUpCounter(TIM4, LL_APB1_GRP1_PERIPH_TIM4, TIM4_IRQn, period_us);
}

void TIM_SetupCounterTIM5(uint32_t period_us, tim_handler th, void* ctx) {
  tim4_handler = th;
  tim4_ctx     = ctx;
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
  _TIM_SetUpCounter(TIM4, LL_APB1_GRP1_PERIPH_TIM4, TIM4_IRQn, period_us);
}
*/

void TIM_SetupCounter(TIM_TypeDef* timer, uint32_t period_us, tim_handler th, void* ctx) {
  if (timer == TIM1) {
    _TIM_SetupCounter(TIM1, FALSE, LL_APB2_GRP1_PERIPH_TIM1, TIM1_UP_TIM10_IRQn, period_us);
  } else if (timer == TIM2) {
    _TIM_SetupCounter(TIM2, TRUE, LL_APB1_GRP1_PERIPH_TIM2, TIM2_IRQn, period_us);
  } else if (timer == TIM3) {
    _TIM_SetupCounter(TIM3, TRUE, LL_APB1_GRP1_PERIPH_TIM3, TIM3_IRQn, period_us);
  } else if (timer == TIM4) {
    _TIM_SetupCounter(TIM4, TRUE, LL_APB1_GRP1_PERIPH_TIM4, TIM4_IRQn, period_us);
  } else if (timer == TIM5) {
    _TIM_SetupCounter(TIM5, TRUE, LL_APB1_GRP1_PERIPH_TIM5, TIM5_IRQn, period_us);
  } else if (timer == TIM6) {
    _TIM_SetupCounter(TIM6, TRUE, LL_APB1_GRP1_PERIPH_TIM6, TIM6_DAC_IRQn, period_us);
  } else if (timer == TIM7) {
    _TIM_SetupCounter(TIM7, TRUE, LL_APB1_GRP1_PERIPH_TIM7, TIM7_IRQn, period_us);
  } else if (timer == TIM8) {
    _TIM_SetupCounter(TIM8, FALSE, LL_APB2_GRP1_PERIPH_TIM8, TIM8_UP_TIM13_IRQn, period_us);
  } else if (timer == TIM9) {
    _TIM_SetupCounter(TIM9, FALSE, LL_APB2_GRP1_PERIPH_TIM9, TIM1_BRK_TIM9_IRQn, period_us);
  } else if (timer == TIM10) {
    _TIM_SetupCounter(TIM10, FALSE, LL_APB2_GRP1_PERIPH_TIM10, TIM1_UP_TIM10_IRQn, period_us);
  } else if (timer == TIM11) {
    _TIM_SetupCounter(TIM11, FALSE, LL_APB2_GRP1_PERIPH_TIM11, TIM1_TRG_COM_TIM11_IRQn, period_us);
  } else if (timer == TIM12) {
    _TIM_SetupCounter(TIM12, TRUE, LL_APB1_GRP1_PERIPH_TIM12, TIM8_BRK_TIM12_IRQn, period_us);
  } else if (timer == TIM13) {
    _TIM_SetupCounter(TIM13, TRUE, LL_APB1_GRP1_PERIPH_TIM13, TIM8_UP_TIM13_IRQn, period_us);
  } else if (timer == TIM14) {
    _TIM_SetupCounter(TIM14, TRUE, LL_APB1_GRP1_PERIPH_TIM14, TIM8_TRG_COM_TIM14_IRQn, period_us);
  }
  TIM_SetHandler(timer, th, ctx);
}

void TIM_SetHandler(TIM_TypeDef* timer, tim_handler th, void* ctx) {
  if (timer == TIM1) {
    tim1_handler = th;
    tim1_ctx     = ctx;
  } else if (timer == TIM2) {
    tim2_handler = th;
    tim2_ctx     = ctx;
  } else if (timer == TIM3) {
    tim3_handler = th;
    tim3_ctx     = ctx;
  } else if (timer == TIM4) {
    tim4_handler = th;
    tim4_ctx     = ctx;
  } else if (timer == TIM5) {
    tim5_handler = th;
    tim5_ctx     = ctx;
  } else if (timer == TIM6) {
    tim6_handler = th;
    tim6_ctx     = ctx;
  } else if (timer == TIM7) {
    tim7_handler = th;
    tim7_ctx     = ctx;
  } else if (timer == TIM8) {
    tim8_handler = th;
    tim8_ctx     = ctx;
  } else if (timer == TIM9) {
    tim9_handler = th;
    tim9_ctx     = ctx;
  } else if (timer == TIM10) {
    tim10_handler = th;
    tim11_ctx     = ctx;
  } else if (timer == TIM11) {
    tim11_handler = th;
    tim11_ctx     = ctx;
  } else if (timer == TIM12) {
    tim12_handler = th;
    tim12_ctx     = ctx;
  } else if (timer == TIM13) {
    tim13_handler = th;
    tim13_ctx     = ctx;
  } else if (timer == TIM14) {
    tim14_handler = th;
    tim14_ctx     = ctx;
  }
}

////////////////////////////////////////////////////////

static void _TIM_SetupPWM(TIM_TypeDef* tim, int channel, BOOL isAPB1, uint32_t periph, uint32_t period_us, uint32_t ds_us) {
  uint16_t presc, arr;

  _TIM_EnableClock(isAPB1, periph);

  uint32_t timClk = GetTIMx_CLK(isAPB1);
  CalculateTimerValues(timClk, period_us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(tim, presc);
  LL_TIM_SetAutoReload(tim, arr);

  uint16_t duty_count = (uint16_t)((float)arr * ((float)ds_us / (float)period_us));

  uint32_t tim_channel_mask = LL_TIM_CHANNEL_CH1;

  if (channel == 2) {
    tim_channel_mask = LL_TIM_CHANNEL_CH2;
  } else if (channel == 3) {
    tim_channel_mask = LL_TIM_CHANNEL_CH3;
  } else if (channel == 4) {
    tim_channel_mask = LL_TIM_CHANNEL_CH4;
  }

  LL_TIM_OC_InitTypeDef init;
  LL_TIM_OC_StructInit(&init);
  init.CompareValue = duty_count;
  init.OCMode = LL_TIM_OCMODE_PWM1;
  // init.OCPolarity   = LL_TIM_OCPOLARITY_HIGH;

  // break
  
  /*
  init.OCState      = LL_TIM_OCSTATE_DISABLE;
  init.OCNState     = LL_TIM_OCSTATE_DISABLE;
  init.OCNPolarity  = LL_TIM_OCPOLARITY_HIGH;
  init.OCIdleState  = LL_TIM_OCIDLESTATE_LOW;
  init.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
   */ 

  LL_TIM_OC_Init(tim, tim_channel_mask, &init);

  if (IS_TIM_BREAK_INSTANCE(tim)) {
    LL_TIM_EnableAllOutputs(tim);
  }

  // LL_TIM_OC_SetMode(tim, tim_channel_mask, LL_TIM_OCMODE_PWM1);

  // uint32_t cfg = LL_TIM_OCPOLARITY_HIGH | LL_TIM_OCIDLESTATE_LOW;
  // LL_TIM_OC_ConfigOutput(tim, tim_channel_mask, cfg);

  LL_TIM_OC_EnablePreload(tim, tim_channel_mask);
  LL_TIM_CC_EnableChannel(tim, tim_channel_mask);
  LL_TIM_EnableCounter(tim);
  LL_TIM_GenerateEvent_UPDATE(tim);
}

/*
void TIM_SetupPWM_TIM1(uint8_t channel, uint32_t period_us, uint32_t ds_us) {
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  TIM_SetupPWM(TIM1, channel, period_us, ds_us);
}

void TIM_SetupPWM_TIM2(uint8_t channel, uint32_t period_us, uint32_t ds_us) {
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  TIM_SetupPWM(TIM2, channel, period_us, ds_us);
}

TIM_Channel TIM_SetupPWM_TIM3(uint8_t channel, uint32_t period_us, uint32_t ds_us) {
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
  TIM_SetupPWM(TIM3, channel, period_us, ds_us);
  TIM_Channel ch = {TIM3, channel};
  return ch;
}
*/

TIM_Channel TIM_SetupPWM(TIM_TypeDef* tim, uint8_t channel, uint32_t period_us, uint32_t ds_us)
{
  if (tim == TIM1) {
    _TIM_SetupPWM(TIM1, channel, FALSE, LL_APB2_GRP1_PERIPH_TIM1, period_us, ds_us);
  } else if (tim == TIM2) {
    _TIM_SetupPWM(TIM2, channel, TRUE, LL_APB1_GRP1_PERIPH_TIM2, period_us, ds_us);
  } else if (tim == TIM3) {
    _TIM_SetupPWM(TIM3, channel, TRUE, LL_APB1_GRP1_PERIPH_TIM3, period_us, ds_us);
  } else if (tim == TIM4) {
    _TIM_SetupPWM(TIM4, channel, TRUE, LL_APB1_GRP1_PERIPH_TIM4, period_us, ds_us);
  } else if (tim == TIM5) {
    _TIM_SetupPWM(TIM5, channel, TRUE, LL_APB1_GRP1_PERIPH_TIM5, period_us, ds_us);
  } else if (tim == TIM6) {
    _TIM_SetupPWM(TIM6, channel, TRUE, LL_APB1_GRP1_PERIPH_TIM6, period_us, ds_us);
  } else if (tim == TIM7) {
    _TIM_SetupPWM(TIM7, channel, TRUE, LL_APB1_GRP1_PERIPH_TIM7, period_us, ds_us);
  } else if (tim == TIM8) {
    _TIM_SetupPWM(TIM8, channel, FALSE, LL_APB2_GRP1_PERIPH_TIM8, period_us, ds_us);
  } else if (tim == TIM9) {
    _TIM_SetupPWM(TIM9, channel, FALSE, LL_APB2_GRP1_PERIPH_TIM9, period_us, ds_us);
  } else if (tim == TIM10) {
    _TIM_SetupPWM(TIM10, channel, FALSE, LL_APB2_GRP1_PERIPH_TIM10, period_us, ds_us);
  } else if (tim == TIM11) {
    _TIM_SetupPWM(TIM11, channel, FALSE, LL_APB2_GRP1_PERIPH_TIM11, period_us, ds_us);
  } else if (tim == TIM12) {
    _TIM_SetupPWM(TIM12, channel, TRUE, LL_APB1_GRP1_PERIPH_TIM12, period_us, ds_us);
  } else if (tim == TIM13) {
    _TIM_SetupPWM(TIM13, channel, TRUE, LL_APB1_GRP1_PERIPH_TIM13, period_us, ds_us);
  } else if (tim == TIM14) {
    _TIM_SetupPWM(TIM14, channel, TRUE, LL_APB1_GRP1_PERIPH_TIM14, period_us, ds_us);
  }

  TIM_Channel ch = {tim, channel};
  return ch;
}

void TIM_UpdatePeriodDs(TIM_Channel* ch, uint32_t period_us, uint32_t ds_us) {

  uint16_t presc, arr;
  uint32_t timClk = GetTIMx_CLK(IS_APB1(ch->tim));

  CalculateTimerValues(timClk, period_us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(ch->tim, presc);
  LL_TIM_SetAutoReload(ch->tim, arr);

  TIM_UpdateDs(ch, ds_us);
}


void TIM_UpdateDs(TIM_Channel* ch, uint32_t ds_us) {
  uint32_t presc = LL_TIM_GetPrescaler(ch->tim);
  int isAPB1     = ((uint32_t)ch->tim < APB2PERIPH_BASE);
  uint32_t t_clk = GetTIMx_CLK(isAPB1);

  uint16_t ccr_val = (uint64_t)ds_us * (uint64_t)t_clk / 1000000ull / (uint64_t)(presc + 1);
  if (ch->channel == 1) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH1(ch->tim);
    LL_TIM_OC_SetCompareCH1(ch->tim, ccr_val);
  } else if (ch->channel == 2) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH2(ch->tim);
    LL_TIM_OC_SetCompareCH2(ch->tim, ccr_val);
  } else if (ch->channel == 3) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH3(ch->tim);
    LL_TIM_OC_SetCompareCH3(ch->tim, ccr_val);
  } else if (ch->channel == 4) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH4(ch->tim);
    LL_TIM_OC_SetCompareCH4(ch->tim, ccr_val);
  }
}

#ifdef TIM_HELPERS

void TIM_SetupPWM_TIM1_A8(uint32_t period_us, uint32_t ds_us) {
  GPIO_PIN pin1 = GPIO_GetPin("A8");    // TIM1 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM1(1, period_us, ds_us);

void TIM_SetupPWM_TIM1_A9(uint32_t period_us, uint32_t ds_us) {
  GPIO_PIN pin1 = GPIO_GetPin("A9");    // TIM1 channel 2
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM1(2, period_us, ds_us);
}

void TIM_SetupPWM_TIM1_A10(uint32_t period_us, uint32_t ds_us) {
  GPIO_PIN pin1 = GPIO_GetPin("A10");    // TIM1 channel 3
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM1(3, period_us, ds_us);
}

void TIM_SetupPWM_TIM1_A11(uint32_t period_us, uint32_t ds_us) {
  GPIO_PIN pin1 = GPIO_GetPin("A11");    // TIM1 channel 4
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM1(4, period_us, ds_us);
}

// TIM2
void TIM_SetupPWM_TIM2_A0(uint32_t period_us, uint32_t ds_us) {
  GPIO_PIN pin1 = GPIO_GetPin("A0");    // TIM2 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM2(1, period_us, ds_us);
}

void TIM_SetupPWM_TIM2_A1(uint32_t period_us, uint32_t ds_us) {
  GPIO_PIN pin1 = GPIO_GetPin("A1");    // TIM2 channel 2
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM2(2, period_us, ds_us);
}

void TIM_SetupPWM_TIM2_A2(uint32_t period_us, uint32_t ds_us) {
  GPIO_PIN pin1 = GPIO_GetPin("A2");    // TIM2 channel 3
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM2(3, period_us, ds_us);
}

void TIM_SetupPWM_TIM2_A3(uint32_t period_us, uint32_t ds_us) {
  GPIO_PIN pin1 = GPIO_GetPin("A3");    // TIM2 channel 4
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM2(4, period_us, ds_us);
}

void TIM_SetupPWM_TIM2_A15(uint32_t period_us, uint32_t ds_us) {
  // A15 is JTDI
  LL_GPIO_AF_Remap_SWJ_NOJTAG();
  LL_GPIO_AF_EnableRemap_TIM2();
  GPIO_PIN pin1 = GPIO_GetPin("A15");    // TIM2 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM2(1, period_us, ds_us);
}

void TIM_SetupPWM_TIM2_B3(uint32_t period_us, uint32_t ds_us) {
  // B3 is JTDO
  LL_GPIO_AF_Remap_SWJ_NOJTAG();
  LL_GPIO_AF_EnableRemap_TIM2();
  GPIO_PIN pin1 = GPIO_GetPin("B3");    // TIM2 channel 2
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM2(2, period_us, ds_us);
}

void TIM_SetupPWM_TIM2_B10(uint32_t period_us, uint32_t ds_us) {
  LL_GPIO_AF_EnableRemap_TIM2();
  GPIO_PIN pin1 = GPIO_GetPin("B10");    // TIM2 channel 3
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM2(3, period_us, ds_us);
}

void TIM_SetupPWM_TIM2_B11(uint32_t period_us, uint32_t ds_us) {
  LL_GPIO_AF_EnableRemap_TIM2();
  GPIO_PIN pin1 = GPIO_GetPin("B11");    // TIM2 channel 4
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM2(4, period_us, ds_us);
}

void TIM_SetupPWM_TIM3_A6(uint32_t period_us, uint32_t ds_us)
{
  GPIO_PIN pin1 = GPIO_GetPin("A6");    // TIM3 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM3(1, period_us, ds_us);
}

void TIM_SetupPWM_TIM3_A7(uint32_t period_us, uint32_t ds_us)
{
  GPIO_PIN pin1 = GPIO_GetPin("A7");    // TIM3 channel 2
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM3(2, period_us, ds_us);
}

void TIM_SetupPWM_TIM3_B0(uint32_t period_us, uint32_t ds_us)
{
  GPIO_PIN pin1 = GPIO_GetPin("B0");    // TIM3 channel 3
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM3(3, period_us, ds_us);
}

void TIM_SetupPWM_TIM3_B1(uint32_t period_us, uint32_t ds_us)
{
  GPIO_PIN pin1 = GPIO_GetPin("B1");    // TIM3 channel 4
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM3(4, period_us, ds_us);
}


void TIM_SetupPWM_TIM3_C6(uint32_t period_us, uint32_t ds_us)
{
  LL_GPIO_AF_EnableRemap_TIM3();
  GPIO_PIN pin1 = GPIO_GetPin("C6");    // TIM3 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM3(1, period_us, ds_us);
}

void TIM_SetupPWM_TIM3_C7(uint32_t period_us, uint32_t ds_us)
{
  LL_GPIO_AF_EnableRemap_TIM3();
  GPIO_PIN pin1 = GPIO_GetPin("C7");    // TIM3 channel 2
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM3(2, period_us, ds_us);
}

void TIM_SetupPWM_TIM3_C8(uint32_t period_us, uint32_t ds_us)
{
  LL_GPIO_AF_EnableRemap_TIM3();
  GPIO_PIN pin1 = GPIO_GetPin("C8");    // TIM3 channel 3
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM3(3, period_us, ds_us);
}

void TIM_SetupPWM_TIM3_C9(uint32_t period_us, uint32_t ds_us)
{
  LL_GPIO_AF_EnableRemap_TIM3();
  GPIO_PIN pin1 = GPIO_GetPin("C9");    // TIM3 channel 4
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM3(4, period_us, ds_us);
}

void TIM_SetupPWM_TIM3_B4(uint32_t period_us, uint32_t ds_us)
{
  LL_GPIO_AF_Remap_SWJ_NONJTRST();
  LL_GPIO_AF_RemapPartial_TIM3();
  GPIO_PIN pin1 = GPIO_GetPin("B4");    // TIM3 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM3(1, period_us, ds_us);
}

void TIM_SetupPWM_TIM3_B5(uint32_t period_us, uint32_t ds_us)
{
  LL_GPIO_AF_RemapPartial_TIM3();
  GPIO_PIN pin1 = GPIO_GetPin("B5");    // TIM3 channel 2
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM3(2, period_us, ds_us);
}


void TIM_SetupPWM_OnPin(const char* pin_name, uint32_t period_us, uint32_t ds_us) {
  if (!strcasecmp(pin_name, "A8")) {
    TIM_SetupPWM_TIM1_A8(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "A9")) {
    TIM_SetupPWM_TIM1_A9(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "A10")) {
    TIM_SetupPWM_TIM1_A10(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "A11")) {
    TIM_SetupPWM_TIM1_A11(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "A0")) {
    TIM_SetupPWM_TIM2_A0(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "A1")) {
    TIM_SetupPWM_TIM2_A1(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "A2")) {
    TIM_SetupPWM_TIM2_A2(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "A3")) {
    TIM_SetupPWM_TIM2_A3(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "A15")) {
    TIM_SetupPWM_TIM2_A15(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "B3")) {
    TIM_SetupPWM_TIM2_B3(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "B10")) {
    TIM_SetupPWM_TIM2_B10(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "B11")) {
    TIM_SetupPWM_TIM2_B11(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "A6")) {
    TIM_SetupPWM_TIM3_A6(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "A7")) {
    TIM_SetupPWM_TIM3_A7(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "B0")) {
    TIM_SetupPWM_TIM3_B0(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "B1")) {
    TIM_SetupPWM_TIM3_B1(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "C6")) {
    TIM_SetupPWM_TIM3_C6(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "C7")) {
    TIM_SetupPWM_TIM3_C7(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "C8")) {
    TIM_SetupPWM_TIM3_C8(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "C9")) {
    TIM_SetupPWM_TIM3_C9(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "B4")) {
    TIM_SetupPWM_TIM3_B4(period_us, ds_us);
  } else if (!strcasecmp(pin_name, "B5")) {
    TIM_SetupPWM_TIM3_B5(period_us, ds_us);
  }
}
#endif