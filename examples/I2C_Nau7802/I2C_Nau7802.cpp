/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 igorS
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#include "I2C_Nau7802.h"

I2C_Nau7802::I2C_Nau7802(I2C* i2c)
{
    _i2c = i2c;
    _initialized = false;
}

bool I2C_Nau7802::Calibrate(uint8_t mode)
{
    uint8_t byte = mode;

    byte |= 1 << 2;  //start cal
    _i2c->MasterWrite(NAU7802_ADDR, 0x2, byte);

    while (1)
    {
        _i2c->MasterRead(NAU7802_ADDR, 0x2, 1, &byte);

        if (byte & (1 << 3))
        {
            return false;
        }

        if (!(byte & (1 << 2)))
        {
            break;
        }
    }

    return true;

}

void I2C_Nau7802::InitializeI2C()
{
    //_i2c->SetErrorHandler(this);

    bool initializeGPIO = true;
    _i2c->Initialize(200000, false, false, initializeGPIO);
    _i2c->SetModeI2C();
    _i2c->EnableACK(true);
    _i2c->SetOwnAddr7Bit(0x00);

}

void I2C_Nau7802::InitializeSensor()
{
    if (!_initialized)
    {
        InitializeI2C();
        _initialized = true;
    }

    uint8_t byte;
    byte = REG0_RR;
    _i2c->MasterWrite(NAU7802_ADDR, REG0, byte);

    byte = REG0_PUD | REG0_PUA;
    _i2c->MasterWrite(NAU7802_ADDR, REG0, byte);

    while (1)
    {
        _i2c->MasterRead(NAU7802_ADDR, REG0, 1, &byte);

        if (byte & REG0_PUR) //powerup ready
        {
            break;
        }
    }

#if 0
    //PGA bypass
    byte = 0x10;
    //i2c1->MasterWrite(slave_addr, 0x1b, byte);

    //LDO MODE
    byte = 1<<6 | 1;//+ chopper disabled
    //i2c1->MasterWrite(slave_addr, 0x1b, byte);

    //gain
    byte = 0x3;
    //i2c1->MasterWrite(slave_addr, 0x1, byte);

    //conversion rate
    byte = 0x7<<4;
    //i2c1->MasterWrite(slave_addr, 0x2, byte);

    byte = 2<<2;
    //i2c1->MasterWrite(slave_addr, 0x15, byte);

#endif

    //while (!Calibrate(CAL_INTERNAL))
    {
    }

}

uint32_t I2C_Nau7802::Convert()
{
    if (!_initialized)
    {
        InitializeI2C();
        InitializeSensor();
        _initialized = true;
    }

    uint32_t res = 0;
    uint8_t data[3];

    while (1)
    {
        _i2c->MasterRead(NAU7802_ADDR, REG0, 1, data);
        if (!(data[0] & REG0_PUR))
        {
            InitializeSensor();
            continue;
        }
        if ((data[0] & REG0_CR)) //conversion ready
        {
            break;
        }

    }

    _i2c->MasterRead(NAU7802_ADDR, REG12, 3, data);

    res = ((data[0] << 16) | (data[1] << 8) | data[2]) & ~0xff000000;
    return res;
}

void I2C_Nau7802::HandleError()
{
    InitializeI2C();
}

