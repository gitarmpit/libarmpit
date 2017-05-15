#include "ESC_PWM_Control.h"

ESC_PWM_Control::ESC_PWM_Control(TIMER_With_Channels*  timer)
{
	_timer = timer;

	_ch1 = _timer->GetChannel(1);
	_ch1->SetupPWM1(PWM_PERIOD_MS, 0);
	_ch1->EnableChannel(true);

	_ch2 = _timer->GetChannel(2);
	_ch2->SetupPWM1(PWM_PERIOD_MS, 0);
	_ch2->EnableChannel(true);

	_ch3 = _timer->GetChannel(3);
	_ch3->SetupPWM1(PWM_PERIOD_MS, 0);
	_ch3->EnableChannel(true);

	_ch4 = _timer->GetChannel(4);
	_ch4->SetupPWM1(PWM_PERIOD_MS, 0);
	_ch4->EnableChannel(true);

	_timer->EnableCounter(true);

}



void ESC_PWM_Control::SetDuty(uint32_t duty_ms)
{
	SetDuty(duty_ms, duty_ms, duty_ms, duty_ms);
}

void ESC_PWM_Control::SetDuty(uint32_t m1_duty_ms, uint32_t m2_duty_ms, uint32_t m3_duty_ms, uint32_t m4_duty_ms)
{
    _ch1->UpdatePWMDuty(m1_duty_ms);
    _ch2->UpdatePWMDuty(m2_duty_ms);
    _ch3->UpdatePWMDuty(m3_duty_ms);
    _ch4->UpdatePWMDuty(m4_duty_ms);
}






