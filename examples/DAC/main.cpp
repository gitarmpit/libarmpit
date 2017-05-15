#include "dac_cpp.h"
#include <math.h>
#include "gpio_cpp.h"
#include "dma_cpp.h"
#include "RCC_Helper.h"
#include "timer_cpp.h"
#include "GPIO_Helper.h"
#include "spi.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "lcd5110.h"
#include "debug.h"



//#include "5110/Adafruit_PCD8544.h"

//84Mhz timer: 90 steps: 10us (100K) at reload=8-9
//84Mhz timer: 70 steps: 8us (125K) at reload=6

static const uint32_t nsteps = 90;
static uint16_t data[nsteps];

class DAC_IRQ_Handler1: public DAC_IRQ_Handler
{
public:
    virtual void HandleInterrupt(DAC_CHANNEL channelNo)
    {
    }
};

static void GenerateData()
{
    for (uint32_t i = 0; i <= nsteps; i += 1)
    {
        data[i] = (int) ((sin(2 * 3.14159 * (float) i / (float) nsteps) + 1.)
                * (float) (0xfff + 1) / 2.);
    }
}

static void DAC_Test_Triangle()
{

    GenerateData();

    GPIOA* portA = GPIOA::GetInstance();
    portA->EnablePeripheralClock(true);
    portA->SetupGPIO_InAnalog(GPIO_PIN4); //DAC pin

    DAC* dac = DAC::GetInstance();
    dac->EnablePeripheralClock(true);
    DAC_Channel* ch = dac->GetChannel(DAC_CHANNEL_1);
    ch->SelectDataRegister12R();
    ch->SetAmplitude(DAC_CR_AMPL_2047);
    ch->EnableTriangleWaveGeneration();
    ch->EnableChannel();
    //ch->Write

}

static void DAC_Test_DMA()
{

    
    GenerateData();

    GPIOA* portA = GPIOA::GetInstance();
    portA->EnablePeripheralClock(true);
    portA->SetupGPIO_InAnalog(GPIO_PIN4); //DAC pin

    DAC* dac = DAC::GetInstance();
    dac->EnablePeripheralClock(true);
    dac->SetIrqHandler(new DAC_IRQ_Handler1());

    DAC_Channel* dacCh = dac->GetChannel(DAC_CHANNEL_1);
    dacCh->SelectTriggerTimer8();
    //dacCh->EnableUnderrunInterrupt();
    dacCh->SelectDataRegister12R();

    uint32_t dacDataAddr = (uint32_t) dacCh->GetDataRegisterAddress();

    DMA1* dma1 = DMA1::GetInstance();
    dma1->EnablePeripheralClock(true);

#ifdef STM32F1
    DMA_Channel* dmaCh3 = dma1->GetChannel(DMA_CHANNEL_3);
    dmaCh3->SetPeripheralAddress(dacDataAddr);
    dmaCh3->SetMemoryAddress((uint32_t) data);
    dmaCh3->SetCircularMode();
    dmaCh3->SetDirection_MemoryToPeripheral();
    dmaCh3->SetMemorySize_16bits();
    dmaCh3->SetPeripheralSize_16bits();
    dmaCh3->SetNumberOfData(nsteps);
    dmaCh3->SetMemoryIncrementMode();
    dmaCh3->SetPriorityHigh();
#elif defined(STM32F4)
    DMA_Stream* s = dma1->GetStream(DMA_STREAM_5);
    s->SetChannel (DMA_CHANNEL_7);
    s->SetPeripheralAddress(dacDataAddr);
    s->SetMemory0Address((uint32_t) data);
    s->SetCircularMode(TRUE);
    s->SetDirection_MemoryToPeripheral();
    s->SetMemorySize_16bits();
    s->SetPeripheralSize_16bits();
    s->SetNumberOfData(nsteps);
    s->SetMemoryIncrementMode(TRUE);
    s->SetPriorityVeryHigh();
    s->EnableStream(TRUE);
#endif


    dacCh->EnableChannel();
    dacCh->EnableDMA();

    TIM8* tim = TIM8::GetInstance();
    tim->EnablePeripheralClock(TRUE);

    uint32_t sin_period_us = 8;

    uint32_t update_period_ns = sin_period_us * 1000 / nsteps;

    //tim6->SetUpdatePeriod_us(1); //84=100us, 21=25us, 8~11us
    //tim6->SetAutoReloadValue(6);    
    tim->SetUpdatePeriod_ns (update_period_ns);
    
    tim->EnableAutoPreload(TRUE);
    tim->SetMMS_Update();
    tim->EnableCounter(TRUE);

    while(1) 
        ;

}

static void DAC_Test_manual()
{

    GenerateData();

    GPIOA* portA = GPIOA::GetInstance();
    portA->EnablePeripheralClock(true);
    portA->SetupGPIO_InAnalog(GPIO_PIN4); //DAC pin

    DAC* dac = DAC::GetInstance();
    dac->EnablePeripheralClock(true);

    DAC_Channel* dacCh = dac->GetChannel(DAC_CHANNEL_1);
    dacCh->SelectDataRegister12R();

    uint32_t dacDataAddr = (uint32_t) dacCh->GetDataRegisterAddress();

    DMA1* dma1 = DMA1::GetInstance();
    dma1->EnablePeripheralClock(true);

    dacCh->EnableChannel();

    uint32_t i = 0;
    while (1) 
    {
        *((uint32_t*)dacDataAddr) = data [ i % nsteps ];
        ++i;
    }

    while(1) 
        ;

}

static void Test_LCD()  
{
    delay(10);
    GPIOC* portC = GPIOC::GetInstance();
    GPIOD* portD = GPIOD::GetInstance();
    portC->EnablePeripheralClock(true);
    portD->EnablePeripheralClock(true);

    SPI* spi = GPIO_Helper::SetupSPI3_MasterMode_PC_10_11_12(false, false, SPI_BAUD_RATE_2);

    GPIO_PIN* rstPin = portD->GetPin(GPIO_PIN2);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* ssPin = portD->GetPin(GPIO_PIN0);
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

    GPIO_PIN* dcPin = portD->GetPin(GPIO_PIN1);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();



    LCD5110 display(spi, dcPin, rstPin, ssPin, false);

    display.Init();
    display.Clear();

    display.WriteLine("12345");
//    display.GotoXY(5, 1);
//    display.WriteLine("12345");
//    display.GotoXY(0, 1);
//    display.WriteLine("1");
//    display.GotoXY(1, 2);
//    display.WriteLine("23");

    uint32_t i = 0;
    char buf[32] = {0};
    while(6)
    {
      //  display.GotoXY(0, 0);
      //  sprintf (buf, "%d", i++);
      //  display.WriteLine(buf);
      //  delay(10);
    }

    //display.WriteLine("test1!");
    //display.GotoXY(0, 1);
    //display.WriteLine("test2!");
    //display.GotoXY(0, 2);
    //display.WriteLine("test3!");
    //display.GotoXY(0, 3);
    //display.WriteLine("1234567890 123456789");

    while(1)
        ;

}


int main()
{

    RCC_EnableHSI_168Mhz();
    Test_LCD();

    DAC_Test_DMA();
}
