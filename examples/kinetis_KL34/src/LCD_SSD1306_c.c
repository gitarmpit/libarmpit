#include "i2c.h"
#include "gpio_helper.h"
#include "mcg.h"
#include "lcd/SSD1306.h"

void testSSD1306_c()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	//InitClock_FBI_Slow();
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 1200000);
	//I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 5000);

	SSD1306_Init(i2c);

	//GFX_setRotation(2);
	GFX_setTextColor(1);
	GFX_setBgColor(0);
	clearDisplay();
	display();
	//GFX_write("Hello, World!");
	int i = 0;
	while(1)
	{
		if (i++ % 2)
		{
			clearDisplay();
		}
		else
		{
			GFX_printf_xy(0,0, "01234567890123456789");
			GFX_printf_xy(0,1, "Line2");
			GFX_printf_xy(0,2, "Line3");
			GFX_printf_xy(0,3, "Line4");
			GFX_printf_xy(0,4, "Line5");
			GFX_printf_xy(0,5, "Line6");
			GFX_printf_xy(0,6, "Line7");
			GFX_printf_xy(0,7, "01234567890123456789");
		}
		display();
		delay_ms(1000);
	}

	while(1)
		;
}

