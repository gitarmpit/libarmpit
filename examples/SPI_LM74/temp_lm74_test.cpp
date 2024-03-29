#include "gpio_cpp.h"
#include "TempLM74.h"

static void temp_lm74_test()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOA::GetInstance()->SetupGPIO_OutPP(GPIO_PIN4); //NSS  slave select

    SPI1* spi1 = SPI1::GetInstance();
    spi1->Enable(true);

    //spi1->SetSlaveSelectHigh();

    bool isMaster = true;
    bool isFullDuplex = true;
    bool isClockPhase2 = true;
    bool isClockPolarity1 = true;
    bool isLSBFirst = false;
    bool isSetSSM = true;
    bool initializeGPIO = true;

    spi1->Initialize(isMaster, isFullDuplex, SPI::SPI_BAUD_RATE_256,
            isClockPhase2, isClockPolarity1, isLSBFirst, isSetSSM,
            initializeGPIO);

    volatile uint8_t reg8 = 0;
        spi1->SetSlaveSelectLow();
        reg8 = spi1->Transmit (0x0);
        reg8 = spi1->Transmit (0x0);
        spi1->SetSlaveSelectHigh();
    
    
    //spi1->PullDownInputPin();

    TempLM74 lm74(spi1);
    lm74.Init();

    //volatile uint16_t code = lm74.GetProductCode();

    //lm74.Shutdown();

    volatile float t;

    while (true)
    {
        t = lm74.GetTemp();
    }

}

int main()
{
    RCC_SetAHBPrescalerDiv4();
    RCC_SetAPB1PrescalerDiv1();
    RCC_SetAPB2PrescalerDiv1();
    RCC_SetADCPrescalerDiv2();
    RCC_EnableHSI(3);

    temp_lm74_test();
}
