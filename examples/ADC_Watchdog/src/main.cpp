#include "adc_cpp.h"

#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "debug.h"

class ADC_WatchdogHandler: public ADC_IRQ_Handler
{
public:

    //if input voltage is is below 1V, turn on the LED briefly
    virtual void Handle(ADC* adc)
    {
        static volatile uint32_t d;
        d = adc->GetData();
        GPIOB::GetInstance()->SetPin(GPIO_PIN12);
        delay(500);
        GPIOB::GetInstance()->ResetPin(GPIO_PIN12);
    }
};

static void Test_Watchdog()
{
    
    GPIOB* gpioB = GPIOB::GetInstance();
    gpioB->EnablePeripheralClock(true);
    //LED on pin25
    gpioB->SetupGPIO_OutPP(GPIO_PIN12);
    
    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->SetupGPIO_InAnalog(GPIO_PIN3);
    
    ADC1* adc = ADC1::GetInstance();
    adc->Enable(true);
    adc->AddConversion(ADC_IN3, ADC_SMP_SLOW); //pin13
    ADC_WatchdogHandler wdh;

    float min_volt = 0.0;
    float max_volt = 1.0;
    adc->EnableWatchdog(min_volt, max_volt, &wdh, ADC_IN3);
    adc->EnableInterrupt(true);
    adc->StartContinuousConversion();
    while (1)
        ;
}



int main()
{
#if defined(STM32F1)
        RCC_EnableHSI_24Mhz();
#elif defined(STM32F4)
    RCC_EnableHSI_168Mhz();
#else
#error "MCU not defined"
#endif

        Test_Watchdog();
    
}
