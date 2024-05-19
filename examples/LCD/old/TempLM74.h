


#ifndef _TEMPLM74_H
#define _TEMPLM74_H

#include "spi_cpp.h"

/*
 *
 *    Vdd  /CS
 *     8    7     6    5
 *
 *     1    2     3    4
 *    SI/O  CLK        GND
 *
 *
 *   SPI1:  NSS A4, CLK A5, MISO A6, MOSI A7
 *
 */

class TempLM74
{
private:
    SPI* _spi;

    float _realTemp; //initial invalid

public:

    TempLM74(SPI* spi)
    {
        _spi = spi;
        _realTemp = -60;
    }

    void Init();
    void Shutdown();

    uint16_t GetProductCode();

    float GetTemp();

};

#endif
