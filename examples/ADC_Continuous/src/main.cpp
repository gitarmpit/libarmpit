#include "adc_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"

class ADC1_IRQ_Handler: public ADC_IRQ_Handler
{
private:
    //GPIO_PIN* _pin;

public:

    ADC1_IRQ_Handler()
    {
        //GPIOB* gpioB = GPIOB::GetInstance();
        //gpioB->EnablePeripheralClock(true);
        //_pin = gpioB->GetPin(GPIO_PIN12);
        //_pin->SetupGPIO_OutPP();
    }

    virtual void Handle(ADC* adc)
    {

        static volatile uint32_t d;
        d = adc->GetData();
//        if (d < 2000)
//        {
//            GPIOB::GetInstance()->SetPin(GPIO_PIN12);
//        }
//        else
//        {
//            GPIOB::GetInstance()->ResetPin(GPIO_PIN12);
//        }
    }
};


void Test_Continuous_Conversion()
{
#if defined(STM32F1)
    RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();
    // RCC_EnableHSI_24Mhz();
#elif defined(STM32F4)
    RCC_EnableHSI_168Mhz();
    RCC_SetAPB2PrescalerDiv16();
    //RCC_EnableHSI(true);
#else
#error "MCU not defined"
#endif

    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN0);


    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->AddConversion(ADC_IN0, ADC_SMP_SLOWEST);
    adc1->EnableInterrupt(true);
    static ADC1_IRQ_Handler adcHandler;
    adc1->SetInterruptHandler((ADC1_IRQ_Handler*)&adcHandler);
    adc1->StartContinuousConversion();

    while (1)
        ;
}

int main()
{
    Test_Continuous_Conversion();
}
