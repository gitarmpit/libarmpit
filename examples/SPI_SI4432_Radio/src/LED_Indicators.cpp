#include "LED_Indicators.h"
#include "GPIO_Helper.h"
#include <stdlib.h>
#include "_pins.h"
#include "debug.h"

LED_Indicators::LED_Indicators()
{
    _rgb[0] = GPIO_Helper::GetPin(LED_R_PIN);
    _rgb[0]->SetupGPIO_OutPP();

    _rgb[1] = GPIO_Helper::GetPin(LED_G_PIN);
    _rgb[1]->SetupGPIO_OutPP();

    _rgb[2] = GPIO_Helper::GetPin(LED_B_PIN);
    _rgb[2]->SetupGPIO_OutPP();

    RGB_Off();


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
                _rgb[i]->Set();
            }
            else
            {
                _rgb[i]->Reset();
            }
        }
    }
}

void LED_Indicators::SteadyOn(uint32_t idx, uint32_t duration_ms)
{
    RGB_Off();
    _rgb[idx]->Set();
    delay(duration_ms);

}
void LED_Indicators::BlinkingOn(uint32_t idx, uint32_t period_ms, uint32_t duty_ms, uint32_t duration_ms)
{
    RGB_Off();
    uint32_t count = duration_ms / period_ms;
    for (uint32_t i = 0; i < count; ++i)
    {
        _rgb[idx]->Set();
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
    _rgb[0]->Reset();
    _rgb[1]->Reset();
    _rgb[2]->Reset();
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
    _rgb[i % 3]->Reset();
    _rgb[++i % 3]->Set();
}
