#include "timer.h"
#include "stm32f7xx_ll_rcc.h"

extern "C" {

tim_handler __tim1_handler = 0;
tim_handler __tim2_handler = 0;
tim_handler __tim3_handler = 0;
tim_handler __tim4_handler = 0;

static void* tim1_ctx = 0;
static void* tim2_ctx = 0;
static void* tim3_ctx = 0;
static void* tim4_ctx = 0;

#ifdef TIM_DIRECT_INTERRUPT 

void TIM1_UP_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM1) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM1);
    if (__tim1_handler) {
      __tim1_handler(tim1_ctx);
    }
  }
}

void TIM2_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM2);
    if (__tim2_handler) {
      __tim2_handler(tim2_ctx);
    }
  }
}

void TIM3_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM3) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM3);
    if (__tim3_handler) {
      __tim3_handler(tim3_ctx);
    }
  }
}

void TIM4_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM3) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM3);
    if (__tim4_handler) {
      __tim4_handler(tim4_ctx);
    }
  }
}


#else


void TIM1_UP_TIM10_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM1) && LL_TIM_IsEnabledIT_UPDATE(TIM1)) {
    LL_TIM_ClearFlag_UPDATE(TIM1);
    TIMER1::GetInstance()->HandleInterrupt();
  } else if (LL_TIM_IsActiveFlag_UPDATE(TIM10) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM10);
    TIMER10::GetInstance()->HandleInterrupt();
  }

}

// 2
void TIM2_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM2);
    TIMER2::GetInstance()->HandleInterrupt();
  }
}

// 3
void TIM3_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM3) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM3);
    TIMER3::GetInstance()->HandleInterrupt();
  }
}

// 4
void TIM4_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM4) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM4);
    TIMER4::GetInstance()->HandleInterrupt();
  }
}

// 5
void TIM5_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM5) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM5);
    TIMER5::GetInstance()->HandleInterrupt();
  }
}

// 6
void TIM6_DAC_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM6) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM6);
    TIMER6::GetInstance()->HandleInterrupt();
  }
}

// 7
void TIM7_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM7) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM7);
    TIMER7::GetInstance()->HandleInterrupt();
  }
}

// 8, 13
void TIM8_UP_TIM13_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM8) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM8);
    TIMER8::GetInstance()->HandleInterrupt();
  }
  else if (LL_TIM_IsActiveFlag_UPDATE(TIM13) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM13);
    TIMER13::GetInstance()->HandleInterrupt();
  }

}

// 9
void TIM1_BRK_TIM9_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM9) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM9);
    TIMER9::GetInstance()->HandleInterrupt();
  }
}

// 11
void TIM1_TRG_COM_TIM11_IRQHandler(void) 
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM11) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM11);
    TIMER11::GetInstance()->HandleInterrupt();
  }
}


// 12
void TIM8_BRK_TIM12_IRQHandler(void) 
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM12) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM12);
    TIMER12::GetInstance()->HandleInterrupt();
  }
}


// 14
void TIM8_TRG_COM_TIM14_IRQHandler(void) 
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM14) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM14);
    TIMER14::GetInstance()->HandleInterrupt();
  }
}


#endif


}

/********************************************************/

