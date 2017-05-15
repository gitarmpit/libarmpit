#include "_pins.h"
#include "LED_Indicators.h"
#include "GPIO_Helper.h"
#include <math.h>
#include "buzzer.h"
#include "debug.h"

void rgb_pwm_test()
{
    //yellow: r=3500 g = 8000

    TIMER_With_Channels* rt = (TIMER_With_Channels*)TIMER::Get_Instance(LED_R_TIM);
    rt->EnablePeripheralClock(true);

    GPIO_PIN* r = GPIO_Helper::GetPin(LED_R_PIN);
    r->SetAF(rt->GetAf());
    r->SetupGPIO_OutAltPP();

    TIMER_Channel* r_ch = rt->GetChannel(LED_R_CH);

    r_ch->EnableChannel(true);
    r_ch->SetupPWM1(10000, 0);
    rt->EnableCounter(true);

    TIMER_With_Channels* gt = (TIMER_With_Channels*)TIMER::Get_Instance(LED_G_TIM);
    gt->EnablePeripheralClock(true);

    GPIO_PIN* g = GPIO_Helper::GetPin(LED_G_PIN);
    g->SetAF(gt->GetAf());
    g->SetupGPIO_OutAltPP();

    TIMER_Channel* g_ch = gt->GetChannel(LED_G_CH);

    //g_ch->EnableChannel(true);
    //g_ch->SetupPWM1(10000, 0);
    //gt->EnableCounter(true);

    Buzzer*  buzzer = Buzzer::GetInstance();
    buzzer->Init(BUZZER_TIMER, BUZZER_PIN, BUZZER_TIMER_CHANNEL);
    buzzer->SetVolume(2);

    const int n = 64;
    int spoints[n];
    for (int i = 0; i < n; ++i)
    {
        float a = 2. * M_PI / n * i;
        //spoints[i] = 10000 * sin(a)*sin(a);
        //spoints[i] = 5000  + 10000 * sin(a);
        spoints[i] = 5000. *(1.0 + sin (a - M_PI/2.));
    }

    int i = 0;
    while(1)
    {
        //g_ch->UpdatePWMDuty(spoints[i % n]);
        r_ch->UpdatePWMDuty((float)spoints[i % n]);
        buzzer->PlayTone(50 + spoints[i % n]/2);
        if ((i % n) == 0)
        {
            //delay(100);
        }
        ++i;
        delay(20);
    }

//    float i = 0;
//    while(1)
//    {
//        g_ch->UpdatePWMDuty(10000*sin(i)*sin(i));
//        delay(15);
//        i  += 0.1;
//    }

//    TIMER_With_Channels* bt = (TIMER_With_Channels*)TIMER::Get_Instance(LED_B_TIM);
//    bt->EnablePeripheralClock(true);
//
//    GPIO_PIN* b = GPIO_Helper::GetPin(LED_B_PIN);
//    b->SetAF(LED_B_AF);
//    b->SetupGPIO_OutAltPP();
//
//    TIMER_Channel* b_ch = bt->GetChannel(LED_B_CH);
//
//    b_ch->EnableChannel(true);
//    b_ch->SetupPWM1(10000, 300);
//    bt->EnableCounter(true);


    while(1);


}

void rgb_test()
{
    GPIO_PIN* r = GPIO_Helper::GetPin(LED_R_PIN);
    r->SetupGPIO_OutPP();
    GPIO_PIN* g = GPIO_Helper::GetPin(LED_G_PIN);
    g->SetupGPIO_OutPP();
    GPIO_PIN* b = GPIO_Helper::GetPin(LED_B_PIN);
    b->SetupGPIO_OutPP();

    r->Set();
    r->Reset();
    g->Set();
    g->Reset();
    b->Set();
    b->Reset();


    LED_Indicators::GetInstance()->RGB_ToggleGreen();
    LED_Indicators::GetInstance()->RGB_ToggleGreen();

    while(1)
    {
        LED_Indicators::GetInstance()->ToggleRGB();
        delay(100);

    }
}

