#ifndef I2C_WRAPPER_H
#define I2C_WRAPPER_H
#include <stdint.h>
#ifdef __cplusplus
#include "i2c_cpp.h"
#include "rcc.h"
void i2c_init (I2C* i2c);

#endif

//C layer calls this

#ifdef __cplusplus
extern "C"
{
#endif

uint8_t i2c_write1 (uint8_t slaveAddr, uint8_t regAddr, uint8_t data);
uint8_t i2c_write (uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t* data);

uint8_t i2c_read (uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t* data);

void delay_ms(uint32_t ms);


#ifdef __cplusplus
}
#endif


#endif