uint32_t TIM::GetTIMx_CLK() {
  LL_RCC_ClocksTypeDef RCC_Clocks;
  LL_RCC_GetSystemClocksFreq(&RCC_Clocks);

  uint8_t apb_presc;
  if (_isAPB1) {
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

void TIM::CalculateTimerValues(uint32_t timClk, uint32_t us, uint16_t* presc, uint16_t* arr) {
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

void TIM::CalculateTimerValues_ns(uint32_t timClk, uint32_t ns, uint16_t* presc, uint16_t* arr) {

  uint64_t timerCount = (uint64_t) ((float)timClk * (float)ns / 1000000000.0 + 0.5);

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


void TIM::SetUpdatePeriod_us(uint32_t us) {
  uint16_t presc, arr;

  uint32_t timClk = GetTIMx_CLK();

  // Here the perioud means how many times per second the interrupt is generated
  // So technically it is a half a period
  // So if it fires every second, it is a two-second period 
  // Divide by two to make it behave just like the PWM period
  CalculateTimerValues(timClk, us, &presc, &arr);
  // CalculateTimerValues(timClk / 2, us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(_tim, presc);
  LL_TIM_SetAutoReload(_tim, arr);
}

void TIM::SetupCounter(uint32_t period_us) {

  if (_isAPB1) {
    LL_APB1_GRP1_EnableClock(_periph);
  }
  else {
    LL_APB2_GRP1_EnableClock(_periph);
  }

  LL_TIM_SetCounterMode(_tim, LL_TIM_COUNTERMODE_UP);


  SetUpdatePeriod_us(period_us);
  LL_TIM_EnableARRPreload(_tim);

  LL_TIM_GenerateEvent_UPDATE(_tim);


  LL_TIM_EnableIT_UPDATE(_tim);

  NVIC_SetPriority(_IRQn, _IRQ_priority);
  NVIC_EnableIRQ(_IRQn);

  LL_TIM_EnableCounter(_tim);
}

TIM_Channel TIM::SetupPWM(uint8_t channel, uint32_t period_us, uint32_t ds_us) {

  if (_isAPB1) {
    LL_APB1_GRP1_EnableClock(_periph);
  }
  else {
    LL_APB2_GRP1_EnableClock(_periph);
  }

  uint16_t presc, arr;
  uint32_t timClk = GetTIMx_CLK();

  CalculateTimerValues(timClk, period_us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(_tim, presc);
  LL_TIM_SetAutoReload(_tim, arr);

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
  init.OCMode       = LL_TIM_OCMODE_PWM1;
  // init.OCPolarity   = LL_TIM_OCPOLARITY_HIGH;

  // break

  /*
  init.OCState      = LL_TIM_OCSTATE_DISABLE;
  init.OCNState     = LL_TIM_OCSTATE_DISABLE;
  init.OCNPolarity  = LL_TIM_OCPOLARITY_HIGH;
  init.OCIdleState  = LL_TIM_OCIDLESTATE_LOW;
  init.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
   */

  LL_TIM_OC_Init(_tim, tim_channel_mask, &init);

  if (IS_TIM_BREAK_INSTANCE(_tim)) {
    LL_TIM_EnableAllOutputs(_tim);
  }

  // LL_TIM_OC_SetMode(tim, tim_channel_mask, LL_TIM_OCMODE_PWM1);

  // uint32_t cfg = LL_TIM_OCPOLARITY_HIGH | LL_TIM_OCIDLESTATE_LOW;
  // LL_TIM_OC_ConfigOutput(tim, tim_channel_mask, cfg);

  LL_TIM_OC_EnablePreload(_tim, tim_channel_mask);
  LL_TIM_CC_EnableChannel(_tim, tim_channel_mask);
  LL_TIM_EnableCounter(_tim);
  LL_TIM_GenerateEvent_UPDATE(_tim);

  TIM_Channel ch = {this, channel};
  return ch;
}

void TIM::EnableCounter() {
  LL_TIM_EnableCounter(_tim);
}

void TIM::DisableCounter() {
  LL_TIM_DisableCounter(_tim);
}

void TIM::UpdatePeriodDs(uint8_t channel, uint32_t period_us, uint32_t ds_us) {

  uint16_t presc, arr;
  uint32_t timClk = GetTIMx_CLK();

  CalculateTimerValues(timClk, period_us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(_tim, presc);
  LL_TIM_SetAutoReload(_tim, arr);
  UpdateDs (channel, ds_us);
  
}


void TIM::UpdateDs(uint8_t channel, uint32_t ds_us) {
  uint32_t presc = LL_TIM_GetPrescaler(_tim);
  uint32_t t_clk = GetTIMx_CLK();

  uint16_t ccr_val = (uint64_t)ds_us * (uint64_t)t_clk / 1000000ull / (uint64_t)(presc + 1);
  if (channel == 1) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH1(_tim);
    LL_TIM_OC_SetCompareCH1(_tim, ccr_val);
  } else if (channel == 2) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH2(_tim);
    LL_TIM_OC_SetCompareCH2(_tim, ccr_val);
  } else if (channel == 3) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH3(_tim);
    LL_TIM_OC_SetCompareCH3(_tim, ccr_val);
  } else if (channel == 4) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH4(_tim);
    LL_TIM_OC_SetCompareCH4(_tim, ccr_val);
  }
}

void TIM::UpdatePeriodDs_ns(uint8_t channel, uint32_t period_ns, uint32_t ds_ns) {

  uint16_t presc, arr;
  uint32_t timClk = GetTIMx_CLK();

  CalculateTimerValues_ns(timClk, period_ns, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(_tim, presc);
  LL_TIM_SetAutoReload(_tim, arr);

  UpdateDs_ns(channel, ds_ns);
}


void TIM::UpdateDs_ns(uint8_t channel, uint32_t ds_ns) {
  uint32_t presc = LL_TIM_GetPrescaler(_tim);
  uint32_t t_clk = GetTIMx_CLK();

  uint16_t ccr_val = (uint64_t)((float)ds_ns * (float)t_clk / 1000000000.0 / (float)(presc + 1) + 0.5);

  if (channel == 1) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH1(_tim);
    LL_TIM_OC_SetCompareCH1(_tim, ccr_val);
  } else if (channel == 2) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH2(_tim);
    LL_TIM_OC_SetCompareCH2(_tim, ccr_val);
  } else if (channel == 3) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH3(_tim);
    LL_TIM_OC_SetCompareCH3(_tim, ccr_val);
  } else if (channel == 4) {
    uint32_t tmp = LL_TIM_OC_GetCompareCH4(_tim);
    LL_TIM_OC_SetCompareCH4(_tim, ccr_val);
  }
}

