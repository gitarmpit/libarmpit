#include "rotary_encoder/rotary_encoder_handler.h"
#include "gpio_helper.h"
#include "mcg.h"
#include "sim.h"

GPIO_PIN ledPin;

int cw, ccw, err;
class MyRotaryHandler : public RotaryHandler
{
public:
    MyRotaryHandler (PIT* pit, GPIO_PIN* A, GPIO_PIN* B) : RotaryHandler (pit, A, B)
    {
    }

    virtual void OnStateChange(int8_t dir)
    {
        //GPIO_TogglePin(&ledPin);
        //_printf ("dir:%d\n", dir);
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
#ifdef SEMIHOSTING
            _printf ("cw:%d, ccw:%d, err: %d\n", cw, ccw, err);
#endif
            cw = 0;
            ccw = 0;
            err = 0;
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
    GPIO_PIN pinB = GPIO_Helper_GetPin("A5");


    MyRotaryHandler bh (pit, &pinA, &pinB);
    bh.SetUpdateIntervalUs(500);
    bh.Init(true);
    while(1)
        ;
}
