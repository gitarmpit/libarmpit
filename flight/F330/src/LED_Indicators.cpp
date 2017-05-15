#include "LED_Indicators.h"
#include "GPIO_Helper.h"
#include <stdlib.h>
#define PITCH_ROLL_PERIOD 1000
#define THROTTLE_LED_PERIOD 7000
#include "_pins.h"

LED_Indicators::LED_Indicators()
{
    GPIOE::GetInstance()->EnablePeripheralClock(true);
    GPIOC::GetInstance()->EnablePeripheralClock(true);


    TIMER_With_Channels* t = GPIO_Helper::SetupTimer(PITCH_ROLL_LED_TIMER);
    _pitchRollCh = t->GetChannels();


}


void LED_Indicators::EnablePitchRollLeds(bool enable)
{
    for (int i = 0; i < 4; ++i)
    {
        _pitchRollCh[i].EnableChannel(enable);
        if (enable)
        {
            _pitchRollCh[i].SetupPWM1(PITCH_ROLL_PERIOD, 0);
        }
    }
    _pitchRollCh[0].GetTimer()->EnableCounter(enable);
}



void LED_Indicators::DisplayPitchRoll (int8_t p, int8_t r)
{
    static uint8_t k = 3;
    if (abs(p) > 45)
    {
        p = p > 0 ? 45 : -45;
    }

    if (abs(r) > 45)
    {
        r = r > 0 ? 45 : -45;
    }

    if (p >= 0)
    {
        _pitchRollCh[2].UpdatePWMDuty(p*k);
        _pitchRollCh[0].UpdatePWMDuty(0);
    }
    else
    {
        _pitchRollCh[2].UpdatePWMDuty(0);
        _pitchRollCh[0].UpdatePWMDuty(-p*k);
    }
    if (r >= 0)
    {
        _pitchRollCh[1].UpdatePWMDuty(r*k);
        _pitchRollCh[3].UpdatePWMDuty(0);
    }
    else
    {
        _pitchRollCh[1].UpdatePWMDuty(0);
        _pitchRollCh[3].UpdatePWMDuty(-r*k);
    }

}

