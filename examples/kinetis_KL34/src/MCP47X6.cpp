
#include "MCP47X6.h"

MCP47X6::MCP47X6(I2C* i2c) {
    devAddr = MCP47X6_DEFAULT_ADDRESS;
    _i2c = i2c;
}

void MCP47X6::setVoltage(double v)
{
  	double vmax = 3.25;
  	if (v > vmax)
  	{
  		v = vmax;
  	}
  	//double res = vmax / 4095;
  	//double val = v / res;
  	double val = v * 4095 / vmax ;
  	setDAC ((uint16_t) val);
}

void MCP47X6::setDAC(uint16_t level) {
	uint8_t buf[2];
	buf[0] = (level>>8) & 0xf;
	buf[1] = (level & 0xff);
	I2C_MasterWriteToAddr(_i2c, devAddr, (uint8_t*)&buf, 2);
}

void MCP47X6::setVrefPin()
{
	// 00: VCC : 01: unbuffered, 10: buffered
	uint8_t buf = 0b10001000;
	I2C_MasterWriteToAddr(_i2c, devAddr, &buf, 1);
}

void MCP47X6::setVrefVcc()
{
	uint8_t buf = 0b10000000;
	I2C_MasterWriteToAddr(_i2c, devAddr, &buf, 1);
}

void MCP47X6::powerDown() {
	uint16_t buf = 0x000010;
	I2C_MasterWriteToAddr(_i2c, devAddr, (uint8_t*)&buf, 2);
}
