#include "i2c.h"
#include "gpio_helper.h"
#include "lcd/Adafruit_SSD1306.h"
#include "mcg.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static void testLCD1()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	//InitClock_FBI_Slow();
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 1200000);
	//I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 5000);

	Adafruit_SSD1306 display (i2c);
	display.setRotation(2);
	display.setTextColor(1, 0);
	display.clearDisplay();
	display.display();
	//display.write("Hello, World!");
	int i = 0;
	while(1)
	{
		if (i++ % 2)
		{
			display.clearDisplay();
		}
		else
		{
			display.printf(0,0, "01234567890123456789");
			display.printf(0,1, "Line2");
			display.printf(0,2, "Line3");
			display.printf(0,3, "Line4");
			display.printf(0,4, "Line5");
			display.printf(0,5, "Line6");
			display.printf(0,6, "Line7");
			display.printf(0,7, "01234567890123456789");
		}
		display.display();
		delay_ms(100);
	}

	while(1)
		;
}

static void testLCD2()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	//InitClock_FBI_Slow();
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 1200000);
	//I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 5000);

	Adafruit_SSD1306 display (i2c);
	//display.setRotation(2);
	display.setTextColor(1, 0);
	display.clearDisplay();
	display.display();
	//display.write("Hello, World!");
	int i = 0;
	while(1)
	{
		if (i++ % 2)
		{
			display.clearDisplay();
		}
		else
		{
			display.printf(0,0, "01234567890123456789");
			display.printf(0,1, "Line2");
			display.printf(0,2, "Line3");
			display.printf(0,3, "Line4");
			display.printf(0,4, "Line5");
			display.printf(0,5, "Line6");
			display.printf(0,6, "Line7");
			display.printf(0,7, "01234567890123456789");
		}
		display.display();
		delay_ms(1000);
	}

	while(1)
		;
}

void testSSD1306()
{
	testLCD1();
}
