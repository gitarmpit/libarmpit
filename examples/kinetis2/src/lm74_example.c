#include "spi.h"
#include "gpio.h"
#include "TempLM74.h"
#include "gpio_helper.h"
#include <stdio.h>

void test_lm74()
{
	// InitClock_FEI_96Mhz_Bus_32Mhz();

	GPIO_PORT* portD = GPIO_GetInstance(PORTD);
	GPIO_EnableClock(portD, TRUE);

	GPIO_PIN ss = GPIO_GetPin(portD, GPIO_PIN0);
	GPIO_SetupOut(&ss);

	GPIO_PIN ck = GPIO_GetPin(portD, GPIO_PIN1);
	GPIO_SetAF(&ck, 2);

	GPIO_PIN mo = GPIO_GetPin(portD, GPIO_PIN2);
	GPIO_SetAF(&mo, 2);

	GPIO_PIN mi = GPIO_GetPin(portD, GPIO_PIN3);
	GPIO_SetAF(&mi, 2);


	SPI* spi = SPI_GetInstance(SPI0);
	//SPI_Initialize(spi, TRUE, 2, SPI_DIV32, TRUE, TRUE, FALSE);
	SPI_Initialize(spi, TRUE, 2, SPI_DIV2, TRUE, TRUE, FALSE);

	LM74_Init(spi, &ss);
	// uint16_t pc = LM74_GetProductCode();
	float temp = 0;
	while(1)
	{
		temp = LM74_GetTemp();
		int frac = (temp - (int)temp) * 1000;
		printf ("temp: %d.%03d\n", (int)temp, frac);
		delay_ms(1000);

	}
}

static void test_pin(GPIO_PIN* pin)
{
	GPIO_SetupOut(pin);
	GPIO_SetPin(pin);
	// GPIO_ResetPin(&pin);
}

void test_pins()
{
	GPIO_PIN p = GPIO_Helper_GetPin("E0");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E1");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E16");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E17");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E18");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E19");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E20");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E21");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E22");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E23");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E29");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E30");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E31");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E24");
	test_pin(&p);
	p = GPIO_Helper_GetPin("E25");
	test_pin(&p);
	p = GPIO_Helper_GetPin("A1");
	test_pin(&p);
	p = GPIO_Helper_GetPin("A2");
	test_pin(&p);
	p = GPIO_Helper_GetPin("A5");
	test_pin(&p);
	p = GPIO_Helper_GetPin("A12");
	test_pin(&p);
	p = GPIO_Helper_GetPin("A13");
	test_pin(&p);
	p = GPIO_Helper_GetPin("A18");
	test_pin(&p);
	p = GPIO_Helper_GetPin("A19");
	test_pin(&p);
	p = GPIO_Helper_GetPin("B0");
	test_pin(&p);
	p = GPIO_Helper_GetPin("B1");
	test_pin(&p);
	p = GPIO_Helper_GetPin("B2");
	test_pin(&p);
	p = GPIO_Helper_GetPin("B3");
	test_pin(&p);
	p = GPIO_Helper_GetPin("B16");
	test_pin(&p);
	p = GPIO_Helper_GetPin("B17");
	test_pin(&p);
	p = GPIO_Helper_GetPin("B18");
	test_pin(&p);
	p = GPIO_Helper_GetPin("B19");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C0");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C1");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C2");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C3");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C20");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C21");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C22");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C23");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C4");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C5");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C6");
	test_pin(&p);
	p = GPIO_Helper_GetPin("C7");
	test_pin(&p);
	p = GPIO_Helper_GetPin("D0");
	test_pin(&p);
	p = GPIO_Helper_GetPin("D1");
	test_pin(&p);
	p = GPIO_Helper_GetPin("D2");
	test_pin(&p);
	p = GPIO_Helper_GetPin("D3");
	test_pin(&p);
	p = GPIO_Helper_GetPin("D4");
	test_pin(&p);
	p = GPIO_Helper_GetPin("D5");
	test_pin(&p);
	p = GPIO_Helper_GetPin("D6");
	test_pin(&p);
	p = GPIO_Helper_GetPin("D7");
	test_pin(&p);

	while(1)
		;


}

void test_lm74_2()
{
	// InitClock_FEI_96Mhz_Bus_32Mhz();

	GPIO_PORT* portD = GPIO_GetInstance(PORTD);
	GPIO_EnableClock(portD, TRUE);

	GPIO_PIN ss = GPIO_GetPin(portD, GPIO_PIN0);
	GPIO_SetupOut(&ss);


	// SPI* spi = GPIO_Helper_SetupSPI(SPI0_PD_1_2_3, TRUE, 2, SPI_DIV2, TRUE, TRUE, FALSE);

	SPI* spi = GPIO_Helper_SetupSPI(SPI0_PE_17_18_19, TRUE, 2, SPI_DIV2, TRUE, TRUE, FALSE);

	//SPI* spi = GPIO_Helper_SetupSPI(SPI1_PD_5_6_7, TRUE, 2, SPI_DIV2, TRUE, TRUE, FALSE);

	// SPI* spi = GPIO_Helper_SetupSPI(SPI0_PC_5_6_7, TRUE, 2, SPI_DIV2, TRUE, TRUE, FALSE);


	LM74_Init(spi, &ss);
	float temp = 0;
	while(1)
	{
		temp = LM74_GetTemp();
		int frac = (temp - (int)temp) * 1000;
		printf ("temp: %d.%03d\n", (int)temp, frac);
		delay_ms(1000);

	}
}
