#include "lcd/ILI9225/ILI9225.h"
#include "gpio_helper.h"
#include "mcg.h"

extern "C" void initialise_monitor_handles();
#include <stdio.h>

void LCD_9225_test()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	/*
	 * LCD pin order: CS   RS   WR   RD    RST
	 *                E0   E1   E16  E17   E18
	 *  DATA: D0 - D7
	 */


	initialise_monitor_handles();

	GPIO_PIN cs = GPIO_Helper_GetPin("E0");
	GPIO_PIN rs =GPIO_Helper_GetPin("E1");
	GPIO_PIN wr =GPIO_Helper_GetPin("E16");
	GPIO_PIN rd =GPIO_Helper_GetPin("E17");
	GPIO_PIN rst =GPIO_Helper_GetPin("E18");

	GPIO_PORT* portD = GPIO_GetInstance(PORTD);
    GPIO_EnableClock(portD, TRUE);
	ILI9225 lcd (&cs, &rs, &wr, &rd, &rst, portD);
	for (int i = 0; i < 10; ++i)
	{
		printf ("%x\n", lcd.readID());
	}
	lcd.init();
	int color = 0;
	// lcd.setRotation(0);
	//lcd.setAddrWindow(0, 0, 100, 100);

	while (true)
	{
		lcd.flood(color % 2 ? 0 : 0xffff, TFTWIDTH*TFTHEIGHT);
		++color;
		delay_ms(500);
	}

	while(1)
	{
		//printf ("%x\n", lcd.readID());
		//delay_ms(1000);
		delay_ms(1000);
		for (int j = 0; j < 50; ++j)
		{
			for (int i = 0; i < 100; ++i)
			{
				lcd.drawPixel(i, j, color);
				//__nop();
				delay_ms(10);
			}
		}
		color += 18;
	}

	while(1);

}
