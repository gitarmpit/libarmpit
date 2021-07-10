#include "sim.h"

void SIM_Select_RTCClock_OSC32KCLK()
{
    SIM_SOPT1 &= ~SIM_SOPT1_OSC32KSEL_CLEARMASK;
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL_OSC32KCLK;
}

void SIM_Select_RTCClock_RTC_CLKIN()
{
    SIM_SOPT1 &= ~SIM_SOPT1_OSC32KSEL_CLEARMASK;
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL_RTC_CLKIN;
}

void SIM_Select_RTCClock_LPO()
{
    SIM_SOPT1 &= ~SIM_SOPT1_OSC32KSEL_CLEARMASK;
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL_LPO;
}

void SIM_Select_UART0SRC_OFF()
{
    SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC_OFF;
}

void SIM_Select_UART0SRC_MCGFLLCLK()
{
    SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC_MCGFLLCLK;
}

void SIM_Select_UART0SRC_OSCERCLK()
{
    SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC_OSCERCLK;
}

void SIM_Select_UART0SRC_MCGIRCLK()
{
    SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC_MCGIRCLK;
}

void SIM_Select_TPMSRC_OFF()
{
    SIM_SOPT2 &= ~SIM_SOPT2_TPMSRC_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC_OFF;
}

void SIM_Select_TPMSRC_MCGFLLCLK()
{
    SIM_SOPT2 &= ~SIM_SOPT2_TPMSRC_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC_MCGFLLCLK;
}

void SIM_Select_TPMSRC_OSCERCLK()
{
    SIM_SOPT2 &= ~SIM_SOPT2_TPMSRC_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC_OSCERCLK;
}

void SIM_Select_TPMSRC_MCGIRCLK()
{
    SIM_SOPT2 &= ~SIM_SOPT2_TPMSRC_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC_MCGIRCLK;
}

// MCGPLLCLK
void SIM_Select_FLL()
{
    SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL;
}

// MCGFLLCLK
void SIM_Select_PLL()
{
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL;
}

void SIM_SelectCLKOUT_BUS()
{
	SIM_SOPT2 &= ~SIM_SOPT2_CLKOUTSEL_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL_BUS;
}

void SIM_SelectCLKOUT_LPO()
{
	SIM_SOPT2 &= ~SIM_SOPT2_CLKOUTSEL_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL_LPO;
}

void SIM_SelectCLKOUT_MCGIRCLK()
{
	SIM_SOPT2 &= ~SIM_SOPT2_CLKOUTSEL_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL_MCGIRCLK;
}

void SIM_SelectCLKOUT_OSCERCLK()
{
	SIM_SOPT2 &= ~SIM_SOPT2_CLKOUTSEL_CLEARMASK;
    SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL_OSCERCLK;
}

void SIM_Select_RTC_CLOCKOUT_RTC()
{
	SIM_SOPT2 &= ~SIM_SOPT2_RTCCLKOUTSEL;
}

void SIM_Select_RTC_CLOCKOUT_OSCERCLK()
{
	SIM_SOPT2 |= SIM_SOPT2_RTCCLKOUTSEL;
}

void SIM_Get_SDID(uint8_t* famID, uint8_t* subfamID, uint8_t* seriesID, uint8_t* sramSize, uint8_t* revID, uint8_t* dieID, uint8_t* pinID)
{
	*famID = EXTRACT_VALUE(SIM_SDID, 31, 28);
	*subfamID = EXTRACT_VALUE(SIM_SDID, 27, 24);
	*seriesID = EXTRACT_VALUE(SIM_SDID, 23, 20);
	*sramSize = EXTRACT_VALUE(SIM_SDID, 19, 16);
	*revID =    EXTRACT_VALUE(SIM_SDID, 15, 12);
	*dieID =    EXTRACT_VALUE(SIM_SDID, 11, 7);
	*pinID =    EXTRACT_VALUE(SIM_SDID, 3, 0);
}

void SIM_Enable(volatile uint32_t* port, uint32_t mask, BOOL enable)
{
	if (enable)
		*port |= mask;
	else
		*port &= ~mask;
}


