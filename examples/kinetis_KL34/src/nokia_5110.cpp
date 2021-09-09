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
		//display.printf(0,4, "%f", 2.1234);
		display.printf(0,4, "01234567890123");
		display.printf(0,5, "01234567890123");
		display.printf(0,6, "01234567890123");
		*/
		display.fillScreen(1);
		display.display();
	}

	volatile uint32_t t = micros();

	// 74fps with the screened fully filled with characters
	// 829fps blank screen, display() only
	// 113fps fillScreen+display
	volatile uint32_t ps = 1000 * 1000000 / t;
	ps++;
    while(1);
}

void test_nokia5110()
{
	test2();
	while(1);
}
