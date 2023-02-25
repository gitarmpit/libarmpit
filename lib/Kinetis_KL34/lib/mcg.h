#ifndef _MCG_H
#define _MCG_H 

#include "common.h"

#ifndef MCG_BASE
#define MCG_BASE     0x40064000
#endif


#define MCG_C1    (*(volatile uint8_t*)(MCG_BASE + 0x0))
#define MCG_C2    (*(volatile uint8_t*)(MCG_BASE + 0x1))
#define MCG_C3    (*(volatile uint8_t*)(MCG_BASE + 0x2))  // Slow TRIM
#define MCG_C4    (*(volatile uint8_t*)(MCG_BASE + 0x3))
#define MCG_C5    (*(volatile uint8_t*)(MCG_BASE + 0x4))
#define MCG_C6    (*(volatile uint8_t*)(MCG_BASE + 0x5))
#define MCG_S     (*(volatile uint8_t*)(MCG_BASE + 0x6))
#define MCG_SC    (*(volatile uint8_t*)(MCG_BASE + 0x8))
#define MCG_ATCVH (*(volatile uint8_t*)(MCG_BASE + 0xa)) // Auto Trim Compare
#define MCG_ATCVL (*(volatile uint8_t*)(MCG_BASE + 0xb)) // Auto Trim Compare
#define MCG_C7    (*(volatile uint8_t*)(MCG_BASE + 0xc)) // OSCSEL
#define MCG_C8    (*(volatile uint8_t*)(MCG_BASE + 0xd))
#define MCG_C9    (*(volatile uint8_t*)(MCG_BASE + 0xe)) // Reserved
#define MCG_C10   (*(volatile uint8_t*)(MCG_BASE + 0xf)) // Reserved

// Selects the clock source for MCGOUTCLK
#define MCG_C1_CLKS_FLL_PLL    (0<<6)
#define MCG_C1_CLKS_IRC        (1<<6)
#define MCG_C1_CLKS_OSCCLK     (2<<6) // external
#define MCG_C1_CLKS_CLEARMASK  (3<<6)

#define MCG_C1_FRDIV_CLEARMASK (3<<3)
#define MCG_C1_IREFS           (1<<2)
#define MCG_C1_IRCLKEN         (1<<1) // MCGIRCLK enable
#define MCG_C1_REFSTEN         (1<<0) // stop enable

#define MCG_C2_LOCRE0 (1<<7)
#define MCG_C2_FCFTRIM (1<<6)

#define MCG_C2_RANGE0_LOW (0<<4)
#define MCG_C2_RANGE0_HIGH (1<<4)
#define MCG_C2_RANGE0_VERYHIGH (2<<4)
#define MCG_C2_RANGE0_CLEARMASK (3<<4)

#define MCG_C2_HGO0 (1<<3)
#define MCG_C2_EREFS0 (1<<2)
#define MCG_C2_LP     (1<<1)
#define MCG_C2_IRCS   (1<<0)

#define MCG_C4_DMX32 (1<<7)
#define MCG_C4_DRST_DRS_24 (0<<5) // DCO (FLL)
#define MCG_C4_DRST_DRS_48 (1<<5)
#define MCG_C4_DRST_DRS_72 (2<<5)
#define MCG_C4_DRST_DRS_96 (3<<5)
#define MCG_C4_DRST_DRS_CLEARMASK (3<<5)
#define MCG_C4_DRST_DRS_24 (0<<5)
#define MCG_C4_FCTRIM_START 1
#define MCG_C4_FCTRIM_CLEARMASK (3<<1)
#define MCG_C4_SCFTRIM (1<<0)

#define MCG_C5_PLLCLKEN0 (1<<6)
#define MCG_C5_PLLSTEN0  (1<<5)
#define MCG_C5_PRDIV0_CLEARMASK 0xf

#define MCG_C6_LOLIE0 (1<<7)
#define MCG_C6_PLLS   (1<<6)
#define MCG_C6_CME0   (1<<5)
#define MCG_C6_VDIV0_CLEARMASK 0x1f

#define MCG_S_LOLS0         (1<<7)
#define MCG_S_LOCK0         (1<<6)
#define MCG_S_PLLST         (1<<5)
#define MCG_S_IREFST        (1<<4)

#define MCG_S_CLKST_POS     2
#define MCG_S_CLKST_FLL     (0<<MCG_S_CLKST_POS)
#define MCG_S_CLKST_IRC     (1<<MCG_S_CLKST_POS)
#define MCG_S_CLKST_OSCCLK  (2<<MCG_S_CLKST_POS)
#define MCG_S_CLKST_PLL     (3<<MCG_S_CLKST_POS)

#define MCG_S_OSCINIT0      (1<<1)
#define MCG_S_IRCST         (1<<0)

