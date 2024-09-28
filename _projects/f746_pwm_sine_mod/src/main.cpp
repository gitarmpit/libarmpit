#include "config.h"
#include "gpio.h"
#include "timer.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "systick.h"

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
    int presc = 1;
    uint32_t t_clk = 216000000;
    for (int i = 0; i < _max; ++i) {
      float s = 1.0 + sin (_step*i*M_PI/180. - M_PI/2.0);
      float ds = _period*s/2.0;
      uint16_t ccr_val = (uint64_t)ds * (uint64_t)t_clk / 1000000ull / (uint64_t)(presc + 1);
      uint16_t ccr_val_ns = (uint64_t)((float)ds * 1000 * (float)t_clk / 1000000000.0 / (float)(presc + 1) + 0.5);

      _table[i] = ds * 1000;
      printf ("%d %f %f %d %d\n", i, s, _table[i], ccr_val, ccr_val_ns);
      SysTick_Delay(1);
    }
  }

  virtual void HandleInterrupt()  {
    
    _ch->timer->UpdateDs_ns(_ch->channel, _table[_cnt]);

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

  TestInterruptHandler handler (&ch, period);
  TIM* t4 = TIMER4::GetInstance();
  t4->SetInterruptHandler(&handler);
  t4->SetupCounter(period);

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

  testPWM2();
  //MX_TIM1_Init();

  while (1) {
  }
}
