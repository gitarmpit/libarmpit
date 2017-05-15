#include "lis3dsh.h"
#include <stdlib.h>
#include "RCC_helper.h"
#include "GPIO_Helper.h"
#include "debug.h"



static void intro_blink(void) 
{
    //Use the user LED pins in a standard output mode 
    GPIOD::GetInstance()->EnablePeripheralClock(true);
    GPIO_PIN* pins[4] = 
    {
        GPIOD::GetInstance()->GetPin(GPIO_PIN12), //green
        GPIOD::GetInstance()->GetPin(GPIO_PIN13), //orange
        GPIOD::GetInstance()->GetPin(GPIO_PIN14), //red
        GPIOD::GetInstance()->GetPin(GPIO_PIN15)  //blue      
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

static void accelero_test(void)
{

    intro_blink();
    
    
    uint16_t max_threshold = 1000;
    uint16_t read_delay_ms = 20;
    uint16_t min_threshold = 50;
    
    
    TIMER_With_Channels* timer = GPIO_Helper::SetupTIM4_PD_12_13_14_15();
    
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
    
    

    GPIO_PORT* ss_port = GPIOE::GetInstance();
    ss_port->EnablePeripheralClock(true);
    GPIO_PIN* SS_pin = ss_port->GetPin (GPIO_PIN3);
    SS_pin->SetupGPIO_OutPP();
    SS_pin->PullDown();
    SS_pin->SetSpeedHigh();

    SS_pin->Set();

    SPI1* spi1 = GPIO_Helper::SetupSPI1_MasterMode_PA_5_6_7(false, false, SPI_BAUD_RATE_16);

    ///////////////
    delay(50);

    LIS3DSH lis3dsh (spi1, SS_pin);

    uint8_t who_am_i = lis3dsh.ReadID();
    //if (true)
    if (who_am_i == I_AM_LIS3DSH) 
    {
        int16_t x, y, z;
        int16_t xoff = 0, yoff = 0;
        uint16_t ctrl = LIS3DSH_DATARATE_100 |  LIS3DSH_XYZ_ENABLE;
            
        ctrl |= ((LIS3DSH_SERIALINTERFACE_4WIRE |\
                    LIS3DSH_SELFTEST_NORMAL |\
                    LIS3DSH_FULLSCALE_2 |\
                    LIS3DSH_FILTER_BW_800) << 8);
            
        lis3dsh.Init (ctrl);
        delay(50);
        while(1) 
        {
            float fx = 0, fy = 0, fz = 0;
            lis3dsh.ReadACC (&fx, &fy, &fz);   
            x = (int16_t) fx;
            y = (int16_t) fy;
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
            
        }
    }

    while(1) 
    {
    }

}

int main()
{
    RCC_EnableHSE_168Mhz();
    accelero_test();
}
