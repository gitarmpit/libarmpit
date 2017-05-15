#ifndef _SPI_LCD_SETUP_H
#define _SPI_LCD_SETUP_H

#include "spi_cpp.h"

class SPI_LCD_Setup
{
private:
    SPI* _spiLCD;
    GPIO_PIN* _rstPin;
    GPIO_PIN* _dcPin;
    GPIO_PIN* _ssPin;

public:
    SPI_LCD_Setup();

    SPI* GetSPI() { return _spiLCD; }
    GPIO_PIN* GetRstPin() { return _rstPin; }
    GPIO_PIN* GetDcPin() { return _dcPin; }
    GPIO_PIN* GetSsPin() { return _ssPin; }
};


#endif
