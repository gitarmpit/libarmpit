#include "led_pwm.h"

LED_PWM::LED_PWM (TIMER_With_Channels* t, uint8_t ch, int32_t max_brightness)
{
	if (!t->IsClockEnabled())
	{
		t->EnablePeripheralClock(true);
	}

    _period_ms = max_brightness;

	_ch = t->GetChannel(ch);
    _ch->EnableChannel(true);
    _ch->SetupPWM1(_period_ms, 0);


    t->EnableCounter(true);

}



void LED_PWM::On(uint16_t brightness)
{

	if (brightness == 0)
	{
		brightness = _period_ms;
	}

	if (brightness > _period_ms)
	{
		brightness = _period_ms;
	}

	_ch->UpdatePWMDuty(brightness);


}

void LED_PWM::Off()
{
	_ch->UpdatePWMDuty(0);
}


