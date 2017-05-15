#include "spi_lcd_setup.h"

SPI_LCD_Setup::SPI_LCD_Setup()
{
    //LCD
    GPIOB* portB = GPIOB::GetInstance();
    GPIOC* portC = GPIOC::GetInstance();

    GPIO_PIN* CLK_pin = portB->GetPin (GPIO_PIN3);
    GPIO_PIN* MOSI_pin = portB->GetPin (GPIO_PIN5);

    MOSI_pin->SetupGPIO_OutAltPP();
    CLK_pin->SetupGPIO_OutAltPP();

    bool isFullDuplex = true;
    bool isLSBFirst = false;
    bool isSetSSM = true;
    bool is16Bit = false;

    _spiLCD = SPI1::GetInstance();
    _spiLCD->Initialize(true, isFullDuplex, SPI_BAUD_RATE_2,  false, false, isLSBFirst, isSetSSM, is16Bit);

    //not used
    _rstPin = portB->GetPin(GPIO_PIN0);
    //_rstPin->SetupGPIO_OutPP();
    //_rstPin->SetSpeedHigh();

    //4
    _dcPin = portC->GetPin(GPIO_PIN15);
    _dcPin->SetupGPIO_OutPP();
    _dcPin->SetSpeedHigh();

    //2
    _ssPin = portC->GetPin(GPIO_PIN13);
    _ssPin->SetupGPIO_OutPP();
    _ssPin->SetSpeedHigh();

}
