#include "i2c.h"
#include "mcg.h"
#include "gpio_helper.h"
#include "sim.h"
#include "system_time.h"
#include "pit.h"
#include "lcd/Adafruit_SSD1306.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

Adafruit_SSD1306* SSD1306_GetInstance();


static void test1()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();

	SPI* spi0 = SPI_GetInstance(SPI0);
	SPI_Initialize(spi0, TRUE, 2, SPI_DIV32, TRUE, TRUE, TRUE);

	Adafruit_SSD1306* lcd = SSD1306_GetInstance();
	lcd->setTextColor(1, 0);
	lcd->clearDisplay();
	lcd->write(0,5, "Hello, World5! 55555");
	lcd->display();

	while (1)
	{
		uint8_t res = SPI_TransmitByte(spi0, 0);
	}

	while(1);
}


void test_thermocouple()
{
    test1();
    while(1);
}
