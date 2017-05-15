#define _MULTI_REGISTER_ACCEL_READ

#include "lis3dsh.h"

LIS3DSH::LIS3DSH (SPI* spi, GPIO_PIN* SS_pin)
{
    _spi = spi;
    _SS_pin = SS_pin;
    Init();
}


void LIS3DSH::Init(void)
{
    // Normal power mode, all axes enabled, 50 Hz ODR
    WriteByte(LIS3DSH_CTRL_REG4, 0x5F);
    // 200 Hz antialias filter, +/- 2g FS range
    WriteByte(LIS3DSH_CTRL_REG5, 0x80);
    // configure FIFO for bypass mode
    WriteByte(LIS3DSH_FIFO_CTRL_REG, 0);
    // disable FIFO, enable register address auto-increment
    WriteByte(LIS3DSH_CTRL_REG6, 0x10);
}

uint8_t LIS3DSH::ReadID(void)
{
	return ReadByte(LIS3DSH_WHO_AM_I_ADDR);
}


void LIS3DSH::ReadAccel(int16_t *x, int16_t *y, int16_t *z)
{
	int8_t buf[6];

	buf[0] = ReadByte(LIS3DSH_OUT_X_L);
	buf[1] = ReadByte(LIS3DSH_OUT_X_H);
	buf[2] = ReadByte(LIS3DSH_OUT_Y_L);
	buf[3] = ReadByte(LIS3DSH_OUT_Y_H);
	buf[4] = ReadByte(LIS3DSH_OUT_Z_L);
	buf[5] = ReadByte(LIS3DSH_OUT_Z_H);

	*x = (int16_t)(buf[0] << 8 | buf[1]);
	*y = (int16_t)(buf[2] << 8 | buf[3]);
	*z = (int16_t)(buf[4] << 8 | buf[5]);
}


int8_t LIS3DSH::ReadTemp()
{
    return ReadByte(LIS3DSH_OUT_TEMP);
}


void LIS3DSH::SetFullScaleRange(uint8_t range)
{
    uint8_t val;

    val = ReadByte(LIS3DSH_CTRL_REG5);

    val &= ~(0x38);
    val |= ((range & 0x07) << 3);

    WriteByte(LIS3DSH_CTRL_REG5, val);
}


//void LIS3DSH::SetBandwidth(uint8_t Bandwidth)
//{
//    uint8_t val;
//
//    val = ReadByte(LIS3DSH_CTRL_REG5);
//
//    val &= ~(0xC0);
//    val |= ((Bandwidth & 0x03) << 6);
//
//    WriteByte(LIS3DSH_CTRL_REG5, val);
//}
//
//
//void LIS3DSH::SetDataRate(uint8_t rate)
//{
//    uint8_t val;
//
//    val = ReadByte(LIS3DSH_CTRL_REG4);
//
//    val &= ~(0xF0);
//    val |= ((rate & 0x0f) << 4);
//
//    WriteByte(LIS3DSH_CTRL_REG4, val);
//}
//


void LIS3DSH::WriteByte(uint8_t data, uint8_t addr)
{
  _SS_pin->Reset();
  _spi->TransmitByte(addr);
  _spi->TransmitByte(data);
  _SS_pin->Set();
  
}

uint8_t LIS3DSH::ReadByte(uint8_t addr)
{  
	addr |= 0x80;
	_SS_pin->Reset();
  
	_spi->TransmitByte(addr);
  
	uint8_t res = _spi->TransmitByte(0x0);
	_SS_pin->Set();

	return res;
}

