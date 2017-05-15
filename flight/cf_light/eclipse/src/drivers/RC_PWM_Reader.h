#ifndef RC_PWM_READER_H
#define RC_PWM_READER_H

#include "timer_cpp.h"
#include "EXTI_PWM_InputCapture.h"

class RC_PWM_Reader
{
private:
    TIMER_With_Channels* _timer;
    PWM_Capture_EXTI*        _rollReader;
    PWM_Capture_EXTI*        _pitchReader;
    PWM_Capture_EXTI*        _throttleReader;

public:
    RC_PWM_Reader(TIMER_With_Channels* timer);

    void Start();
    uint32_t GetThrottle();
    uint32_t GetPitch();
    uint32_t GetRoll();
};



#endif