void SIM_Enable_SPI1  (BOOL enable)
{
	if (enable)
		SIM_SCGC4 |= SIM_SCGC4_SPI1;
	else
		SIM_SCGC4 &= ~SIM_SCGC4_SPI1;

}
void SIM_Enable_SPI0  (BOOL enable)
{
	if (enable)
		SIM_SCGC4 |= SIM_SCGC4_SPI0;
	else
		SIM_SCGC4 &= ~SIM_SCGC4_SPI0;
}
void SIM_Enable_CMP   (BOOL enable)
{
	if (enable)
		SIM_SCGC4 |= SIM_SCGC4_CMP;
	else
		SIM_SCGC4 &= ~SIM_SCGC4_CMP;
}
void SIM_Enable_UART2 (BOOL enable)
{
	if (enable)
		SIM_SCGC4 |= SIM_SCGC4_UART2;
	else
		SIM_SCGC4 &= ~SIM_SCGC4_UART2;
}
void SIM_Enable_UART1 (BOOL enable)
{
	if (enable)
		SIM_SCGC4 |= SIM_SCGC4_UART1;
	else
		SIM_SCGC4 &= ~SIM_SCGC4_UART1;
}
void SIM_Enable_UART0 (BOOL enable)
{
	if (enable)
		SIM_SCGC4 |= SIM_SCGC4_UART0;
	else
		SIM_SCGC4 &= ~SIM_SCGC4_UART0;
}
void SIM_Enable_I2C1  (BOOL enable)
{
	if (enable)
		SIM_SCGC4 |= SIM_SCGC4_I2C1;
	else
		SIM_SCGC4 &= ~SIM_SCGC4_I2C1;
}
void SIM_Enable_I2C0  (BOOL enable)
{
	if (enable)
		SIM_SCGC4 |= SIM_SCGC4_I2C0;
	else
		SIM_SCGC4 &= ~SIM_SCGC4_I2C0;
}
void SIM_Enable_SLCD  (BOOL enable)
{
	if (enable)
		SIM_SCGC5 |= SIM_SCGC5_SLCD;
	else
		SIM_SCGC5 &= ~SIM_SCGC5_SLCD;
}
void SIM_Enable_PORTE (BOOL enable)
{
	if (enable)
		SIM_SCGC5 |= SIM_SCGC5_PORTE;
	else
		SIM_SCGC5 &= ~SIM_SCGC5_PORTE;
}
void SIM_Enable_PORTD (BOOL enable)
{
	if (enable)
		SIM_SCGC5 |= SIM_SCGC5_PORTD;
	else
		SIM_SCGC5 &= ~SIM_SCGC5_PORTD;
}
void SIM_Enable_PORTC (BOOL enable)
{
	if (enable)
		SIM_SCGC5 |= SIM_SCGC5_PORTC;
	else
		SIM_SCGC5 &= ~SIM_SCGC5_PORTC;
}
void SIM_Enable_PORTB (BOOL enable)
{
	if (enable)
		SIM_SCGC5 |= SIM_SCGC5_PORTB;
	else
		SIM_SCGC5 &= ~SIM_SCGC5_PORTB;
}
void SIM_Enable_PORTA (BOOL enable)
{
	if (enable)
		SIM_SCGC5 |= SIM_SCGC5_PORTA;
	else
		SIM_SCGC5 &= ~SIM_SCGC5_PORTA;
}
void SIM_Enable_TSI   (BOOL enable)
{
	if (enable)
		SIM_SCGC5 |= SIM_SCGC5_TSI;
	else
		SIM_SCGC5 &= ~SIM_SCGC5_TSI;
}
void SIM_Enable_LPTMR (BOOL enable)
{
	if (enable)
		SIM_SCGC5 |= SIM_SCGC5_LPTMR;
	else
		SIM_SCGC5 &= ~SIM_SCGC5_LPTMR;
}
void SIM_Enable_DAC0  (BOOL enable)
{
	if (enable)
		SIM_SCGC6 |= SIM_SCGC6_DAC0;
	else
		SIM_SCGC6 &= ~SIM_SCGC6_DAC0;
}
void SIM_Enable_RTC   (BOOL enable)
{
	if (enable)
		SIM_SCGC6 |= SIM_SCGC6_RTC;
	else
		SIM_SCGC6 &= ~SIM_SCGC6_RTC;
}
void SIM_Enable_ADC0  (BOOL enable)
{
	if (enable)
		SIM_SCGC6 |= SIM_SCGC6_ADC0;
	else
		SIM_SCGC6 &= ~SIM_SCGC6_ADC0;
}
void SIM_Enable_TPM2  (BOOL enable)
{
	if (enable)
		SIM_SCGC6 |= SIM_SCGC6_TPM2;
	else
		SIM_SCGC6 &= ~SIM_SCGC6_TPM2;
}
void SIM_Enable_TPM1  (BOOL enable)
{
	if (enable)
		SIM_SCGC6 |= SIM_SCGC6_TPM1;
	else
		SIM_SCGC6 &= ~SIM_SCGC6_TPM1;
}
void SIM_Enable_TPM0  (BOOL enable)
{
	if (enable)
		SIM_SCGC6 |= SIM_SCGC6_TPM0;
	else
		SIM_SCGC6 &= ~SIM_SCGC6_TPM0;
}
void SIM_Enable_PIT    (BOOL enable)
{
	if (enable)
		SIM_SCGC6 |= SIM_SCGC6_PIT;
	else
		SIM_SCGC6 &= ~SIM_SCGC6_PIT;
}
void SIM_Enable_DMAMUX (BOOL enable)
{
	if (enable)
		SIM_SCGC6 |= SIM_SCGC6_DMAMUX;
	else
		SIM_SCGC6 &= ~SIM_SCGC6_DMAMUX;
}
void SIM_Enable_FTF    (BOOL enable)
{
	if (enable)
		SIM_SCGC6 |= SIM_SCGC6_FTF;
	else
		SIM_SCGC6 &= ~SIM_SCGC6_FTF;
}
void SIM_Enable_DMA    (BOOL enable)
{
	if (enable)
		SIM_SCGC7 |= SIM_SCGC7_DMA;
	else
		SIM_SCGC7 &= ~SIM_SCGC7_DMA;
}

void SIM_SetOUTDIV1 (uint8_t div)
{
	if (div < 1 || div > 16)
	{
		while(1)
			;
	}

	SIM_CLKDIV1 &= ~(0xf<<28);
	SIM_CLKDIV1 |= ((div-1)<<28);

}
void SIM_SetOUTDIV4 (uint8_t div)
{
	if (div < 1 || div > 8)
	{
		while(1)
			;
	}

	SIM_CLKDIV1 &= ~(0x7<<16);
	SIM_CLKDIV1 |= ((div-1)<<16);
}

void SIM_GetUID (uint32_t* low, uint32_t* mid, uint32_t* high)
{
	*low = SIM_UIDL;
	*mid = SIM_UIDML;
	*high = SIM_UIDMH;
}


void SIM_DisableWatchDog()
{
	SIM_COPC = 0;
}
