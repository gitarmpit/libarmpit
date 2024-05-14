#ifndef _I2C_NAU7802_H
#define _I2C_NAU7802_H

#include "i2c_cpp.h"

#define REG0  0x00
#define REG1  0x01
#define REG2  0x02
#define REG12 0x12   //ADC 23-16
#define REG13 0x13   //ADC 15-8
#define REG14 0x14   //ADC 7-0
#define REG1F 0x1F   //revision iD

#define REG0_RR    1<<0
#define REG0_PUD   1<<1
#define REG0_PUA   1<<2
#define REG0_PUR   1<<3
#define REG0_CS    1<<4
#define REG0_CR    1<<5
#define REG0_OSCS  1<<6
#define REG0_AVDDS 1<<7

#define REG2_CALS     1<<2
#define REG2_CAL_ERR  1<<3
#define REG2_CHS      1<<7  //channel select

#define NAU7802_ADDR  0x2a
#define CAL_INTERNAL 0x0
#define CAL_OFFSET   0x2
#define CAL_GAIN     0x3

class I2C_Nau7802: public I2C_ErrorHandler
{
private:
    I2C* _i2c;
    bool _initialized;

    bool Calibrate(uint8_t mode);

protected:
    virtual void HandleError();

public:

    I2C_Nau7802(I2C* i2c);

    void InitializeSensor();
    uint32_t Convert();

};

#endif
