#ifndef _SIM_H
#define _SIM_H 
#include "common.h"

#ifndef SIM_BASE
#define SIM_BASE     0x40047000
#endif

#define SIM_SOPT1    (*(volatile uint32_t*)(SIM_BASE + 0x0))
#define SIM_SOPT1CFG (*(volatile uint32_t*)(SIM_BASE + 0x4))
#define SIM_SOPT2    (*(volatile uint32_t*)(SIM_BASE + 0x1004))
#define SIM_SOPT4    (*(volatile uint32_t*)(SIM_BASE + 0x100C)) // External Clock Pin Select for TPM
#define SIM_SOPT5    (*(volatile uint32_t*)(SIM_BASE + 0x1010)) // UART open drain & Rx/Tx source select, defaults are fine
#define SIM_SOPT7    (*(volatile uint32_t*)(SIM_BASE + 0x1018)) // ADC0: alternate trigger, pretrigger A/B, trigger select
#define SIM_SDID     (*(volatile uint32_t*)(SIM_BASE + 0x1024))
#define SIM_SCGC4    (*(volatile uint32_t*)(SIM_BASE + 0x1034))
#define SIM_SCGC5    (*(volatile uint32_t*)(SIM_BASE + 0x1038))
#define SIM_SCGC6    (*(volatile uint32_t*)(SIM_BASE + 0x103C))
#define SIM_SCGC7    (*(volatile uint32_t*)(SIM_BASE + 0x1040))
#define SIM_CLKDIV1  (*(volatile uint32_t*)(SIM_BASE + 0x1044))
#define SIM_FCFG1    (*(volatile uint32_t*)(SIM_BASE + 0x104C)) // Flash cfg
#define SIM_FCFG2    (*(volatile uint32_t*)(SIM_BASE + 0x1050)) // Flash cfg
#define SIM_UIDMH    (*(volatile uint32_t*)(SIM_BASE + 0x1058))
#define SIM_UIDML    (*(volatile uint32_t*)(SIM_BASE + 0x105C))
#define SIM_UIDL     (*(volatile uint32_t*)(SIM_BASE + 0x1060))
#define SIM_COPC     (*(volatile uint32_t*)(SIM_BASE + 0x1100)) // Watchdog
#define SIM_SRVCOP   (*(volatile uint32_t*)(SIM_BASE + 0x1104)) // Watchdog

// SOPT1 register is only reset on POR or LVD
#define SIM_SOPT1_OSC32KSEL_OSC32KCLK  (0<<18)
#define SIM_SOPT1_OSC32KSEL_RTC_CLKIN  (2<<18)
#define SIM_SOPT1_OSC32KSEL_LPO        (3<<18)
#define SIM_SOPT1_OSC32KSEL_CLEARMASK  (3<<18)

// 27-26
#define SIM_SOPT2_UART0SRC_OFF        (0<<26)
#define SIM_SOPT2_UART0SRC_MCGFLLCLK  (1<<26) // MCG output of the FLL
#define SIM_SOPT2_UART0SRC_OSCERCLK   (2<<26) // external reference clock
#define SIM_SOPT2_UART0SRC_MCGIRCLK   (3<<26) // internal reference clock (32KHz or 4MHz)
#define SIM_SOPT2_UART0SRC_CLEARMASK  (3<<26)

// 25-24
#define SIM_SOPT2_TPMSRC_OFF          (0<<24)
#define SIM_SOPT2_TPMSRC_MCGFLLCLK    (1<<24) // MCG output of the FLL
#define SIM_SOPT2_TPMSRC_OSCERCLK     (2<<24) // external reference clock
#define SIM_SOPT2_TPMSRC_MCGIRCLK     (3<<24) // internal reference clock (32KHz or 4MHz)
#define SIM_SOPT2_TPMSRC_CLEARMASK    (3<<24)

#define SIM_SOPT2_PLLFLLSEL           (1<<16)

// 7-5
#define SIM_SOPT2_CLKOUTSEL_BUS       (2<<5)
#define SIM_SOPT2_CLKOUTSEL_LPO       (3<<5)
#define SIM_SOPT2_CLKOUTSEL_MCGIRCLK  (4<<5)
#define SIM_SOPT2_CLKOUTSEL_OSCERCLK  (6<<5)
#define SIM_SOPT2_CLKOUTSEL_CLEARMASK (7<<5)

#define SIM_SOPT2_RTCCLKOUTSEL       (1<<4)
void SIM_Select_RTC_CLOCKOUT_RTC();
void SIM_Select_RTC_CLOCKOUT_OSCERCLK();

