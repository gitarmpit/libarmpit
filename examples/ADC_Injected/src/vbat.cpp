#include <stdio.h>
#include "fp2.h"
#include "adc_cpp.h"
#include "gpio_cpp.h"
#include "debug.h"

//This is the working example for a 1s lipo battery

//stm32f103c8 at 64Mhz RC  Vref ADC = 1482, Vref = 1.194

//Vref = 3.3 * adc_res / 4095;  3.3*1482/4096= 1.194

//Vcc = Vref / adc_res * 4096

//3.77/4096*3131*6.87/4.87
//Vbat = Vcc/4096 * adc_R2  * (R1+R2) / R2


void bat(void)
{
    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN2);
    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->EnableTempVref(true);

    //R2
    adc1->AddJConversion (ADC_IN2, ADC_SMP_SLOWEST);

    adc1->AddJConversion (ADC_VREFINT, ADC_SMP_SLOWEST);
    adc1->SetAutoInjection(true);
    adc1->SetScanMode();

    char buf[8];
    volatile uint32_t adc_vref, adc_r2;
    volatile float  vcc, vbat;
    const static float vref = 1.194;
    const float R1 = 2.0, R2 = 4.87;

    while (1)
    {
        adc1->SingleJConversion();
        adc_r2 = adc1->GetJData2();
        adc_vref = adc1->GetJData1();
        vcc = vref / adc_vref * 4095.;
        vbat = vcc / 4095. * adc_r2 * (R1 + R2) / R2;
#ifdef SEMIHOSTING
        fix16_to_str (fix16_from_float(vbat), buf, 3);
        printf ("vbat=%s\n", buf);
#endif
        delay(100);
    }

}
