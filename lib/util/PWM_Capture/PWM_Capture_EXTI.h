#ifndef EXTI_PWM_H
#define EXTI_PWM_H
#include "gpio_cpp.h"
#include "exti_cpp.h"
#include "timer_cpp.h"

class PWM_Timeout_Handler
{
public:
    virtual void Handle_Timeout() = 0;
};

class PWM_Capture_EXTI: public EXTI_IRQ_Handler, TIMER_Interrupt_Handler
{
private:
    EXTI* _exti;
    TIMER* _timer;
    GPIO_PIN* _inputPin;
    uint32_t _lastCounterValue;
    uint32_t _clockFreq;
    uint32_t _presc;
    uint32_t _ar;
    uint32_t _missedCount;
    static const uint32_t MAX_MISSED_COUNT = 10;
    uint32_t _update_period_us;
    PWM_Timeout_Handler* _timeoutHandler;

    uint32_t _period_tick;
    uint32_t _duty_tick;

    bool _started;

public:
    PWM_Capture_EXTI(TIMER* timer, const char* pin, PWM_Timeout_Handler* timeoutHandler = 0);

    void Start();
    virtual void HandleInterrupt(EXTI* exti);
    virtual void HandleInterrupt(bool isUpdate, bool isTrigger);
    uint32_t GetPeriod_us() { return
            (uint64_t)_period_tick * 1000000ull * ((uint64_t)_presc + 1) / (uint64_t)_clockFreq;}
    uint32_t GetDuty_us() { return
            (uint64_t)_duty_tick * 1000000ull * ((uint64_t)_presc + 1) / (uint64_t)_clockFreq;}

};


#endif