void SIM_Get_SDID(uint8_t* famID, uint8_t* subfamID, uint8_t* seriesID, uint8_t* sramSize, uint8_t* revID, uint8_t* dieID, uint8_t* pinID);

#define SIM_SCGC4_SPI1     (1<<23)
#define SIM_SCGC4_SPI0     (1<<22)
#define SIM_SCGC4_CMP      (1<<19)
#define SIM_SCGC4_UART2    (1<<12)
#define SIM_SCGC4_UART1    (1<<11)
#define SIM_SCGC4_UART0    (1<<10)
#define SIM_SCGC4_I2C1     (1<<7)
#define SIM_SCGC4_I2C0     (1<<6)

#define SIM_SCGC5_SLCD     (1<<19)
#define SIM_SCGC5_PORTE    (1<<13)
#define SIM_SCGC5_PORTD    (1<<12)
#define SIM_SCGC5_PORTC    (1<<11)
#define SIM_SCGC5_PORTB    (1<<10)
#define SIM_SCGC5_PORTA    (1<<9)
#define SIM_SCGC5_TSI      (1<<5)
#define SIM_SCGC5_LPTMR    (1<<0)

#define SIM_SCGC6_DAC0    (1<<31)
#define SIM_SCGC6_RTC     (1<<29)
#define SIM_SCGC6_ADC0    (1<<27)
#define SIM_SCGC6_TPM2    (1<<26)
#define SIM_SCGC6_TPM1    (1<<25)
#define SIM_SCGC6_TPM0    (1<<24)
#define SIM_SCGC6_PIT     (1<<23)
#define SIM_SCGC6_DMAMUX  (1<<1)
#define SIM_SCGC6_FTF     (1<<0)

#define SIM_SCGC7_DMA     (1<<8)


#ifdef __cplusplus
extern "C"
{
#endif

void SIM_Select_RTCClock_OSC32KCLK();
void SIM_Select_RTCClock_RTC_CLKIN();
void SIM_Select_RTCClock_LPO();

void SIM_Select_UART0SRC_OFF();
void SIM_Select_UART0SRC_MCGFLLCLK();
void SIM_Select_UART0SRC_OSCERCLK();
void SIM_Select_UART0SRC_MCGIRCLK();
void SIM_Select_TPMSRC_OFF();
void SIM_Select_TPMSRC_MCGFLLCLK();
void SIM_Select_TPMSRC_OSCERCLK();
void SIM_Select_TPMSRC_MCGIRCLK();

// Selects the MCGPLLCLK
void SIM_Select_PLL();
// Selects the MCGFLLCLK
void SIM_Select_FLL();

void SIM_SelectCLKOUT_BUS();
void SIM_SelectCLKOUT_LPO();
void SIM_SelectCLKOUT_MCGIRCLK();
void SIM_SelectCLKOUT_OSCERCLK();

void SIM_Enable(volatile uint32_t* port, uint32_t mask, BOOL enable);
void SIM_Enable_SPI1  (BOOL enable);
void SIM_Enable_SPI0  (BOOL enable);
void SIM_Enable_CMP   (BOOL enable);
void SIM_Enable_UART2 (BOOL enable);
void SIM_Enable_UART1 (BOOL enable);
void SIM_Enable_UART0 (BOOL enable);
void SIM_Enable_I2C1  (BOOL enable);
void SIM_Enable_I2C0  (BOOL enable);
void SIM_Enable_SLCD  (BOOL enable);
void SIM_Enable_PORTE (BOOL enable);
void SIM_Enable_PORTD (BOOL enable);
void SIM_Enable_PORTC (BOOL enable);
void SIM_Enable_PORTB (BOOL enable);
void SIM_Enable_PORTA (BOOL enable);
void SIM_Enable_TSI   (BOOL enable);
void SIM_Enable_LPTMR (BOOL enable);
void SIM_Enable_DAC0  (BOOL enable);
void SIM_Enable_RTC   (BOOL enable);
void SIM_Enable_ADC0  (BOOL enable);
void SIM_Enable_TPM2  (BOOL enable);
void SIM_Enable_TPM1  (BOOL enable);
void SIM_Enable_TPM0  (BOOL enable);
void SIM_Enable_PIT    (BOOL enable);
void SIM_Enable_DMAMUX (BOOL enable);
void SIM_Enable_FTF    (BOOL enable);
void SIM_Enable_DMA    (BOOL enable);

// 1 to 16
void SIM_SetOUTDIV1 (uint8_t div);
// 1 to 8
void SIM_SetOUTDIV4 (uint8_t div);
void SIM_GetUID (uint32_t* low, uint32_t* mid, uint32_t* high);
void SIM_DisableWatchDog();


#ifdef __cplusplus
}
#endif


#endif

