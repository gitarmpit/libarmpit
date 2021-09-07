#ifndef _PMC_H 
#define _PMC_H

#include "common.h"

// Power Management Controller 
// The power management controller (PMC) contains the internal voltage regulator
// power on reset (POR), and low voltage detect system.

#define PMC_LVDSC1 (*(volatile uint8_t*)0x4007D000)
#define PMC_LVDSC2 (*(volatile uint8_t*)0x4007D001)
#define PMC_REGSC  (*(volatile uint8_t*)0x4007D002)

// Low Voltage Detect Status And Control 1 register
#define PMC_LVDSC1_LVDF   (1<<7) // Low-Voltage Detect Flag
#define PMC_LVDSC1_LVDACK (1<<6) // Low-Voltage Detect Acknowledge. Write 1 to clear LVDF
#define PMC_LVDSC1_LVDIE  (1<<5) // Low-Voltage Detect Interrupt Enable
#define PMC_LVDSC1_LVDRE  (1<<4) // Low-Voltage Detect Reset Enable (defaults to 1)

// Low-Voltage Detect Voltage Select
#define PMC_LVDSC1_LVDV_LVDL      0 // Low trip point selected   ~1.6V 
#define PMC_LVDSC1_LVDV_LVDH      1 // High trip point selected  ~2.6V
#define PMC_LVDSC1_LVDV_CLEARMASK 3

// Low Voltage Detect Status And Control 2 register  (warnings)
#define PMC_LVDSC2_LVWF   (1<<7) // Low-Voltage Warning Flag 
#define PMC_LVDSC2_LVWACK (1<<6) // Low-Voltage Warning Acknowledge
#define PMC_LVDSC2_LVWIE  (1<<5) // Low-Voltage Warning Interrupt Enable

// Low-Voltage Warning Voltage Select
#define PMC_LVDSC2_LVWV_LOW        0  // 2.7V
#define PMC_LVDSC2_LVWV_MID1       1  // 2.8V
#define PMC_LVDSC2_LVWV_MID2       2  // 2.9V
#define PMC_LVDSC2_LVWV_HIGH       3  // 3.0V
#define PMC_LVDSC2_LVWV_CLEARMASK  3

// Regulator Status And Control register 
#define PMC_REGSC_BGEN   (1<<4) // Bandgap Enable In VLPx Operation
#define PMC_REGSC_ACKISO (1<<3) // Acknowledge Isolation. 1: Certain peripherals and I/O pads are in an isolated and latched state
#define PMC_REGSC_REGONS (1<<2) // 1: Regulator is in run regulation
#define PMC_REGSC_BGBE   (1<<0) // Bandgap Buffer Enable

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
