#ifndef _LED_PWM_H
#define _LED_PWM_H

#include "gpio_cpp.h"
#include "timer_cpp.h"


class LED_PWM
{
private:
	TIMER_Channel*  _ch;

	int32_t   _period_ms;

    void operator=(LED_PWM const&);

public:
    LED_PWM (TIMER_With_Channels* t, uint8_t ch, int32_t max_brightness);

    void On(uint16_t brightness = 0);
    void Off();

};


#endif
