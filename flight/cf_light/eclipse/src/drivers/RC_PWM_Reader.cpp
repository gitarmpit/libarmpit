#include "RC_PWM_Reader.h"
#include "_pins.h"

RC_PWM_Reader::RC_PWM_Reader(TIMER_With_Channels* timer)
{
    _timer = timer;
    _rollReader =  new PWM_Capture_EXTI(timer, PWM_INPUT_ROLL, 0);
    _pitchReader =  new PWM_Capture_EXTI(timer, PWM_INPUT_PITCH, 0);
    _throttleReader =  new PWM_Capture_EXTI(timer, PWM_INPUT_THROTTLE, 0);
}

void RC_PWM_Reader::Start()
{
    _timer->EnablePeripheralClock(true);
    _timer->EnableNVICInterrupt(true);

    _timer->SetUpdatePeriod_us(50000);
    _timer->EnableAutoPreload(true);

    _timer->EnableUpdateInterrupt(true);
    _timer->SetCounterValue(0);

    _throttleReader->Start();
    _rollReader->Start();
    _pitchReader->Start();

    _timer->EnableCounter(true);

}

uint32_t RC_PWM_Reader::GetThrottle()
{
    return _throttleReader->GetDuty_us();
}
uint32_t RC_PWM_Reader::GetPitch()
{
    return _pitchReader->GetDuty_us();

}
uint32_t RC_PWM_Reader::GetRoll()
{
    return _rollReader->GetDuty_us();
}



