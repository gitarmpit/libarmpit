#include "Pressure_MPL311.h"
#include "GPIO_Helper.h"
#include "i2c_cpp.h"
#include "RCC_Helper.h"
#include <stdio.h>
#include "debug.h"

void* error (void* arg)
{
    static int count = 0;
    ++count;

}
static void test_pressure()
{
    I2C* i2c =  GPIO_Helper::SetupI2C(I2C1_PB_6_7, 100000);
    i2c->SetErrorCallback(error, 0);

    bool isAltimeterMode = true;
    bool isAutoAquisition = false;
    Pressure_MPL311 mpl311(i2c, isAltimeterMode, isAutoAquisition, OSR_1);
    uint8_t id = mpl311.GetDeviceID();
    mpl311.GetDeviceID();
    mpl311.GetDeviceID();
    mpl311.GetDeviceID();
    mpl311.GetDeviceID();
    id = mpl311.GetDeviceID();

    static volatile float result1;
    static volatile float pa;
    static volatile float t;
    static volatile float tpa = 0.;
    while (1)
    {
        tpa = 0.;

        for (uint i = 0; i < 1; ++i)
        {
            pa = mpl311.GetPressureAltitude();
            tpa += pa;
        }

        tpa /= 1.;

        //t = mpl311.GetTemperature();
        //result1 = mpl311.GetPressure() *  0.00750061561303; //mm of mercury
        //result2 = mpl311.GetPressure() * 0.000295333727; //inches of mercury



        volatile float delta = (tpa - (int)tpa);
        volatile int idelta = delta * 10.;
#ifdef SEMIHOSTING
        printf ("%d.%d\n", (int)pa, idelta);
#endif
        delay(1000);
    }
}


extern "C" void initialise_monitor_handles(void);

int main()
{
#ifdef SEMIHOSTING
    initialise_monitor_handles();
 #endif
    RCC_EnableHSI(TRUE);


    test_pressure();
}
