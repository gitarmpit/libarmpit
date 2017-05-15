#include "timer_ch_setup.h"
#include "GPIO_Helper.h"

Timer_Ch_Setup::Timer_Ch_Setup()
{
    TIMER_With_Channels* t4 = GPIO_Helper::SetupTimer(TIM4_PB_6_7_8_9);
    uint32_t t4_period = 1000;
    t4->EnableUpdateInterrupt(true);
    t4->EnableNVICInterrupt(true);

    //B6 42 BL
    _ch_bl = t4->GetChannel(1);
    _ch_bl->SetupPWM1(t4_period, 0);
    _ch_bl->EnableChannel(true);

    //43 B7 Sound
    _ch_sound = t4->GetChannel(2);
    _ch_sound->SetupPWM1(t4_period, 0);
    _ch_sound->EnableChannel(true);

    //45 Red LED
    _ch_red = t4->GetChannel(3);
    _ch_red->SetupPWM1(t4_period, 0);
    _ch_red->EnableChannel(true);

    //46 Green LED
    _ch_green = t4->GetChannel(4);
    _ch_green->SetupPWM1(t4_period, 20);
    _ch_green->EnableChannel(true);

    t4->EnableCounter(true);

}
