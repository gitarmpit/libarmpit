#include "adc_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "debug.h"

static void TestSingleConversion() 
{
#if defined(STM32F1)
	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#elif defined(STM32F4)
	RCC_EnableHSI_168Mhz();
    RCC_SetAPB2PrescalerDiv8();
#endif

    GPIOB* gpioB = GPIOB::GetInstance();
    gpioB->EnablePeripheralClock(true);
    //LED on pin25
    gpioB->SetupGPIO_OutPP(GPIO_PIN12);

    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN1);
    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    //adc1->SetPrescalerDIV8();
    adc1->AddConversion (ADC_IN1, ADC_SMP_FAST);
    volatile uint32_t res1;
    while (1) 
    {
        res1 = adc1->SingleConversion();
        if (res1 < 2000)
        {
            gpioB->SetPin(GPIO_PIN12);
        }
        else
        {
            gpioB->ResetPin(GPIO_PIN12);
        }
        delay_us(100);
    }







}

int main()
{

    TestSingleConversion();

}
