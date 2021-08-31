#ifndef _RCM_H
#define _RCM_H

#include "common.h"

// Reset Control Module (get reset source)

#ifndef RCM_BASE
#define RCM_BASE     0x4007F000 
#endif


#define RCM_SRS0    (*(volatile uint8_t*)(RCM_BASE + 0x0))
#define RCM_SRS1    (*(volatile uint8_t*)(RCM_BASE + 0x1))
#define RCM_RPFC    (*(volatile uint8_t*)(RCM_BASE + 0x4))
#define RCM_RPFW    (*(volatile uint8_t*)(RCM_BASE + 0x5))

// Reset Source
#define RCM_SRS0_POR     (1<<7)
#define RCM_SRS0_PIN     (1<<6)
#define RCM_SRS0_WDOG    (1<<5)
#define RCM_SRS0_LOL     (1<<3)
#define RCM_SRS0_LOC     (1<<2)
#define RCM_SRS0_LVD     (1<<1)
#define RCM_SRS0_WAKEUP  (1<<0)

// Reset Source 1
#define RCM_SRS1_SACKERR  (1<<5)
#define RCM_SRS1_MDM_AP   (1<<3)
#define RCM_SRS1_SW       (1<<2)
#define RCM_SRS1_LOCKUP   (1<<1)

typedef struct _RCM_SRC
{
    BOOL isPOR;    // Power-on reset
    BOOL isPIN;    // External /reset pin
    BOOL isWDOG;
    BOOL isLOL;    // Loss of Lock
    BOOL isLOC;    // Loss of Clock
    BOOL isLVD;    // Low-Voltage Detect Reset
    BOOL isWAKEUP; // Low Leakage Wakeup Reset (LLS/VLLS modes)

    BOOL isSACKERR; // Stop Mode Acknowledge Error Reset
    BOOL isMDM_AP;  // Reset caused by host debugger system setting of the System Reset Request bit
    BOOL isSW;      // SYSRESETREQ bit in Application Interrupt and Reset Control Register in the ARM core.
    BOOL isLOCKUP;  // Core Lockup

} RCM_SRC_t;


#define RCM_RPFC_RSTFLTSS            (1<<2)
#define RCM_RPFC_RSTFLTSRW_DISABLED  0
#define RCM_RPFC_RSTFLTSRW_BUS       1  // Bus clock filter enabled for normal operation
#define RCM_RPFC_RSTFLTSRW_LPO       2  // LPO clock filter enabled for normal operation
#define RCM_RPFC_RSTFLTSRW_CLEARMASK 3

#ifdef __cplusplus
 extern "C" {
#endif

void RCM_GetResetSource(RCM_SRC_t* rstSrc);

#ifdef __cplusplus
}
#endif


#endif
