#ifndef ESC_PWM_CONTROL_H
#define ESC_PWM_CONTROL_H

#include "timer_cpp.h"

class ESC_PWM_Control
{
private:
    TIMER_With_Channels* _timer;
    TIMER_Channel* _ch1;
    TIMER_Channel* _ch2;
    TIMER_Channel* _ch3;
    TIMER_Channel* _ch4;

    static const uint16_t PWM_PERIOD_MS = 20000;

public:
    ESC_PWM_Control(TIMER_With_Channels*  timer);

    void SetDuty(uint32_t duty_ms);
    void SetDuty(uint32_t m1_duty_ms, uint32_t m2_duty_ms, uint32_t m3_duty_ms, uint32_t m4_duty_ms);

};


#endif
