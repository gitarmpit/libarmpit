#ifndef _NVMC_H 
#define _NVMC_H

#include "common.h"

// Power Management Controller 
// The power management controller (PMC) contains the internal voltage regulator
// power on reset (POR), and low voltage detect system.

#ifndef NVMC_BASE
#define NVMC_BASE     0x4003B000 
#endif

// Low Voltage Detect Status And Control 1 register
#define PMC_LVDSC1_LVDF   (1<<7) // Low-Voltage Detect Flag
#define PMC_LVDSC1_LVDACK (1<<6) // Low-Voltage Detect Acknowledge. Write 1 to clear LVDF
#define PMC_LVDSC1_LVDIE  (1<<5) // Low-Voltage Detect Interrupt Enable
#define PMC_LVDSC1_LVDRE  (1<<4) // Low-Voltage Detect Reset Enable (defaults to 1)

#ifdef __cplusplus
 extern "C" {
#endif

BOOL PMC_Is_LVDF();
void PMC_EnableInterrupt_LVD(BOOL enable);
void PMC_Clear_LVDF();
void PMC_EnableResetOnLVD(BOOL enable);

// 1.6V
void PMC_Select_LVDL();
// 2.6V
void PMC_Select_LVDH();

// Warnings
BOOL PMC_Is_LVWF();
void PMC_EnableInterrupt_LVW(BOOL enable);
void PMC_Clear_LVWF();

// 2.7V
void PMC_Select_LVWL();
// 2.8V
void PMC_Select_LVWM1();
// 2.9V
void PMC_Select_LVWM2();
// 3.0V
void PMC_Select_LVWH();

// Bandgap Enable In VLPx Operation
void PMC_EnableBG(BOOL enable);
// Bandgap Buffer Enable
void PMC_EnableBGB(BOOL enable);

BOOL PMC_Is_ACKISO();
// Restore peripheral cfg before clearing
void PMC_Clear_ACKISO();


#ifdef __cplusplus
}
#endif


#endif
