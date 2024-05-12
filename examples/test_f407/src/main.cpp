#include "adc_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include <stdio.h>
#include "debug.h"

class ADC_IRQ_JHandler_Test: public ADC_IRQ_Handler
{
public:

    virtual void Handle(ADC* adc)
    {
        static volatile uint32_t d0;
        static volatile uint32_t d1;

        d0 = adc->GetJData1();
        d1 = adc->GetJData2();
#ifdef SEMIHOSTING
        printf ("%d %d\n", d0, d1);
#endif
        delay(30);
    }
};


static void Test_Injected1()
{
    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);

    gpioA->SetupGPIO_InAnalog(GPIO_PIN0);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN1);

    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->AddJConversion  (ADC_IN0, ADC_SMP_SLOWEST);
    adc1->AddJConversion  (ADC_IN1, ADC_SMP_SLOWEST);

    ADC_IRQ_JHandler_Test  handler;
    adc1->SetJInterruptHandler(&handler);

    adc1->EnableJInterrupt(true);
    adc1->SetAutoInjection(true);
    adc1->SetScanMode();
    adc1->StartContinuousConversion();
    while (1)
        ;
}

static void Test_Injected2()
{
    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);

    gpioA->SetupGPIO_InAnalog(GPIO_PIN0);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN1);

    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->AddJConversion  (ADC_IN0, ADC_SMP_SLOWEST);
    adc1->AddJConversion  (ADC_IN1, ADC_SMP_SLOWEST);
    
    adc1->SetAutoInjection(true);
    adc1->SetScanMode();
    static volatile uint32_t d0;
    static volatile uint32_t d1;
    while (1)
    {
        adc1->SingleJConversion();
        d0 = adc1->GetJData1();
        d1 = adc1->GetJData2();
#ifdef SEMIHOSTING
        printf ("%d %d\n", d0, d1);
#endif
        delay(30);

    }

}



#ifdef SEMIHOSTING
extern "C" void initialise_monitor_handles(void);
#endif

int main()
{
#ifdef SEMIHOSTING
    initialise_monitor_handles();
#endif

#if defined(STM32F1)
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#elif defined(STM32F4)
    RCC_EnableHSI_168Mhz();
#else
#error "MCU not defined"
#endif

    Test_Injected2();

}
