#ifndef TIMER_PWM_H
#define TIMER_PWM_H

#include "gpio_cpp.h"
#include "timer_cpp.h"

class PWM_Timeout_Handler
{
public:
    virtual void Handle_Timeout() = 0;
};

class PWM_Capture_TIM : public TIMER_Channel_CCInterrupt_Handler, TIMER_Interrupt_Handler
{
private:
    TIMER_With_Channels* _timer;
    TIMER_Channel* _ch;
    GPIO_PIN* _pin;
    uint32_t _lastCounterValue;
    uint32_t _clockFreq;
    uint32_t _presc;
    uint32_t _ar;
    uint32_t _missedCount;
    //timeout: 50ms x 20 = 1 sec
    static const uint32_t MAX_MISSED_COUNT = 20;
    static const uint32_t UPDATE_PERIOD_US = 5000;
    PWM_Timeout_Handler* _timeoutHandler;

    uint32_t _period_tick;
    uint32_t _duty_tick;

    bool     _fallingEdge;  //STM32F1

    bool _started;

public:

    PWM_Capture_TIM(TIMER_With_Channels* timer,
              int timerChannel,
              const char* pin,
              PWM_Timeout_Handler* timeoutHandler);

    void Start();

    virtual void HandleInterrupt(TIMER_Channel* ch);

    virtual void HandleInterrupt(bool isUpdate, bool isTrigger);

    uint32_t GetPeriod_us() { return
            (uint64_t)_period_tick * 1000000ull * ((uint64_t)_presc + 1) / (uint64_t)_clockFreq;}
    uint32_t GetDuty_us() { return
            (uint64_t)_duty_tick * 1000000ull * ((uint64_t)_presc + 1) / (uint64_t)_clockFreq;}

};


#endif
