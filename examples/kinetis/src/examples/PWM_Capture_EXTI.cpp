#if 0
#include "PWM_Capture_EXTI.h"
#include "GPIO_Helper.h"
    
PWM_Capture_EXTI::PWM_Capture_EXTI(TIMER* timer, const char* pin, PWM_Timeout_Handler* timeoutHandler)
{
    _inputPin = GPIO_Helper::GetPin(pin);
    _inputPin->SetupGPIO_InPullDown();
    _exti = GPIO_Helper::SetupEXTI(pin);
    //timer is already set up
    _timer = timer;
    _lastCounterValue = 0;
    _missedCount = 0;
    _timeoutHandler = timeoutHandler;
    _period_tick = 0;
    _duty_tick = 0;
    _started = false;

}

void PWM_Capture_EXTI::Start()
{
    _exti->SetIrqHandler(this);
    _exti->EnableInterruptRisingEdge(true);
    _exti->EnableInterruptFallingEdge(true);
    _timer->AddInterruptHandler(this);

    _presc =  _timer->GetPrescalerValue();
    _clockFreq = _timer->GetTimerClockFreq();
    _ar = _timer->GetAutoReloadValue();

    //TODO this or outside?
//    if (_timeoutHandler)
//    {
//      _timer->EnableUpdateInterrupt(true);
//    }

}

void PWM_Capture_EXTI::HandleInterrupt(EXTI* exti)
{
    static int32_t tick;
    uint32_t cnt = _timer->GetCounterValue();
    _missedCount = 0;
    if (_lastCounterValue != 0)
    {
        _started = true;
        tick = cnt - _lastCounterValue;
        if (tick < 0)
        {
            tick += (_ar + 1);
        }

        if (_inputPin->IsSet())
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


void PWM_Capture_EXTI::HandleInterrupt(bool isUpdate, bool isTrigger)
{
    if (isUpdate)
    {
        if (_started && ++_missedCount == MAX_MISSED_COUNT)
        {
            _period_tick = 0;
            _duty_tick = 0;
            if (_timeoutHandler)
                _timeoutHandler->Handle_Timeout();
        }
    }
}

#endif
