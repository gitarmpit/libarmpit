#include "timer_cpp.h"
#include "stm32f1xx_ll_rcc.h"

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


void TIM1_UP_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM1) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM1);
    TIMER1::GetInstance()->HandleInterrupt();
  }
}

void TIM2_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM2);
    TIMER2::GetInstance()->HandleInterrupt();
  }
}

void TIM3_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM3) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM3);
    TIMER3::GetInstance()->HandleInterrupt();
  }
}

void TIM4_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM4) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM4);
    TIMER4::GetInstance()->HandleInterrupt();
  }
}

#endif


}

/********************************************************/

uint32_t TIMER::GetTIMx_CLK() {
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

void TIMER::CalculateTimerValues(uint32_t timClk, uint32_t us, uint16_t* presc, uint16_t* arr) {
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

void TIMER::SetUpdatePeriod_us(uint32_t us) {
  uint16_t presc, arr;

  uint32_t timClk = GetTIMx_CLK();

  CalculateTimerValues(timClk / 2, us, &presc, &arr);
  // LL_TIM_SetClockDivision(timer, LL_TIM_CLOCKDIVISION_DIV2);

  LL_TIM_SetPrescaler(_tim, presc);
  LL_TIM_SetAutoReload(_tim, arr);
}

void TIMER::SetupCounter(uint32_t period_us) {

  if (_isAPB1) {
    LL_APB1_GRP1_EnableClock(_periph);
  }
  else {
    LL_APB2_GRP1_EnableClock(_periph);
  }

  LL_TIM_SetCounterMode(_tim, LL_TIM_COUNTERMODE_UP);
  LL_TIM_EnableARRPreload(_tim);

  SetUpdatePeriod_us(period_us);

  LL_TIM_EnableIT_UPDATE(_tim);

  NVIC_SetPriority(_IRQn, _IRQ_priority);
  NVIC_EnableIRQ(_IRQn);

  LL_TIM_EnableCounter(_tim);
}

TIM_Channel TIMER::SetupPWM(uint8_t channel, uint32_t period_us, uint32_t ds_us) {

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

void TIMER::EnableCounter() {
  LL_TIM_EnableCounter(_tim);
}

void TIMER::DisableCounter() {
  LL_TIM_DisableCounter(_tim);
}


void TIMER::UpdateDs(uint8_t channel, uint32_t ds_us) {
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
