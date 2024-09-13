#include "config.h"
#include "gpio.h"
#include "timer.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

GPIO_PIN* led;

class TestInterruptHandler : public TIMER_Interrupt_Handler {
public:
  TestInterruptHandler(TIM_Channel* ch, uint16_t period) : 
  _ch (ch), _period(period)
  {
    _max = 20000 / period;
    _step = 180.0 / _max * 2.0;
    _cnt = 0;
    _table = (double*)malloc (_max * sizeof(double));
    for (int i = 0; i < _max; ++i) {
      float ds = 1.0 + sin (_step*i*M_PI/180. - M_PI/2.0);
      _table[i] = _period*ds/2.0;
      //printf ("%d %f %f\n", i, ds, _table[i]);
    }
  }

  virtual void HandleInterrupt()  {
    
    _ch->timer->UpdateDs(_ch->channel, _table[_cnt]);

    if (++_cnt >= _max) {
      _cnt = 0;
    }

    // GPIO_TogglePin(led);

  }

  virtual void HandleInterrupt2()  {
    
    static int inc = 1;
    static int cnt = 1;
    float ds = 1.0 + sin (18.0*_cnt*M_PI/180. - M_PI/2.0);
    //printf ("%d %f\n", _cnt, 500*ds);
    // _ch->timer->UpdateDs(_ch->channel, cnt * 100);
    _ch->timer->UpdateDs(_ch->channel, 500*ds);
    
    /*
    cnt += inc;

    if (cnt == 10 || cnt == 0) {
      inc = -inc;
    }
    */

    if (++_cnt >= _max) {
      _cnt = 0;
    }

  }

private:
  TIM_Channel* _ch;
  uint16_t _period;
  uint32_t _cnt;
  uint32_t _max;
  double _step;
  double* _table;
};


static GPIO_PIN SetupPWMPin(const char* pinName, uint8_t AF) {
  GPIO_PIN pin = GPIO_GetPin(pinName);
  GPIO_Setup_OutAltPP(&pin);
  GPIO_SetAF(&pin, AF);
  return pin;
}


static void MX_TIM1_Init(void)
{

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);


  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM1, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_FROZEN;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
  TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
  LL_TIM_SetTriggerOutput2(TIM1, LL_TIM_TRGO2_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM1);
  TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_DISABLE;
  TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_DISABLE;
  TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
  TIM_BDTRInitStruct.DeadTime = 0;
  TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
  TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
  TIM_BDTRInitStruct.BreakFilter = LL_TIM_BREAK_FILTER_FDIV1;
  TIM_BDTRInitStruct.Break2State = LL_TIM_BREAK2_DISABLE;
  TIM_BDTRInitStruct.Break2Polarity = LL_TIM_BREAK2_POLARITY_HIGH;
  TIM_BDTRInitStruct.Break2Filter = LL_TIM_BREAK2_FILTER_FDIV1;
  TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_DISABLE;
  LL_TIM_BDTR_Init(TIM1, &TIM_BDTRInitStruct);
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**TIM1 GPIO Configuration
  PB13   ------> TIM1_CH1N
  PA8   ------> TIM1_CH1
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}


static void testPWM2() {

  GPIO_PIN a8 = GPIO_GetPin("A8");
  GPIO_Setup_OutAltPP(&a8);
  GPIO_SetAF(&a8, 1);

  // Complimentary PB13 TIM1 CH1 
  GPIO_PIN b13 = GPIO_GetPin("B13");
  GPIO_Setup_OutAltPP(&b13);
  GPIO_SetAF(&b13, 1);

  TIM* pwm = TIMER1::GetInstance();
  int period = 20;
  TIM_Channel ch = pwm->SetupPWM(1, period, 5);

  /*
  TestInterruptHandler handler (&ch, period);
  TIM* t4 = TIMER4::GetInstance();
  t4->SetInterruptHandler(&handler);
  t4->SetupCounter(period);
  */

  while(1)
    ;
}

static void testPWM3() {

  // TIM8 CH1
  GPIO_PIN c6 = GPIO_GetPin("C6");
  GPIO_Setup_OutAltPP(&c6);
  GPIO_SetAF(&c6, 3);

  // Complimentary 
  GPIO_PIN a5 = GPIO_GetPin("A5");
  GPIO_Setup_OutAltPP(&a5);
  GPIO_SetAF(&a5, 3);

  TIM* pwm = TIMER8::GetInstance();
  int period = 20;
  TIM_Channel ch = pwm->SetupPWM(1, period, 5, true, 0x2f);

  /*
  TestInterruptHandler handler (&ch, period);
  TIM* t4 = TIMER4::GetInstance();
  t4->SetInterruptHandler(&handler);
  t4->SetupCounter(period);
  */

  while(1)
    ;
}


static void test2() {
  
  GPIO_PIN a8 = GPIO_GetPin("A8");
  GPIO_Setup_OutPP(&a8);
  led = &a8;

  int period = 100;
  TestInterruptHandler handler (0, period);
  TIM* t4 = TIMER4::GetInstance();
  t4->SetInterruptHandler(&handler);
  t4->SetupCounter(period);
  while(1)
    ;
}


int main(void) {
  System_Config();
  BOOL enableBypass = FALSE;
  SystemClock_Config_HSE(enableBypass);
  // SystemClock_Config_HSI();

  testPWM3();
  //MX_TIM1_Init();

  while (1) {
  }
}
