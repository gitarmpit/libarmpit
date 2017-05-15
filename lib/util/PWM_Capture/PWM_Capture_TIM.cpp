#include "PWM_Capture_TIM.h"
#include "GPIO_Helper.h"

PWM_Capture_TIM::PWM_Capture_TIM(TIMER_With_Channels* timer,
        int timerChannel,
        const char* pin,
        PWM_Timeout_Handler* timeoutHandler)
{
	_timer = timer;
	_ch = _timer->GetChannel(timerChannel);

	_lastCounterValue = 0;

	_pin = GPIO_Helper::GetPin(pin);
#if defined(STM32F4) || defined(STM32F2)
	_pin->SetAF(timer->GetAf());
	_pin->SetupGPIO_OutAltPP();
#else
	_pin->SetupGPIO_InFloat();
#endif

	_missedCount = 0;
	_timeoutHandler = timeoutHandler;
	_period_tick = 0;
	_duty_tick = 0;
	_started = false;
	_ar = 0;
	_presc = 0;
	_clockFreq = 0;
	_fallingEdge = false;

}

void PWM_Capture_TIM::Start()
{
	//_timer->EnablePeripheralClock(true);

	_ch->EnableCCInterrupt(true, this);
	_ch->SetModeInputDirect();
#ifdef STM32F1
	_ch->SetICCFallingEdge();
	_fallingEdge = true;
#else
	_ch->SetICCBothEdges();
#endif
	_ch->EnableChannel(true);

	//_timer->EnableNVICInterrupt(true);

	//_timer->SetUpdatePeriod_us(UPDATE_PERIOD_US);
	//_timer->EnableAutoPreload(true);

	_timer->AddInterruptHandler(this);
	//_timer->EnableUpdateInterrupt(true);
	//_timer->SetCounterValue(0);

	_presc = _timer->GetPrescalerValue();
	_clockFreq = _timer->GetTimerClockFreq();
	_ar = _timer->GetAutoReloadValue();

	//_timer->EnableCounter(true);

}

void PWM_Capture_TIM::HandleInterrupt(TIMER_Channel* ch)
{
	static int32_t tick;

	uint32_t cnt = ch->GetCCR();

#ifdef STM32F1
    if (_fallingEdge)
    {
        _ch->SetICCRisingEdge();
    }
    else
    {
        _ch->SetICCFallingEdge();
    }
    _fallingEdge = !_fallingEdge;
#endif

	_missedCount = 0;
	if (_lastCounterValue != 0)
	{
		_started = true;
		tick = cnt - _lastCounterValue;
		if (tick < 0)
		{
			tick += (_ar + 1);
		}

        if (_pin->IsSet())
        {
            if (_duty_tick != 0)
            {
                _period_tick = tick + _duty_tick;
            }
        }
        else
        {
            _duty_tick = tick;
        }

	}
	_lastCounterValue = cnt;
}

void PWM_Capture_TIM::HandleInterrupt(bool isUpdate, bool isTrigger)
{
	if (isUpdate)
	{
		if (_started && ++_missedCount == MAX_MISSED_COUNT && _timeoutHandler)
		{
			_duty_tick = 0;
			_period_tick = 0;
			_timeoutHandler->Handle_Timeout();
		}
	}
}

