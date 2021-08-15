#include "gpio_helper.h"
#include <stdlib.h>
#include <string.h>

static SPI* SetupSPIx (BOOL isMaster, uint8_t presc, SPI_BRDIV div, BOOL isPhaseStart, BOOL isPolarityLow, BOOL isLSB,
		GPIO_PORT* port,  GPIO_PIN_N clkPin, GPIO_PIN_N mosiPin, GPIO_PIN_N misoPin, SPI_N spiN, int af)
{
	GPIO_EnableClock(port, TRUE);
    GPIO_PIN CLK_pin = GPIO_GetPin (port, clkPin);
    GPIO_PIN MOSI_pin = GPIO_GetPin (port, mosiPin);
    GPIO_PIN MISO_pin = GPIO_GetPin (port, misoPin);

    GPIO_SetAF(&CLK_pin, af);
    GPIO_SetAF(&MISO_pin, af);
    GPIO_SetAF(&MOSI_pin, af);

    SPI* spi = SPI_GetInstance(spiN);
    SPI_Initialize(spi, isMaster, presc, div, isPhaseStart, isPolarityLow, isLSB);

	return spi;
}

SPI* GPIO_Helper_SetupSPI(SPI_Pins spi_no, BOOL isMaster, uint8_t presc, SPI_BRDIV div, BOOL isPhaseStart, BOOL isPolarityLow, BOOL isLSB)
{
	switch (spi_no)
	{
	case SPI0_PD_1_2_3:
		return SetupSPIx(isMaster, presc, div, isPhaseStart, isPolarityLow, isLSB, GPIO_GetInstance(PORTD),
				GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, SPI0, 2);

	case SPI0_PC_5_6_7:
		return SetupSPIx(isMaster, presc, div, isPhaseStart, isPolarityLow, isLSB, GPIO_GetInstance(PORTC),
				GPIO_PIN5, GPIO_PIN6, GPIO_PIN7, SPI0, 2);

	case SPI1_PD_5_6_7:
		return SetupSPIx(isMaster, presc, div, isPhaseStart, isPolarityLow, isLSB, GPIO_GetInstance(PORTD),
				GPIO_PIN5, GPIO_PIN6, GPIO_PIN7, SPI1, 2);

	case SPI0_PE_17_18_19:
		return SetupSPIx(isMaster, presc, div, isPhaseStart, isPolarityLow, isLSB, GPIO_GetInstance(PORTE),
				GPIO_PIN17, GPIO_PIN18, GPIO_PIN19, SPI0, 2);
	};

    return NULL;

}

GPIO_PIN GPIO_Helper_GetPin (const char* pin) //A1,B1,C15 etc
{
    if (!pin || strlen(pin) < 2)
    {
    	while(1)
    		;
    }

    GPIO_PORT* port;
    switch (pin[0])
    {
    case 'a':
    case 'A':
        port = GPIO_GetInstance(PORTA);
        break;
    case 'b':
    case 'B':
        port = GPIO_GetInstance(PORTB);
        break;
    case 'c':
    case 'C':
        port = GPIO_GetInstance(PORTC);
        break;
    case 'd':
    case 'D':
        port = GPIO_GetInstance(PORTD);
        break;
    case 'e':
    case 'E':
        port = GPIO_GetInstance(PORTE);
        break;
    default:
    	while(1)
    		;

    };

    GPIO_EnableClock(port, TRUE);

    int pin_no = atoi(&pin[1]);
    return GPIO_GetPin(port, (GPIO_PIN_N)pin_no);
}

