#ifndef _TIMER_CH_SETUP_H
#define _TIMER_CH_SETUP_H

#include "timer_cpp.h"

class Timer_Ch_Setup
{
private:
    TIMER_Channel* _ch_bl;
    TIMER_Channel* _ch_sound;
    TIMER_Channel* _ch_red;
    TIMER_Channel* _ch_green;
public:
    Timer_Ch_Setup();

    TIMER_Channel* Get_BL() { return _ch_bl; }
    TIMER_Channel* Get_Sound() { return _ch_sound; }
    TIMER_Channel* Get_Red() { return _ch_red; }
    TIMER_Channel* Get_Green() { return _ch_green; }
};


#endif
