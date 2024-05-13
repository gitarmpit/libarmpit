#include "lis3dsh.h"
#include <stdlib.h>
#include "RCC_helper.h"
#include "GPIO_Helper.h"
#include "_pins.h"
#include "button_handler.h"
#include "debug.h"

//#include "led_pwm.h"


//signals to get out of the accel loop
bool g_accel_off;

class BHandler : public ButtonHandler
{
private:
    int cur_brightness;
    int max_brightness;
    TIMER_Channel*  leds;
    int inc100;
    int inc10;
public:
    BHandler(TIMER* t) : ButtonHandler(t)
    {
        max_brightness = 1000;
        cur_brightness = max_brightness;
        inc100 = 100;
        inc10 = 10;
        SetTimerHookInterval(10);
    }

    void Run()
    {
        Button butt (GPIO_Helper::GetPin(SWITCH_PIN), 1);
        butt.PullDown();
        AddButton(&butt);

        TIMER_With_Channels* timer = GPIO_Helper::SetupTimer(LED_TIMER);
        leds = timer->GetChannels();

//        LED_PWM  g (timer, 1, max_brightness);
//        LED_PWM  o (timer, 2, max_brightness);
//        LED_PWM  r (timer, 3, max_brightness);
//        LED_PWM  b (timer, 4, max_brightness);
//
//        g.On(max_brightness);
//        o.On(max_brightness);
//        r.On(max_brightness);
//        b.On(max_brightness);


        for (uint8_t i = 0; i < 4; ++i)
        {
            leds[i].SetupPWM1(max_brightness, cur_brightness);
            leds[i].EnableChannel(true);
        }

        timer->EnableCounter(true);


        while(1)
        {
            delay(1);
        }
    }

    virtual void OnStateChange(Button* button)
    {
    }

    virtual void OnButtonUp(Button* button)
    {
    }

    virtual void OnButtonDown(Button* button)
    {
    }

    virtual void  TimerHook(Button* button)
    {
        if (button->IsDown())
        {
            if (cur_brightness == max_brightness || cur_brightness == 0)
            {
                inc10 = -inc10;
                inc100 = -inc100;
            }

            cur_brightness += inc10;

            if (cur_brightness > max_brightness)
            {
                cur_brightness = max_brightness;
            }

            if (cur_brightness < 0)
            {
                cur_brightness = 0;
            }

            for (uint8_t i = 0; i < 4; ++i)
            {
                leds[i].UpdatePWMDuty(cur_brightness);
            }
        }
    }

    virtual void OnClick(Button* button)
    {
        if (cur_brightness == max_brightness || cur_brightness == 0)
        {
            inc10 = -inc10;
            inc100 = -inc100;
        }

        cur_brightness += inc100;

        if (cur_brightness > max_brightness)
        {
            cur_brightness = max_brightness;
        }

        if (cur_brightness < 0)
        {
            cur_brightness = 0;
        }


        for (uint8_t i = 0; i < 4; ++i)
        {
            leds[i].UpdatePWMDuty(cur_brightness);
        }

    }

    virtual void OnDoubleClick(Button* button)
    {
    }
};

static void intro_blink(void) 
{
    //Use the user LED pins in a standard output mode 
    GPIOD::GetInstance()->EnablePeripheralClock(true);
    GPIO_PIN* pins[4] = 
    {
            GPIO_Helper::GetPin(LED_GREEN),
            GPIO_Helper::GetPin(LED_ORANGE),
            GPIO_Helper::GetPin(LED_RED),
            GPIO_Helper::GetPin(LED_BLUE)
    };
    
    for (int i = 0; i < 4; ++i) 
    {
        pins[i]->SetupGPIO_OutPP();
    }
    
    for (uint16_t i = 0; i < 10*4; ++i) 
    {
        pins[i % 4]->Set();
        delay(100);
        pins[i % 4]->Reset();
    }
    
    for (int i = 0; i < 4; ++i) 
    {
        pins[i]->Reset();
    }
    
}

static void button_led_test(void)
{
    BHandler bhandler (TIMER::Get_Instance(BUTTON_TIMER));
    bhandler.Run();

    while(1)
        ;
}

static void accelero_test(void)
{
    
    Button b (GPIO_Helper::GetPin(SWITCH_PIN), 1);
    b.PullDown();

    
    uint16_t max_threshold = 1000;
    uint16_t read_delay_ms = 20;
    uint16_t min_threshold = 50;
    
    
    TIMER_With_Channels* timer = GPIO_Helper::SetupTimer(LED_TIMER);
    
    //green: negative X
    TIMER_Channel* ch1_green = timer->GetChannel(1);
    ch1_green->SetupPWM1(max_threshold, 0); 
    ch1_green->EnableChannel(true);
    
    //orange: positive Y
    TIMER_Channel* ch2_orange = timer->GetChannel(2);
    ch2_orange->SetupPWM1(max_threshold, 0); 
    ch2_orange->EnableChannel(true);

    //red: positive X
    TIMER_Channel* ch3_red = timer->GetChannel(3);
    ch3_red->SetupPWM1(max_threshold, 0); 
    ch3_red->EnableChannel(true);

    //blue: negative Y
    TIMER_Channel* ch4_blue = timer->GetChannel(4);
    ch4_blue->SetupPWM1(max_threshold, 0); 
    ch4_blue->EnableChannel(true);    
    
    timer->EnableCounter(true);
    
    

    GPIO_PIN* SS_pin = GPIO_Helper::GetPin(SPI_SS_PIN);
    SS_pin->SetupGPIO_OutPP();
    SS_pin->PullDown();
    SS_pin->SetSpeedHigh();

    SS_pin->Set();

    SPI* spi = GPIO_Helper::SetupSPI(ACCEL_SPI, true, false, false, SPI_BAUD_RATE_8);

    ///////////////
    delay(50);

    LIS3DSH lis3dsh (spi, SS_pin);

    uint8_t who_am_i = lis3dsh.ReadID();
    //if (true)
    if (who_am_i != LIS3DSH_ID)
    {
        int i = 0;
        while (1)
        {
            if ((i++ % 2) == 0)
            {
                ch3_red->UpdatePWMDuty(max_threshold);
            }
            else
            {
                ch3_red->UpdatePWMDuty(0);
            }
        }
    }

    int16_t x, y, z;
    int16_t xoff = 0, yoff = 0;
    delay(50);
    while(1)
    {
        lis3dsh.ReadAccel(&x, &y, &z);
        if (xoff == 0 && yoff == 0)
        {
            xoff = x;
            yoff = y;
        }
        else
        {
            x -= xoff;
            y -= yoff;
            TIMER_Channel* chx = 0;
            TIMER_Channel* chy = 0;
            if (abs(x) > min_threshold)
            {
                if (x > 0)
                {
                    chx = ch3_red;
                }
                else
                {
                    chx = ch1_green;
                    x = abs(x);
                }

                chx->UpdatePWMDuty (x);
            }

            if (abs(y) > min_threshold)
            {
                if (y > 0)
                {
                    chy = ch2_orange;
                }
                else
                {
                    chy = ch4_blue;
                    y = abs(y);
                }

                chy->UpdatePWMDuty (y);
            }

            delay(read_delay_ms);
            if (chx)
            {
                chx->UpdatePWMDuty (0);
            }
            if (chy)
            {
                chy->UpdatePWMDuty (0);
            }

        }

        if (b.IsPinSet())
        {
            delay(50);
            break;
        }

    }

}




int main()
{
    RCC_EnableHSE_168Mhz();
    intro_blink();
    accelero_test();
    button_led_test();
}
