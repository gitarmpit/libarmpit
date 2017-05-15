/*
 7-17-2011
 Spark Fun Electronics 2011
 Nathan Seidle
 
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This code writes a series of images and text to the Nokia 5110 84x48 graphic LCD:
 http://www.sparkfun.com/products/10168
 
 Do not drive the backlight with 5V. It will smoke. However, the backlight on the LCD seems to be 
 happy with direct drive from the 3.3V regulator.

 Although the PCD8544 controller datasheet recommends 3.3V, the graphic Nokia 5110 LCD can run at 3.3V or 5V. 
 No resistors needed on the signal lines.
 
 You will need 5 signal lines to connect to the LCD, 3.3 or 5V for power, 3.3V for LED backlight, and 1 for ground.
 */

#include "spi_cpp.h"
#include "gpio_cpp.h"

#define PIN_SCE   7 //Pin 3 on LCD
#define PIN_RESET 6 //Pin 4 on LCD
#define PIN_DC    5 //Pin 5 on LCD
#define PIN_SDIN  4 //Pin 6 on LCD
#define PIN_SCLK  3 //Pin 7 on LCD

//The DC pin tells the LCD if we are sending a command or data
#define LCD_COMMAND 0 
#define LCD_DATA  1

//You may find a different size screen, but this one is 84 by 48 pixels
#define LCD_X     84
#define LCD_Y     48

class LCD5110
{
private:
	SPI* _spi;
	GPIO_PIN* _dcPin;
	GPIO_PIN* _rstPin;
	uint8_t _x;
	bool _reverse;

	void SendCommand(uint8_t data);
	void SendData(uint8_t data);
	void Character(char character);

public:

	LCD5110(SPI* spi, GPIO_PIN* dcPin, GPIO_PIN* rstPin, GPIO_PIN* ssPin, bool reverse);

	void GotoXY(int x, int y);
	void Bitmap(char my_array[]);
	void WriteLine(char *characters);
	void WriteLine(int x, int y, char *characters);
	void Clear(void);
	void Init(void);
};
