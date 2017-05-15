#include "i2c_wrapper.h"
#include "debug.h"
I2C* g_i2c;

void i2c_init (I2C* i2c)
{
    g_i2c = i2c;
}

extern "C" uint8_t i2c_write1 (uint8_t slaveAddr, uint8_t regAddr, uint8_t data)
{
    return !g_i2c->MasterWrite(slaveAddr, regAddr, data);
}


extern "C" uint8_t i2c_write (uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t* data)
{
    return !g_i2c->MasterWrite(slaveAddr, regAddr, length, data);
}


extern "C" uint8_t i2c_read (uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t* data)
{
    return !g_i2c->MasterRead(slaveAddr, regAddr, length, data);
}


extern "C" void delay_ms(uint32_t ms)
{
    delay(ms);
}


