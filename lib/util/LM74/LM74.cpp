#include "LM74.h"

void LM74::Init()
{

    _ssPin->Reset();

    _spi->TransmitByte(0x00);
    _spi->TransmitByte(0x00);
    _spi->TransmitByte(0x00);
    _spi->TransmitByte(0x00);

    _ssPin->Set();

}

uint16_t LM74::GetProductCode()
{
    Shutdown();
    _ssPin->Reset();
    uint16_t res = (_spi->TransmitByte(0xff) << 8) & 0xff00;
    res |= _spi->TransmitByte(0xff);
    _ssPin->Set();
    return (res >> 2) & 0x3FFF;
}

void LM74::Shutdown()
{
    _ssPin->Reset();

    _spi->TransmitByte(0xff);
    _spi->TransmitByte(0xff);
    _spi->TransmitByte(0xff);
    _spi->TransmitByte(0xff);

    _ssPin->Set();

}

float LM74::GetTemp()
{

    static uint8_t tmp;
    static int16_t res;
    static uint8_t bits = 13;

    _ssPin->Reset();

    //_spi->Write(0x00);
    //_spi->Write(0x00);

    res = (_spi->TransmitByte(0x00) << 8) & 0xff00;
    tmp = _spi->TransmitByte(0x00);

    _ssPin->Set();

    if (tmp & 0x4)
    {
        res |= tmp;
        res = (res >> 3) & 0x1FFF;
        if (res & (1 << (bits - 1)))
        {
            res -= (1 << bits);
        }

        _realTemp = ((float) (res)) * 0.0625;
    }
    else
    {
        _realTemp = -60.;
        Init();
    }

    return _realTemp;

}