#define MCG_SC_ATME       (1<<7)
#define MCG_SC_ATMS       (1<<6)
#define MCG_SC_ATMF       (1<<5)
#define MCG_SC_FLTPRSRV   (1<<4)
#define MCG_SC_FCRDIV_OFF        3
#define MCG_SC_FCRDIV_CLEARKMASK (3<<3)
#define MCG_SC_LOCS0             (1<<0)

#define MCG_C7_OSCSEL (1<<0)

#ifdef __cplusplus
 extern "C" {
#endif

// IRC is enabled in Stop mode
//    if IRCLKEN is set or if MCG is in FEI, FBI, or BLPI
//    modes before entering Stop mode.
void MCG_IRCStopEnable(BOOL isEnabled);
void MCG_DisableFLLPLL_InLowPowerMode(BOOL isDisabled);
void MCG_PLLStopEnable(BOOL enable);

void MCG_Set_FCFTRIM(BOOL isOn);

// 14. C2 bit 5-4
// Selects the frequency range for the crystal oscillator or external clock source
void MCG_Set_OSC_Range_Low();
//<=8Mhz
void MCG_Set_OSC_Range_High();
//<=16Mhz
void MCG_Set_OSC_Range_VeryHigh();

void MCG_Set_HighGain_OSC(BOOL isOn);

// 15.
void MCG_Set_ExternalClock_OSC();
void MCG_Set_ExternalClock_ERC();


// 2.
void MCG_Set_IRC_Slow();
void MCG_Set_IRC_Fast();

void MCG_SetFLL_Freq_24(BOOL DMX32);
void MCG_SetFLL_Freq_48(BOOL DMX32);
void MCG_SetFLL_Freq_72(BOOL DMX32);
void MCG_SetFLL_Freq_96(BOOL DMX32);


// PLL
// 5.
void MCG_EnablePLLClock(BOOL enable);
BOOL MCG_Is_PLL_Locked();

// 1 to 25
void MCG_Set_PLL_DIV(uint8_t div);

//PLL Output divider (multiplier) 24-55
void MCG_Set_VCO_MUL(uint8_t mul);

// 6.
void MCG_Set_PLLS_PLL();
void MCG_Set_PLLS_FLL();

BOOL  MCG_Is_LOLS0();
BOOL  MCG_Is_LOCK0();
BOOL  MCG_Is_PLLST();
BOOL  MCG_Is_IREFST();
BOOL  MCG_Is_CLKST_FLL();
BOOL  MCG_Is_CLKST_IRC();
BOOL  MCG_Is_CLKST_OSCCLK();
BOOL  MCG_Is_CLKST_PLL();
BOOL  MCG_Is_OSCINIT0();
BOOL  MCG_Is_IRCST();

//1. 4Mhz IRC divider (/2 on reset)
void MCG_Set_FCRDIV1();
void MCG_Set_FCRDIV2();
void MCG_Set_FCRDIV4();
void MCG_Set_FCRDIV8();
void MCG_Set_FCRDIV16();
void MCG_Set_FCRDIV32();
void MCG_Set_FCRDIV64();
void MCG_Set_FCRDIV128();

// 7.
void MCG_Select_MCGOUTCLK_FLL_PLL();
void MCG_Select_MCGOUTCLK_IRC();
void MCG_Select_MCGOUTCLK_OSCCLK();

// 8. OSCLK div, 0 to 7
void MCG_Set_FRDIV(uint8_t val);

// 4.
void MCG_Set_FLL_IRC32K();
void MCG_Set_FLL_OSCCLK();

// 3.
void MCG_Enable_MCGIRCLK (BOOL isOn);

void InitClock_FEI_Common();

void InitClock_FEI_24Mhz_Bus_24Mhz();
void InitClock_FEI_48Mhz_Bus_24Mhz();
void InitClock_FEI_72Mhz_Bus_24Mhz();
void InitClock_FEI_72Mhz_Bus_36Mhz();
void InitClock_FEI_96Mhz_Bus_24Mhz();
void InitClock_FEI_96Mhz_Bus_32Mhz();

// FLL with external XTAL OSC
// For 32K XTAL: set frdiv = 0  high = FALSE
// For 4Mhz XTAL:  set frdiv = 128  high = TRUE
void InitClock_FEE_24Mhz_Bus_24Mhz(uint8_t frdiv, BOOL high);
void InitClock_FEE_48Mhz_Bus_24Mhz(uint8_t frdiv, BOOL high);
void InitClock_FEE_72Mhz_Bus_24Mhz(uint8_t frdiv, BOOL high);
void InitClock_FEE_72Mhz_Bus_36Mhz(uint8_t frdiv, BOOL high);
void InitClock_FEE_96Mhz_Bus_24Mhz(uint8_t frdiv, BOOL high);
void InitClock_FEE_96Mhz_Bus_32Mhz(uint8_t frdiv, BOOL high);

// 4Mhz internal
void InitClock_FBI_Fast();
// 32K internal
void InitClock_FBI_Slow();
void InitClock_BLPI();

#ifdef __cplusplus
}
#endif


#endif

