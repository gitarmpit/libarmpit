#include "uart.h"
#include "gpio.h"
#include "mcg.h"
#include "sim.h"
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wunused-function"

// UART0
static void test_uart1()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();
	SIM_Select_UART0SRC_MCGFLLCLK();
	SIM_Select_FLL();


    GPIO_PORT* portD = GPIO_GetInstance(PORTD);
    GPIO_EnableClock(portD, TRUE);
    GPIO_PIN d6 = GPIO_GetPin(portD, GPIO_PIN6);
    GPIO_SetAF(&d6, 3);
    GPIO_PIN d7 = GPIO_GetPin(portD, GPIO_PIN7);
    GPIO_SetAF(&d7, 3);

	UART* uart0 = UART_GetInstance(UART0);
	UART_Initialize(uart0, (uint32_t)UART_BAUD_9600, TRUE, TRUE);
	char c = 0;
	while (1)
	{
		//int8_t b = UART_ReceiveByte(uart0);
		//printf ("%d\n", b);
		UART_SendByte(uart0, c++);
		delay_ms(1000);
	}
	//uint8_t v = UART_ReceiveByte(uart0);
}

// UART1
static void test_uart2()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();

    GPIO_PORT* portE = GPIO_GetInstance(PORTE);
    GPIO_EnableClock(portE, TRUE);
    GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
    GPIO_SetAF(&e0, 3);
    GPIO_PIN e1 = GPIO_GetPin(portE, GPIO_PIN1);
    GPIO_SetAF(&e1, 3);

	UART* uart1 = UART_GetInstance(UART1);
	UART_Initialize(uart1, (uint32_t)UART_BAUD_9600, TRUE, TRUE);
	int8_t i = 0;
	while (1)
	{
		//UART_ReceiveByte(uart0);
		UART_SendByte(uart1, i++);
		delay_ms(1000);
	}
	//uint8_t v = UART_ReceiveByte(uart0);
}

// UART1
static void test_uart3()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();

    GPIO_PORT* portE = GPIO_GetInstance(PORTE);
    GPIO_EnableClock(portE, TRUE);
    GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
    GPIO_SetAF(&e0, 3);
    GPIO_PIN e1 = GPIO_GetPin(portE, GPIO_PIN1);
    GPIO_SetAF(&e1, 3);

	UART* uart1 = UART_GetInstance(UART1);
	UART_Initialize(uart1, (uint32_t)UART_BAUD_9600, TRUE, TRUE);
	// int8_t i = 0;
	while (1)
	{
		int8_t b = UART_ReceiveByte(uart1);
		printf ("%d\n", b);
		//UART_SendByte(uart1, i++);
		//delay_ms(100);
	}
	//uint8_t v = UART_ReceiveByte(uart0);
}

// UART2
static void test_uart4()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();

    GPIO_PORT* portE = GPIO_GetInstance(PORTE);
    GPIO_EnableClock(portE, TRUE);
    GPIO_PIN e22 = GPIO_GetPin(portE, GPIO_PIN22);
    GPIO_SetAF(&e22, 4);
    GPIO_PIN e23 = GPIO_GetPin(portE, GPIO_PIN23);
    GPIO_SetAF(&e23, 4);

	UART* uart2 = UART_GetInstance(UART2);
	UART_Initialize(uart2, (uint32_t)UART_BAUD_9600, TRUE, TRUE);
	// int8_t i = 0;
	while (1)
	{
		int8_t b = UART_ReceiveByte(uart2);
		printf ("%d\n", b);
		//UART_SendByte(uart1, i++);
		//delay_ms(100);
	}
	//uint8_t v = UART_ReceiveByte(uart0);
}

// UART0
// TODO : recv doesn't work may need a pullup?
static void test_uart5()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();
	SIM_Select_UART0SRC_MCGFLLCLK();
	SIM_Select_FLL();

    GPIO_PORT* portE = GPIO_GetInstance(PORTE);
    GPIO_EnableClock(portE, TRUE);
    GPIO_PIN e20 = GPIO_GetPin(portE, GPIO_PIN20);
    // GPIO_PullUp(&e20);
    GPIO_SetAF(&e20, 4);
    GPIO_PIN e21 = GPIO_GetPin(portE, GPIO_PIN21);
    // GPIO_PullUp(&e21);
    GPIO_SetAF(&e21, 4);

	UART* uart0 = UART_GetInstance(UART0);
	UART_Initialize(uart0, (uint32_t)UART_BAUD_9600, TRUE, TRUE);
	//int8_t i = 0;
	while (1)
	{
		int8_t b = UART_ReceiveByte(uart0);
		printf ("%d\n", b);
		//UART_SendByte(uart0, i++);
		delay_ms(1000);
	}
	//uint8_t v = UART_ReceiveByte(uart0);
}


// UART0
static void test_uart6()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();
	SIM_Select_UART0SRC_MCGFLLCLK();
	SIM_Select_FLL();


    GPIO_PORT* portB = GPIO_GetInstance(PORTB);
    GPIO_EnableClock(portB, TRUE);
    GPIO_PIN b16 = GPIO_GetPin(portB, GPIO_PIN16);
    GPIO_SetAF(&b16, 3);
    GPIO_PIN b17 = GPIO_GetPin(portB, GPIO_PIN17);
    GPIO_SetAF(&b17, 3);

	UART* uart0 = UART_GetInstance(UART0);
	UART_Initialize(uart0, (uint32_t)UART_BAUD_9600, TRUE, TRUE);
	//char c = 0;
	while (1)
	{
		int8_t b = UART_ReceiveByte(uart0);
		printf ("%d\n", b);
		//UART_SendByte(uart0, c++);
		//delay_ms(1000);
	}
	//uint8_t v = UART_ReceiveByte(uart0);
}

void test_uart()
{
	test_uart1();
}
