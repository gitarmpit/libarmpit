#include "adc_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "dma_cpp.h"
#include <stdio.h>
#include <string.h>
#include "debug.h"

class ADC1_IRQ_Handler: public ADC_IRQ_Handler
{
private:
    GPIO_PIN* _pin;

public:

    ADC1_IRQ_Handler()
    {
    }

    virtual void Handle(ADC* adc)
    {
    }
};

class DMA1_IRQ_Handler: public DMA_IRQ_Handler
{
private:
    uint16_t data[16];
public:
    DMA1_IRQ_Handler() 
    {
            memset (data, 0, sizeof(data));
    }
    

    virtual void HandleInterrupt(DMA_Channel* channel, bool isTransferComplete,
            bool isHalfTransfer, bool isTransferError)
    {
        static volatile int i;
        if (isTransferComplete)
        {
#ifdef SEMIHOSTING
            //printf ("%d %d %d %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
            printf ("%d\n", data[0]);
#endif
            ++i;
        }
    }

    virtual ~DMA1_IRQ_Handler()
    {
    }

    void* getDataPtr() { return data; }

};



void Test_DMA()
{

    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN0);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN1);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN2);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN3);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN4);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN5);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN6);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN7);

    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->SetScanMode();
    adc1->AddConversion(ADC_IN0, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN1, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN2, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN3, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN4, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN5, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN6, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN7, ADC_SMP_SLOWEST);

    //adc1->EnableInterrupt(true);
    //ADC1_IRQ_Handler adcHandler;
    //adc1->SetInterruptHandler(&adcHandler);

    DMA1_IRQ_Handler irqHandler;

    DMA1* dma1 = DMA1::GetInstance();
    dma1->EnablePeripheralClock(true);

    DMA_Channel* dmaCh = dma1->GetChannel(DMA_CHANNEL_1);
    dma1->SetIRQHandler (&irqHandler);
    dmaCh->EnableTransferCompleteInterrupt();
    dmaCh->EnableTransferErrorInterrupt();


    adc1->EnableDMA(dmaCh, 8, irqHandler.getDataPtr());
    adc1->StartContinuousConversion();


    while (1)
        ;
}

static void Test_Analog_As_Digital()
{
    GPIOC* gpioC = GPIOC::GetInstance();
    gpioC->EnablePeripheralClock(true);
    GPIO_PIN* pin = gpioC->GetPin(GPIO_PIN14);
    pin->SetupGPIO_InPullUp();
    while(1)
    {
#ifdef SEMIHOSTING
        printf ("%d\n, ", pin->IsSet());
#endif
        delay(100);
    }

}

#ifdef SEMIHOSTING

extern "C" void initialise_monitor_handles(void);

#endif

int main()
{
#ifdef SEMIHOSTING
    initialise_monitor_handles();
    printf ("test semihosting\n");
 #endif
    RCC_EnableHSI_24Mhz();



    //Test_DMA();
    Test_Analog_As_Digital();

}
