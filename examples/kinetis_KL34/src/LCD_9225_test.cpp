#include "lcd/ILI9225/ILI9225.h"
#include "gpio_helper.h"
#include "mcg.h"
//#include "consola24.h"
//#include "fonts/cour20.h"
//#include "fonts/coure12.h"
//#include "fonts/fixedsys20.h"
//#include "fonts/FreeSerif24pt7b.h"
//#include "fonts/12/arial12.h"
//#include "fonts/12/calibri12.h"
//#include "fonts/12/consola12.h"
//#include "fonts/12/Urbanist_Regular12.h"
//#include "fonts/12/tahoma12.h"
//#include "fonts/12/verdana12.h"
//#include "fonts/12/SourceCodePro_Regular12.h"
//#include "fonts/12/corbel12.h"
//#include "fonts/12/UbuntuCondensed_Regular18.h"
#include "fonts/36/consola36.h"

// Good
//#include "fonts/18/cour18.h"
//#include "fonts/8/consola8.h"

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

void LCD_9225_test()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	/*
	 * LCD pin order: CS   RS   WR   RD    RST
	 *                E0   E1   E16  E17   E18
	 *  DATA: D0 - D7
	 */


	//initialise_monitor_handles();

	GPIO_PIN cs = GPIO_Helper_GetPin("E0");
	GPIO_PIN rs =GPIO_Helper_GetPin("E1");
	GPIO_PIN wr =GPIO_Helper_GetPin("E16");
	GPIO_PIN rd =GPIO_Helper_GetPin("E17");
	GPIO_PIN rst =GPIO_Helper_GetPin("E18");

	GPIO_PORT* portD = GPIO_GetInstance(PORTD);
    GPIO_EnableClock(portD, TRUE);
	ILI9225 lcd (&cs, &rs, &wr, &rd, &rst, portD);
	//uint16_t id = lcd.readRegister16(0);
	//id = lcd.readID();
	lcd.init();
	//delay_ms(1000);
	lcd.setRotation(1);
	lcd.fillScreen(0);

	lcd.setFont(&consola);
	lcd.printf (0, 1, "abcdefghij");
	lcd.printf (0, 2, "ABCDEFGH");
	lcd.printf (0, 3, "1234567890");
	//lcd.printf(0, 0, "12:34");
	while(1);

}
