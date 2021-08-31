#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "common.h"

// ARM

#define  SYST_CSR   (*(volatile uint32_t*)(0xE000E010))    //control
#define  SYST_LOAD (*(volatile uint32_t*)(0xE000E014))    //reload value
#define  SYST_VAL   (*(volatile uint32_t*)(0xE000E018))    //current value
#define  SYST_CALIB (*(volatile uint32_t*)(0xE000E01C))


#define  SYST_CSR_ENABLE   (1<<0) 
#define  SYST_CSR_TICKINT  (1<<1) 
#define  SYST_CSR_CLKSOURCE_CORE        (1<<2) 
#define  SYST_CSR_COUNTFLAG  (1<<16) 


#ifdef __cplusplus
 extern "C" {
#endif

void SystickEnable (uint32_t count, BOOL enableInterrupt, BOOL isDIV16, void (*handler)(void));

void SystickDisable(void);
void SystickSetReloadValue (uint32_t count);

int32_t SystickGetValue(void);

#ifdef __cplusplus
}
#endif


#endif
