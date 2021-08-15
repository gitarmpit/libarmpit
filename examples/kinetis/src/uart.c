#include "uart.h"
#include "sim.h"
#
static UART UART_list[] =
{
    { TRUE, &SIM_SCGC4, SIM_SCGC4_UART0, &UART0_BDH, &UART0_BDL, &UART0_C1, &UART0_C2,
      &UART0_C3, &UART0_C4, &UART0_C5, &UART0_D, &UART0_S1, &UART0_S2, UART0_IRQn, NULL, NULL, NULL
    },
    { FALSE, &SIM_SCGC4, SIM_SCGC4_UART1, &UART1_BDH, &UART1_BDL, &UART1_C1, &UART1_C2,
      &UART1_C3, &UART1_C4, NULL, &UART1_D, &UART1_S1, &UART1_S2, UART1_IRQn, NULL, NULL, NULL
    },
    { FALSE, &SIM_SCGC4, SIM_SCGC4_UART2, &UART2_BDH, &UART2_BDL, &UART2_C1, &UART2_C2,
      &UART2_C3, &UART2_C4, NULL, &UART2_D, &UART2_S1, &UART2_S2, UART2_IRQn, NULL, NULL, NULL
    },
};

UART* UART_GetInstance(UART_N n)
{
    return &UART_list[(uint8_t)n];
}


void UART0_IRQHandler()
{

}

void UART1_IRQHandler()
{

}


void UART2_IRQHandler()
{

}


void UART_Initialize(UART* uart, uint32_t baudRate, BOOL enableRx, BOOL enableTx)
{
    SIM_Enable(uart->SIM_enableReg, uart->SIM_enableMask, TRUE);

	*uart->UART_C2 &= ~UART_C2_RE;
	*uart->UART_C2 &= ~UART_C2_TE;

	uint16_t sbr = BUS_FREQ / baudRate / 16;
	if (sbr == 0)
	{
		sbr = 1;
	}

	if (sbr > 8191)
	{
		sbr = 8191;
	}

	*uart->UART_BDH |= (sbr >> 8);
	*uart->UART_BDL |= (sbr & 0xff);

	if (enableRx)
	{
		*uart->UART_C2 |= UART_C2_RE;
	}
	if (enableTx)
	{
		*uart->UART_C2 |= UART_C2_TE;
	}

}

void UART_SendByte(UART* uart, uint8_t val)
{
	while(!(*uart->UART_S1 & UART_S1_TDRE))
		;

	*uart->UART_D = val;

	//while(!(*uart->UART_S1 & UART_S1_TC))
	//	;

}

uint8_t UART_ReceiveByte(UART* uart)
{
	while(!(*uart->UART_S1 & UART_S1_RDRF))
		;

	return *uart->UART_D;
}

