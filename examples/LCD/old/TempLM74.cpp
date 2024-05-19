


#include "TempLM74.h"

void TempLM74::Init()
{

    _spi->SetSlaveSelectLow();

    _spi->Transmit(0x00);
    _spi->Transmit(0x00);
    _spi->Transmit(0x00);
    _spi->Transmit(0x00);

    _spi->SetSlaveSelectHigh();

}

uint16_t TempLM74::GetProductCode()
{
    Shutdown();
    _spi->SetSlaveSelectLow();
    uint16_t res = (_spi->Transmit(0xff) << 8) & 0xff00;
    res |= _spi->Transmit(0xff);
    _spi->SetSlaveSelectHigh();
    return (res >> 2) & 0x3FFF;
}

void TempLM74::Shutdown()
{
    _spi->SetSlaveSelectLow();

    _spi->Transmit(0xff);
    _spi->Transmit(0xff);
    _spi->Transmit(0xff);
    _spi->Transmit(0xff);

    _spi->SetSlaveSelectHigh();

}

float TempLM74::GetTemp()
{

    static uint8_t tmp;
    static int16_t res;
    static uint8_t bits = 13;

    _spi->SetSlaveSelectLow();

    //_spi->Write(0x00);
    //_spi->Write(0x00);

    res = (_spi->Transmit(0x00) << 8) & 0xff00;
    tmp = _spi->Transmit(0x00);

    _spi->SetSlaveSelectHigh();

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

