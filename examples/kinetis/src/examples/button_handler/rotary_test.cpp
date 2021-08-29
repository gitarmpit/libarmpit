#include "rotary_encoder_handler.h"
#include "gpio_helper.h"
#include "mcg.h"
#include "sim.h"
#include <stdio.h>

GPIO_PIN ledPin;

int cw, ccw;
class MyRotaryHandler : public RotaryHandler
{
public:
    MyRotaryHandler (PIT* pit, Button* A, GPIO_PIN* B) : RotaryHandler (pit, A, B)
    {
    }

    virtual void OnStateChange(int8_t dir)
    {
        //GPIO_TogglePin(&ledPin);
        //printf ("dir:%d\n", dir);
    	if (dir == 1)
    	{
    		++cw;
    	}
    	else
    	{
    		++ccw;
    	}
    	if (cw > 100 || ccw > 100)
    	{
    		printf ("cw:%d, ccw:%d\n", cw, ccw);
    		cw = 0;
    		ccw = 0;
    	}
    }

};


void test_rotary_handler()
{
    InitClock_FEI_48Mhz_Bus_24Mhz();
    SIM_Enable_PIT(TRUE);

    PIT* pit = PIT_GetInstance(PIT0);
    ledPin = GPIO_Helper_GetPin("D7");
    GPIO_SetupOut(&ledPin);
    GPIO_TogglePin(&ledPin);

    GPIO_PIN pinA = GPIO_Helper_GetPin("A12");
    Button A (&pinA, 1);
    GPIO_PIN pinB = GPIO_Helper_GetPin("A5");


    MyRotaryHandler bh (pit, &A, &pinB);
    bh.SetUpdateIntervalUs(100);
    bh.SetSettleTimeUs(100);
    bh.Init(true);
    while(1)
        ;
}
