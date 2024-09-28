#include "gpio.h"
#include "timer_cpp.h"

void testTimer() {
  GPIO_PIN pin = GPIO_GetPin("A0");
  GPIO_Setup_OutAltPP(&pin);
  GPIO_SetAF(&pin, 2);
  TIMER* t = TIMER2::GetInstance();
  TIM_Channel ch = t->SetupPWM(1, 400, 200);
  while(1)
    ;
}


/*


class TIMBuzzerTimer : public BuzzerTimer {
  public:
    TIMBuzzerTimer(TIM_Channel* ch) : _ch (ch) {}
    virtual void SetupPWM(uint32_t period_us, uint32_t ds_us) {
      _ch->timer->SetupPWM(_ch->channel, period_us, ds_us);
    }
    virtual void UpdatePWM(uint32_t period_us, uint32_t ds_us) {
      _ch->timer->UpdatePeriodDs(_ch->channel, period_us, ds_us);
    }
    virtual void Stop() {
      _ch->timer->UpdateDs(_ch->channel, 0);
    }
  private:
    TIM_Channel* _ch;
};


void testTimBuzzerTimer() {
  GPIO_PIN pin = GPIO_GetPin("A0");
  GPIO_Setup_OutAltPP(&pin);
  GPIO_SetAF(&pin, 2);
  TIMER* t       = TIMER2::GetInstance();
  TIM_Channel ch = t->SetupPWM(1, 0, 0);

  TIMBuzzerTimer tb (&ch);
  // tb.SetupPWM(400, 200);

  Buzzer* buzzer = Buzzer::GetInstance();
  buzzer->Init(&tb);
  buzzer->SetVolume(10);
  buzzer->Stop();
  buzzer->PlayTune(barbie);

  while (1)
    ;
}
*/