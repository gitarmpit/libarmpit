#include "MCP47X6.h"

#include "i2c.h"
#include "gpio_helper.h"
#include "lcd/Adafruit_SSD1306.h"
#include "mcg.h"
#include "system_time.h"


void test_mcp47x6()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	//InitClock_FBI_Slow();
	//I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PB_0_1, 1200000);
	//I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 5000);


	GPIO_PIN led = GPIO_Helper_GetPin("E17");
	GPIO_SetupOut(&led);

	GPIO_TogglePin(&led);

	delay_ms(1000);

	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C1_PE_1_0, 1200000);
	delay_ms(1000);

	MCP47X6 dac (i2c);

	dac.setVrefVcc();
	dac.setVoltage(3.25);
	dac.setVrefPin();
	dac.setVoltage(3);
	dac.setVrefVcc();
	dac.setVoltage(2.5);

	dac.powerDown();

	dac.setVoltage(0);
	dac.setVoltage(0.1);
	dac.setVoltage(0.2);
	dac.setVoltage(1.0);
	dac.setVoltage(2.0);
	dac.setVoltage(3.0);
	dac.setVoltage(3.25);

	uint16_t level = 0;

	while(1)
	{

	}

}
