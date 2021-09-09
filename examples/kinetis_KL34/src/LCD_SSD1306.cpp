#include "i2c.h"
#include "gpio_helper.h"
#include "lcd/Adafruit_SSD1306.h"
#include "mcg.h"
#include "system_time.h"

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
	//InitClock_FBI_Fast();
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 1200000);
	//I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 5000);

	Adafruit_SSD1306 display (i2c);
	//display.setRotation(2);
	display.setTextColor(1, 0);
	display.clearDisplay();
	display.display();

	systick_enable(TRUE);
	for (int i = 0; i < 1000; ++i)
	{
		/*
		display.printf(0,0, "01234567890123456789");
		display.printf(0,1, "Line2");
		display.printf(0,2, "Line3");
		display.printf(0,3, "Line4");
		display.printf(0,4, "%f", 2.1234);
		display.printf(0,5, "Line6");
		display.printf(0,6, "Line7");
		display.printf(0,7, "01234567890123456789");
		*/
		//display.clearDisplay();
		//display.fillScreen(1);
		display.write2( 0, 0, "test0");
		//display.write2( 0, 1, "test00000000001");
		//display.write2( 0, 2, "test00000000002");
		display.write2( 0, 3, "test3");
		display.write2( 0, 7, "test7");
		//display.printf(0,7, "test");
		//display.display3(0);
	}

	volatile uint32_t t = micros();

	volatile uint32_t ps = 1000 * 1000000 / t;
	ps++;

	while(1)
		;
}

static void testLCD3()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	//InitClock_FBI_Slow();
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 1200000);
	//I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 5000);

	const char* buf[] = {
			"00000000000000000000000",
			"11111111111111111111111",
			"22222222222222222222222",
			"33333333333333333333333",
			"44444444444444444444444",
			"55555555555555555555555",
			"6666666666666666666666",
			"7777777777777777777777"
	};

	Adafruit_SSD1306 display (i2c);
	//display.setRotation(2);
	display.setTextColor(1, 0);
	display.clearDisplay();
	display.display();
	//display.display2();
	//display.printf(0,0,"Hello, World6!");
	uint8_t cnt = 0;
	while(1)
	{
		display.clearDisplay();
		display.display();
		display.printf(0,cnt % 8, buf[cnt % 8]);
		display.display3(cnt % 8);
		++cnt;
		delay_ms(1000);
	}
	while(1)
		;
}

static void testLCD4()
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
	while(1)
	{
		display.write2(0,0, "000000000000000000");
		display.write2(0,1, "111111111111111111");
		display.write2(0,2, "22222222222222222");
		delay_ms(1000);
	}
	while(1)
		;
}


void testSSD1306()
{
	testLCD2();
}
