#include <stdio.h>
#include "config.h"
#include "gpio.h"
#include "timer_cpp.h"
#include <math.h>


class TestInterruptHandler : public TIMER_Interrupt_Handler {
public:
  TestInterruptHandler(TIM_Channel* ch, uint16_t period) : 
  _ch (ch), _period(period)
  {
    _max = 20000 / period;
    _step = 180.0 / _max * 2.0;
    _cnt = 0;
  }

  virtual void HandleInterrupt()  {
    
    float ds = 1.0 + sin (_step*_cnt*M_PI/180. - M_PI/2.0);
    //float ds = sin (_step*_cnt*M_PI/180.);
    ds = _period*ds/2.0;
    //printf ("%d %f\n", _cnt, ds);
    _ch->timer->UpdateDs(_ch->channel, ds);

    if (++_cnt >= _max) {
      _cnt = 0;
    }

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
  float _step;
};



static void testPWM2() {

  GPIO_PIN pin1 = GPIO_GetPin("A0");    // TIM2 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIMER* t = TIMER2::GetInstance();
  int period = 50;
  TIM_Channel ch = t->SetupPWM(1, period, 1);
  t->EnableCounter();

  TestInterruptHandler handler (&ch, period);
  TIMER* t4 = TIMER4::GetInstance();
  t4->SetInterruptHandler(&handler);
  t4->SetupCounter(period);

  while(1)
    ;
}




int main(void) {

  System_Config();
  SystemClock_Config_HSE(); // 24mA

  testPWM2();

  while (1)
    ;
}
