#ifndef _PROGRESS_BLINKER_H
#define _PROGRESS_BLINKER_H

#include "gpio_cpp.h"
#include "timer_cpp.h"

class LED_Indicators
{
private:
    GPIO_PIN* _rgb[3];

    void SteadyOn(uint32_t idx, uint32_t duration_ms);
    void Toggle(uint32_t idx);
    void BlinkingOn(uint32_t idx, uint32_t period_ms, uint32_t duty_ms, uint32_t duration_ms);

    LED_Indicators(LED_Indicators const&);
    LED_Indicators();
    void operator=(LED_Indicators const&);


public:

    static LED_Indicators* GetInstance()
    {
        static LED_Indicators  instance;
        return &instance;
    }

    void RGB_BlinkAll();

    void RGB_SteadyRed(uint32_t duration_ms);
    void RGB_BlinkingRed(uint32_t period_ms, uint32_t duty_ms, uint32_t duration_ms);
    void RGB_SteadyGreen(uint32_t duration_ms);
    void RGB_BlinkingGreen(uint32_t period_ms, uint32_t duty_ms, uint32_t duration_ms);
    void RGB_SteadyBlue(uint32_t duration_ms);
    void RGB_BlinkingBlue(uint32_t period_ms, uint32_t duty_ms, uint32_t duration_ms);

    void RGB_Off();

    void RGB_ToggleRed();
    void RGB_ToggleBlue();
    void RGB_ToggleGreen();
    void ToggleRGB();

};

#endif
