#ifndef _PWR_H
#define _PWR_H

#include "common.h"


#include "rcc.h"

/* PWR   APB1 ****************************************************/
#define PWR_OFFSET 0x7000
#define PWR_BASE  (PERIPH_BASE + PWR_OFFSET)

#define PWR_CR  (*(volatile uint32_t*)(PWR_BASE))
#define PWR_CR_VOS  (1<<14)  
#define PWR_CR_FPDS  (1<<9)
#define PWR_CR_DBP  (1<<8)   //backup domain write protection
#define PWR_CR_PLS (1<<5)
#define PWR_CR_PVDE (1<<4)  //power voltage detector enable
#define PWR_CR_CSBF (1<<3)  //clear standby flag (write 1 to clear)
#define PWR_CR_CWUF (1<<2)  //clear the WUF wake-up flag

#define PWR_CR_PDDS (1<<1)  //set and cleared by software. It works together with the LPDS bit
                            //0: STOP on deepsleep   1: STANDBY on deepsleep

#define PWR_CR_LPDS (1<<0)  //set and cleared by software. It works together with the PDDS bit.
                            //0: Voltage regulator on during Stop mode
                            //1: Voltage regulator in low-power mode during Stop mode

#define PWR_CSR  (*(volatile uint32_t*)(PWR_BASE + 0x4))
  
#define PWR_CSR_EWUP (1<<8)     //1: WKUP pin is used for wakeup from Standby mode and forced in input pull down 
                                //   configuration (rising edge on WKUP pin wakes-up the system from Standby mode).

#define PWR_CSR_PVDO (1<<2)  //This bit is set and cleared by hardware. It is valid only if PVD is enabled by the PVDE bit. 
                             //0: VDD/VDDAis higher than the PVD threshold selected with the PLS[2:0] bits. 
                             //1: VDD/VDDAis lower than the PVD threshold selected with the PLS[2:0] bits.

#define PWR_CSR_SBF (1<<1)   //Standby flag  (read only)
                             //0: Device has not been in Standby mode
                             //1: Device has been in Standby mode

#define PWR_CSR_WUF (1<<0)   //Wakeup flag  (read only) 
                             //0: No wakeup event occurred
                             //1: A wakeup event was received from the WKUP pin or from the RTC alarm

#ifdef __cplusplus
extern "C"
{
#endif


void PWR_EnablePeripheralClock(BOOL enable);
void PWR_DisableWriteProtection(BOOL disable);

void PWR_Sleep(BOOL isWFI, BOOL sleepOnExit);
void PWR_Stop(BOOL isLowPower, BOOL isWFI, BOOL sleepOnExit);
void PWR_Standby(BOOL isWFI, BOOL enableWakeupPin, BOOL sleepOnExit);
void PWR_EnableWakeupPin(BOOL enable);
BOOL PWR_IsSBF(void);
void PWR_ClearSBF(void);
BOOL PWR_IsWUF(void);
void PWR_ClearWUF(void);

#ifdef __cplusplus
}
#endif


#endif
