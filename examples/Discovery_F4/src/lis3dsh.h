#ifndef _LIS3DSH_H
#define _LIS3DSH_H

#include "spi_cpp.h"
#include "gpio_cpp.h"

#define LIS3DSH_WHO_AM_I_ADDR   0x0F
#define LIS3DSH_CTRL_REG3    	0x23
#define LIS3DSH_CTRL_REG4    	0x20
#define LIS3DSH_CTRL_REG5    	0x24
#define LIS3DSH_CTRL_REG6    	0x25
#define LIS3DSH_OUT_TEMP     	0x0C

#define LIS3DSH_OUT_X_L      	0x28
#define LIS3DSH_OUT_X_H      	0x29
#define LIS3DSH_OUT_Y_L      	0x2A
#define LIS3DSH_OUT_Y_H      	0x2B
#define LIS3DSH_OUT_Z_L      	0x2C
#define LIS3DSH_OUT_Z_H      	0x2D

#define LIS3DSH_FIFO_CTRL_REG	0x2E

#define LIS3DSH_ID              0x3F

class LIS3DSH
{
private:
    SPI*   _spi;
    GPIO_PIN* _SS_pin;

    uint8_t ReadByte(uint8_t addr);
    void    WriteByte(uint8_t data, uint8_t addr);

    void Init();

public:

    LIS3DSH (SPI* spi, GPIO_PIN* SS_pin);

    uint8_t  ReadID(void);
    void     SetFullScaleRange(uint8_t range);
    void     ReadAccel(int16_t *x, int16_t *y, int16_t *z);
    void     SetDataRate(uint8_t rate);
    int8_t   ReadTemp();
};


#endif
