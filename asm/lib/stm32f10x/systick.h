#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "../common.h"

#if defined(__GNUC__) && defined(__ASSEMBLY__)


#define  SYST_CSR    0xE000E010    //control
#define  SYST_LOAD   0xE000E014    //reload value
#define  SYST_VAL    0xE000E018    //current value
#define  SYST_CALIB  0xE000E01C

#define  SYST_CSR_ENABLE   (1<<0) 
#define  SYST_CSR_TICKINT  (1<<1) 
#if defined(STM32) 
#define  SYST_CSR_CLKSOURCE_AHB_DIV8   (0<<2) 
#define  SYST_CSR_CLKSOURCE_AHB        (1<<2) 
#else
#define  SYST_CSR_CLKSOURCE_CORE        (1<<2) 
#endif
#define  SYST_CSR_CLKSOURCE_CLEARMASK  (1<<2) 
#define  SYST_CSR_COUNTFLAG  (1<<16) 

#else

#ifdef __cplusplus
 extern "C" {
#endif

#if defined(STM32) 
void SystickEnable (uint32_t count, BOOL enableInterrupt, BOOL isDIV8);
#else 
void SystickEnable (uint32_t count, BOOL enableInterrupt);
#endif

void SystickDisable(void);
void SystickSetReloadValue (uint32_t count);

#ifdef __cplusplus
 }
 #endif

#endif

#endif
