#include "lcd/HX8357/HX8357.h"
#include "gpio_helper.h"
#include "mcg.h"
#include "sim.h"
#include "fonts/small13.h"
#include "fonts/consola42.h"

// Good
//#include "fonts/cour18.h"
//#include "fonts/consola8.h"
//#include "fonts/consola36.h"
//#include "fonts/Lucida_Console18.h"
//#include "fonts/Lucida_Sans16.h"  //more compact, proportional
//#include "fonts/LucidaBright18.h" //proportional
//#include "fonts/SourceHanMonoK_Regular18.h"
//#include "fonts/dos8.h"
//#include "fonts/fixedsys20.h"
//#include "fonts/term6.h"




//extern "C" void initialise_monitor_handles();
//#include <stdio.h>

/*
 	lcd.setFont(&consola);
	lcd.setFont(&FreeSerif24pt7b);
	lcd.setFont(&arial);
	lcd.printf (0, 0, "Arial TEST Test: %d", 123);

	lcd.setFont(&calibri);
	lcd.printf (0, 1, "Calibri TEST Test: %d", 123);

	lcd.setFont(&consola);
	lcd.printf (0, 2, "Consola TEST Test: %d", 123);

	lcd.setFont(&Urbanist_Regular);
	lcd.printf (0, 3, "Urbanist regular TEST Test: %d", 123);
	lcd.setFont(&tahoma);
	lcd.printf (0, 4, "Tahoma  TEST Test: %d abcd", 1234567);
	lcd.setFont(&verdana);
	lcd.printf (0, 5, "Verdana TEST Test: %d abcd", 1234567);

	lcd.setFont(&SourceCodePro_Regular);
	lcd.printf (0, 6, "SCPRO Regular TEST Test: %d abcd", 12345);
	lcd.setFont(&corbel);
	lcd.printf (0, 4, "Corbel TEST Test: %d abcd", 12345);

	lcd.setFont(&UbuntuCondensed_Regular);
	lcd.printf (0, 5, "UbuntuCondensed TEST Test: %d abcd", 12345);

 */


void LCD_HX8357_test()
{
	InitClock_FEI_72Mhz_Bus_36Mhz();
	//InitClock_FEI_48Mhz_Bus_24Mhz();

	/*
	 * LCD pin order: CS   RS   WR   RD    RST
	 *                E0   E1   E16  E17   E18
	 *  DATA: D0 - D7
	 */


	//initialise_monitor_handles();

	GPIO_PIN cs  = GPIO_Helper_GetPin("E0");
	GPIO_PIN rs  = GPIO_Helper_GetPin("E1");
	GPIO_PIN wr  = GPIO_Helper_GetPin("E16");
	GPIO_PIN rd  = GPIO_Helper_GetPin("E17");
	GPIO_PIN rst = GPIO_Helper_GetPin("E18");

	GPIO_PORT* portD = GPIO_GetInstance(PORTD);
    GPIO_EnableClock(portD, TRUE);
	HX8357 lcd (&cs, &rs, &wr, &rd, &rst, portD);
	lcd.readID();
	lcd.init();
	lcd.readID();
	//lcd.setAddrWindow(0,100,0,100);

	/*
	for (int y = 0; y < 100; ++y)
	{
		for (int x = 0; x < 100; ++x)
		{
			lcd.drawPixel(x, y, 0xffff);
		}
	}
	*/

	//lcd.setAddrWindow(0,lcd.width(),0,lcd.height());
	while(1)
	{
		lcd.fillScreen(0);
		delay_ms(1000);
		lcd.fillScreen(0xf8f8);
		delay_ms(1000);
		lcd.fillScreen(0xffff);
		delay_ms(1000);
		lcd.fillScreen(0x1f1f);
		delay_ms(1000);
		lcd.fillScreen(0x0707);
		delay_ms(1000);

	}
	//delay_ms(1000);
	lcd.setRotation(1);
	lcd.fillScreen(0);

	/*
	lcd.setFont(&small13);
	lcd.printf (0, 1, "abcdefghij");
	lcd.printf (0, 2, "ABCDEFGH");
	lcd.printf (0, 3, "1234567890");
*/

	lcd.setFont(&consola);
	lcd.printf (0, 1, "abcdefghij");
	lcd.printf (0, 2, "ABCDEFGH");
	lcd.printf (0, 3, "1234567890");
	lcd.printf (0, 4, "abcdefghij");
	lcd.printf (0, 5, "ABCDEFGH");
	lcd.printf (0, 6, "1234567890");
	while(1);

}
