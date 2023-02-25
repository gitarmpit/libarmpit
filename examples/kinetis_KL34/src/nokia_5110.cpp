#include "lcd/Adafruit_Nokia5110.h"
#include "gpio_helper.h"
#include "system_time.h"
#include "mcg.h"

static void test1()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();

	// SPI0: bus clock = 24, max seems to be div4 = 6Mhz
	SPI* spi = GPIO_Helper_SetupSPI(SPI0_PE_17_18_19, TRUE, 1, SPI_DIV4, FALSE, FALSE, FALSE);

    // Override SPI MISO
    GPIO_PIN dcPin = GPIO_Helper_GetPin("E19");
    GPIO_SetupOut(&dcPin);

	// CE
    GPIO_PIN ssPin = GPIO_Helper_GetPin("E20");
    GPIO_SetupOut(&ssPin);

    GPIO_PIN rstPin = GPIO_Helper_GetPin("E21");
    GPIO_SetupOut(&rstPin);

    Adafruit_Nokia5110 display(spi, &dcPin, &rstPin, &ssPin);
    display.Init(0xbc);

    display.setRotation(2);

    //display.write(L"123");
    //display.display();
    //double f = -4.12345;
    //display.printf("%8.5f", f);
    display.printf ("%d", 123);
    display.display();
    while(1);
}

static void test2()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();

	// SPI0: bus clock = 24, max seems to be div4 = 6Mhz
	SPI* spi = GPIO_Helper_SetupSPI(SPI0_PE_17_18_19, TRUE, 1, SPI_DIV4, FALSE, FALSE, FALSE);

    // Override SPI MISO
    GPIO_PIN dcPin = GPIO_Helper_GetPin("E19");
    GPIO_SetupOut(&dcPin);

	// CE
    GPIO_PIN ssPin = GPIO_Helper_GetPin("E20");
    GPIO_SetupOut(&ssPin);

    GPIO_PIN rstPin = GPIO_Helper_GetPin("E21");
    GPIO_SetupOut(&rstPin);

    Adafruit_Nokia5110 display(spi, &dcPin, &rstPin, &ssPin);
    display.Init(0xbc);

    display.setRotation(2);

	systick_enable(TRUE);
	//13 columns x 7 lines
	for (int i = 0; i < 1000; ++i)
	{
		/*
		display.printf(0,0, "01234567890123");
		display.printf(0,1, "01234567890123");
		display.printf(0,2, "01234567890123");
		display.printf(0,3, "01234567890123");
		display.printf(0,4, "%f", 2.1234);
		display.printf(0,4, "01234567890123");
		display.printf(0,5, "01234567890123");
		display.printf(0,6, "01234567890123");
		*/
		//display.clearDisplay();
		display.printf(0,6, "01234567890123");

		//display.fillScreen(1);
		display.display();
	}

	volatile uint32_t t = micros();

	// 74fps with the screen fully filled with characters
	// 829fps blank screen, display() only
	// 113fps fillScreen+display
	volatile uint32_t ps = 1000 * 1000000 / t;
	ps++;
    while(1);
}

#include <stdio.h>
#include "lcd/ee_printf.h"
static int __printf (const char *fmt, ...)
{
	static char buf[256];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    return 0;
}

static int __printf_float(double f, uint8_t prec)
{
	UNUSED(prec);
	static volatile char buf[256];
	int i = (int32_t)f;
	int frac = (f-i)*1000;
	sprintf ((char*)buf, "%d.%d", i, frac);
	return 0;
}

static void test_printf()
{
	systick_enable(TRUE);
	for (int i = 0; i < 1000; ++i)
	{
		//__printf ("%8.3f %10.4f", 1.2192, -22244.3994);
		__printf_float (1.2192, 3);
		__printf_float (-22244.3994, 3);
		// __printf ("%d %d", 1, 2);
	}
	volatile uint32_t t = micros();

	volatile uint32_t ps = 1000 * 1000000 / t;
	ps++;
	//printf 2 floats: 863 ps,   two ints: 10,000
	//ee_printf 2 floats: 948 ps, two ints: 24,000
	//size increase: ee_printf: +20k,  -u _printf_float +20k
}

void test_nokia5110()
{
	test_printf();
	while(1);
}
