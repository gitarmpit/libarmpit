#include "TempLM74.h"

static SPI* _spi;
static GPIO_PIN* _ssPin;

static void init();

void LM74_Init(SPI* spi, GPIO_PIN* ssPin)
{
    _spi = spi;
    _ssPin = ssPin;
    init();
}

static void init()
{
    GPIO_ResetPin(_ssPin);

    SPI_TransmitByte(_spi, 0x00);
    SPI_TransmitByte(_spi, 0x00);
    SPI_TransmitByte(_spi, 0x00);
    SPI_TransmitByte(_spi, 0x00);

    GPIO_SetPin(_ssPin);

}

uint16_t LM74_GetProductCode()
{
    LM74_Shutdown();
    GPIO_ResetPin(_ssPin);
    uint16_t res = (SPI_TransmitByte(_spi, 0xff) << 8) & 0xff00;
    res |= SPI_TransmitByte(_spi, 0xff);
    GPIO_SetPin(_ssPin);
    return (res >> 2) & 0x3FFF;
}

void LM74_Shutdown()
{
    GPIO_ResetPin(_ssPin);

    SPI_TransmitByte(_spi, 0xff);
    SPI_TransmitByte(_spi, 0xff);
    SPI_TransmitByte(_spi, 0xff);
    SPI_TransmitByte(_spi, 0xff);

    GPIO_SetPin(_ssPin);

}

float LM74_GetTemp()
{
    static uint8_t tmp;
    static int16_t res;
    static uint8_t bits = 13;

    float realTemp;

    GPIO_ResetPin(_ssPin);

    //g_spi->Write(0x00);
    //g_spi->Write(0x00);

    res = (SPI_TransmitByte(_spi, 0x00) << 8) & 0xff00;
    tmp = SPI_TransmitByte(_spi, 0x00);

    GPIO_SetPin(_ssPin);

    if (tmp & 0x4)
    {
        res |= tmp;
        res = (res >> 3) & 0x1FFF;
        if (res & (1 << (bits - 1)))
        {
            res -= (1 << bits);
        }

        realTemp = ((float) (res)) * 0.0625;
    }
    else
    {
        realTemp = -60.;
        init();
    }

    return realTemp;
}

