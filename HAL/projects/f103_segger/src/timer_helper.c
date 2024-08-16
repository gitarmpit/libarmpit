#include "timer_helper.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_tim.h"

tim_handler tim1_handler = 0;
tim_handler tim2_handler = 0;
tim_handler tim3_handler = 0;

static void* tim1_ctx = 0;
static void* tim2_ctx = 0;
static void* tim3_ctx = 0;

void TIM1_UP_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM1) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM1);
    if (tim1_handler) {
      tim1_handler(tim1_ctx);
    }
  }
}

void TIM2_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM2);
    if (tim2_handler) {
      tim2_handler(tim2_ctx);
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

void TIM_SetUpdatePeriod_us(TIM_TypeDef* timer, uint32_t us) {
  uint32_t addr = (uint32_t)timer;
  int isAPB1    = (addr < APB2PERIPH_BASE);
  uint16_t presc, arr;

  uint32_t timClk = GetTIMx_CLK(isAPB1);

  CalculateTimerValues(timClk / 2, us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(timer, presc);
  LL_TIM_SetAutoReload(timer, arr);
}

static void _TIM_SetUpCounter(TIM_TypeDef* tim, uint32_t periph, IRQn_Type IRQn, uint32_t us) {
  LL_TIM_SetCounterMode(tim, LL_TIM_COUNTERMODE_UP);
  LL_TIM_EnableARRPreload(tim);

  TIM_SetUpdatePeriod_us(tim, us);

  LL_TIM_EnableIT_UPDATE(tim);

  NVIC_SetPriority(IRQn, 0);
  NVIC_EnableIRQ(IRQn);

  LL_TIM_EnableCounter(tim);
}

void TIM_SetupCounterTIM1(uint32_t period_us, tim_handler th, void* ctx) {
  tim1_handler = th;
  tim1_ctx     = ctx;
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  _TIM_SetUpCounter(TIM1, LL_APB2_GRP1_PERIPH_TIM1, TIM1_UP_IRQn, period_us);
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

void TIM_SetupCounter(TIM_TypeDef* timer, uint32_t period_us, tim_handler th, void* ctx) {
  if (timer == TIM1) {
    TIM_SetupCounterTIM1(period_us, th, ctx);
  } else if (timer == TIM2) {
    TIM_SetupCounterTIM2(period_us, th, ctx);
  } else if (timer == TIM3) {
    TIM_SetupCounterTIM3(period_us, th, ctx);
  }
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
  }
}

void TIM_SetupPWM(TIM_TypeDef* tim, int channel, uint32_t period_us, uint32_t ds_us) {
  uint32_t addr = (uint32_t)tim;
  int isAPB1    = (addr < APB2PERIPH_BASE);
  uint16_t presc, arr;

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

void TIM_UpdateDs(TIM_Channel* ch, uint32_t ds_us) {
  uint32_t presc = LL_TIM_GetPrescaler(ch->timer);
  int isAPB1     = ((uint32_t)ch->timer < APB2PERIPH_BASE);
  uint32_t t_clk = GetTIMx_CLK(isAPB1);

  uint16_t ccr_val = (uint64_t)ds_us * (uint64_t)t_clk / 1000000ull / (uint64_t)(presc + 1);
  if (ch->channel == 1) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH1(ch->timer);
    LL_TIM_OC_SetCompareCH1(ch->timer, ccr_val);
  } else if (ch->channel == 2) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH2(ch->timer);
    LL_TIM_OC_SetCompareCH2(ch->timer, ccr_val);
  } else if (ch->channel == 3) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH3(ch->timer);
    LL_TIM_OC_SetCompareCH3(ch->timer, ccr_val);
  } else if (ch->channel == 4) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH4(ch->timer);
    LL_TIM_OC_SetCompareCH4(ch->timer, ccr_val);
  }
}

/*
void TIM_SetupPWM_TIM1_A8(uint32_t period_us, uint32_t ds_us) {
  GPIO_PIN pin1 = GPIO_GetPin("A8");    // TIM1 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM1(1, period_us, ds_us);
}

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
*/