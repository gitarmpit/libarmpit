#include "lcd/Adafruit_Nokia5110.h"
#include "gpio_helper.h"

void test_nokia5110()
{

	SPI* spi = GPIO_Helper_SetupSPI(SPI0_PE_17_18_19, TRUE, 8, SPI_DIV128, FALSE, FALSE, FALSE);

    GPIO_PIN ssPin = GPIO_Helper_GetPin("D0");
    GPIO_SetupOut(&ssPin);

    GPIO_PIN dcPin = GPIO_Helper_GetPin("E19");
    GPIO_SetupOut(&dcPin);

    GPIO_PIN rstPin = GPIO_Helper_GetPin("E21");
    GPIO_SetupOut(&rstPin);

    Adafruit_Nokia5110 display(spi, &dcPin, &rstPin, &ssPin);
    display.Init(0xbc);

    display.setRotation(2);

    //display.write(L"123");
    display.display();
    double f = -4.12345;
    display.printf("%8.5f", f);
    //display.printf ("%d", 123);
    display.display();
    while(1);
}
