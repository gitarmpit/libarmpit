#include "GPIO_Helper.h"
#include "RCC_Helper.h"
#include "debug.h"
#include "afio.h"
#include "adc_cpp.h"
#include "gpio_cpp.h"

static void setup_ADC(ADC_IRQ_Handler* adcHandler)
{
    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN0);
    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->AddConversion(ADC_IN0, ADC_SMP_SLOWEST);
    adc1->EnableInterrupt(true);
    adc1->SetInterruptHandler(adcHandler);
    adc1->StartContinuousConversion();
}

int main()
{
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
    Debug_EnableCYCCNT(TRUE);
    while(1)
      ;
}
