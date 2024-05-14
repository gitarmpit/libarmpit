#include "LED_Indicators.h"
#include "GPIO_Helper.h"
#include <stdlib.h>
#define PITCH_ROLL_PERIOD 1000
#define THROTTLE_LED_PERIOD 7000
#include "_pins.h"
#include "debug.h"

LED_Indicators::LED_Indicators()
{
    GPIOE::GetInstance()->EnablePeripheralClock(true);
    GPIOC::GetInstance()->EnablePeripheralClock(true);

    _rgb[0] = RGB_LED_RED;
    _rgb[0]->SetupGPIO_OutPP();

    _rgb[1] = RGB_LED_GREEN;
    _rgb[1]->SetupGPIO_OutPP();

    _rgb[2] = RGB_LED_BLUE;
    _rgb[2]->SetupGPIO_OutPP();

    RGB_Off();



    _greenMotorLed = LED_GREEN;
    _greenMotorLed->SetupGPIO_OutPP();
    _greenMotorLed->Reset();

    TIMER_With_Channels* t = GPIO_Helper::SetupTimer(PITCH_ROLL_LED_TIMER);
    _pitchRollCh = t->GetChannels();

    t = GPIO_Helper::SetupTimer(THROTTLE_LED_TIMER);
    _throttleLedCh = t->GetChannels();

}

void LED_Indicators::RGB_BlinkAll()
{
    static uint32_t  cnt = 0;
    static uint32_t  delay = 0;

    if (!(delay++ % 5))
    {
        uint8_t active = cnt++ % 3;

        for (uint8_t i = 0; i < 3; ++i)
        {
            if (i == active)
            {
                _rgb[i]->Reset();
            }
            else
            {
                _rgb[i]->Set();
            }
        }
    }
}

void LED_Indicators::SteadyOn(uint32_t idx, uint32_t duration_ms)
{
    RGB_Off();
    _rgb[idx]->Reset();
    delay(duration_ms);

}
void LED_Indicators::BlinkingOn(uint32_t idx, uint32_t period_ms, uint32_t duty_ms, uint32_t duration_ms)
{
    RGB_Off();
    uint32_t count = duration_ms / period_ms;
    for (uint32_t i = 0; i < count; ++i)
    {
        _rgb[idx]->Reset();
        delay(duty_ms);
        RGB_Off();
        delay(period_ms - duty_ms);
    }
}


void LED_Indicators::RGB_SteadyRed(uint32_t duration_ms)
{
    SteadyOn(0, duration_ms);
}
void LED_Indicators::RGB_BlinkingRed(uint32_t period_ms, uint32_t duty_ms, uint32_t duration_ms)
{
    BlinkingOn(0, period_ms, duty_ms, duration_ms);
}

void LED_Indicators::RGB_SteadyGreen(uint32_t duration_ms)
{
    SteadyOn(1, duration_ms);
}
void LED_Indicators::RGB_BlinkingGreen(uint32_t period_ms, uint32_t duty_ms, uint32_t duration_ms)
{
    BlinkingOn(1, period_ms, duty_ms, duration_ms);
}

void LED_Indicators::RGB_SteadyBlue(uint32_t duration_ms)
{
    SteadyOn(2, duration_ms);
}
void LED_Indicators::RGB_BlinkingBlue(uint32_t period_ms, uint32_t duty_ms, uint32_t duration_ms)

{
    BlinkingOn(2, period_ms, duty_ms, duration_ms);
}

void LED_Indicators::RGB_Off()
{
    _rgb[0]->Set();
    _rgb[1]->Set();
    _rgb[2]->Set();
}


void LED_Indicators::MotorLedOn()
{
    _greenMotorLed->Set();
}
void LED_Indicators::MotorLedOff()
{
    _greenMotorLed->Reset();
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

void LED_Indicators::EnableThrottleLevelLeds(bool enable)
{
    for (int i = 0; i < 4; ++i)
    {
        _throttleLedCh[i].EnableChannel(enable);
        if (enable)
        {
            _throttleLedCh[i].SetupPWM1(THROTTLE_LED_PERIOD, 0);
        }
    }
    _throttleLedCh[0].GetTimer()->EnableCounter(enable);
}



void LED_Indicators::DisplayPitchRoll (int8_t p, int8_t r)
{
    static uint8_t k = 5;
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

void LED_Indicators::DisplayThrottleLevel (int32_t m1, int32_t m2, int32_t m3, int32_t m4)
{
    m1 -= 1500;
    if (m1 < 0)
    {
        m1 = 0;
    }
    m2 -= 1500;
    if (m2 < 0)
    {
        m2 = 0;
    }
    m3 -= 1500;
    if (m3 < 0)
    {
        m3 = 0;
    }
    m4 -= 1500;
    if (m4 < 0)
    {
        m4 = 0;
    }

    _throttleLedCh[0].UpdatePWMDuty(m1/2);
    _throttleLedCh[1].UpdatePWMDuty(m2/2);
    _throttleLedCh[2].UpdatePWMDuty(m3/2);
    _throttleLedCh[3].UpdatePWMDuty(m4/2);

}

void LED_Indicators::Toggle(uint32_t idx)
{
    _rgb[idx]->Toggle();

}

void LED_Indicators::RGB_ToggleRed()
{
    Toggle(0);
}

void LED_Indicators::RGB_ToggleGreen()
{
    Toggle(1);
}

void LED_Indicators::RGB_ToggleBlue()
{
    Toggle(2);
}


void LED_Indicators::ToggleRGB()
{
    static int i = 0;
    _rgb[i++ % 3]->Set();
    _rgb[i % 3]->Reset();
}
