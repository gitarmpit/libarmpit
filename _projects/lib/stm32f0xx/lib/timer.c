#if 0
#include "timer.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_tim.h"

tim_handler tim1_handler = 0;
tim_handler tim2_handler = 0;
tim_handler tim3_handler = 0;
tim_handler tim4_handler = 0;

static void* tim1_ctx = 0;
static void* tim2_ctx = 0;
static void* tim3_ctx = 0;
static void* tim4_ctx = 0;

void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM1) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM1);
    if (tim1_handler) {
      tim1_handler(tim1_ctx);
    }
  }
}


void TIM3_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM3) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM3);
    if (tim3_handler) {
      tim3_handler(tim3_ctx);
    }
  }
}


static uint32_t GetTIMx_CLK(void) {
  LL_RCC_ClocksTypeDef RCC_Clocks;
  LL_RCC_GetSystemClocksFreq(&RCC_Clocks);

  uint8_t apb_presc;
  apb_presc = 1 << (APBPrescTable[LL_RCC_GetAPB1Prescaler() >> RCC_CFGR_PPRE_Pos]);

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

  uint32_t timClk = GetTIMx_CLK();

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


static void _TIM_EnableClock(BOOL isAPB1, uint32_t periph) {
  if (isAPB1) {
    LL_APB1_GRP1_EnableClock(periph);
  } else {
    LL_APB1_GRP2_EnableClock(periph);
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

void TIM_SetupCounter(TIM_TypeDef* tim, uint32_t period_us, tim_handler th, void* ctx) {
  if (tim == TIM1) {
    _TIM_SetupCounter(TIM1, FALSE, LL_APB1_GRP2_PERIPH_TIM1, TIM1_BRK_UP_TRG_COM_IRQn, period_us);
  } else if (tim == TIM3) {
    _TIM_SetupCounter(TIM3, TRUE, LL_APB1_GRP1_PERIPH_TIM3, TIM3_IRQn, period_us);
  }
  TIM_SetHandler(tim, th, ctx);
}

void TIM_SetHandler(TIM_TypeDef* tim, tim_handler th, void* ctx) {
  if (tim == TIM1) {
    tim1_handler = th;
    tim1_ctx     = ctx;
  } else if (tim == TIM3) {
    tim3_handler = th;
    tim3_ctx     = ctx;
  }
}

////////////////////////////////////////////////////////////////////////////////
// PWM


static void _TIM_SetupPWM(TIM_TypeDef* tim, int channel, BOOL isAPB1, uint32_t periph, uint32_t period_us, uint32_t ds_us) {
  uint16_t presc, arr;

  _TIM_EnableClock(isAPB1, periph);

  uint32_t timClk = GetTIMx_CLK();
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

TIM_Channel TIM_SetupPWM(TIM_TypeDef* tim, uint8_t channel, uint32_t period_us, uint32_t ds_us)
{
  if (tim == TIM1) {
    _TIM_SetupPWM(TIM1, channel, FALSE, LL_APB1_GRP2_PERIPH_TIM1, period_us, ds_us);
  } else if (tim == TIM3) {
    _TIM_SetupPWM(TIM3, channel, TRUE, LL_APB1_GRP1_PERIPH_TIM3, period_us, ds_us);
  }

  TIM_Channel ch = {tim, channel};
  return ch;
}



void TIM_UpdatePeriodDs(TIM_Channel* ch, uint32_t period_us, uint32_t ds_us) {

  uint16_t presc, arr;
  uint32_t timClk = GetTIMx_CLK();

  CalculateTimerValues(timClk, period_us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(ch->tim, presc);
  LL_TIM_SetAutoReload(ch->tim, arr);

  TIM_UpdateDs(ch, ds_us);
}


void TIM_UpdateDs(TIM_Channel* ch, uint32_t ds_us) {
  uint32_t presc = LL_TIM_GetPrescaler(ch->tim);
  uint32_t t_clk = GetTIMx_CLK();

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

#endif