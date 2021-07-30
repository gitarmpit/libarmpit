#ifndef _RTC_H
#define _RTC_H

#include "common.h"

#ifndef RTC_BASE
#define RTC_BASE     0x4003D000
#endif

#define RTC_TSR    (*(volatile uint32_t*)(RTC_BASE + 0x0))
#define RTC_TPR    (*(volatile uint32_t*)(RTC_BASE + 0x4))
#define RTC_TAR    (*(volatile uint32_t*)(RTC_BASE + 0x8))
#define RTC_TCR    (*(volatile uint32_t*)(RTC_BASE + 0xC))
#define RTC_CR     (*(volatile uint32_t*)(RTC_BASE + 0x10))
#define RTC_SR     (*(volatile uint32_t*)(RTC_BASE + 0x14))
#define RTC_LR     (*(volatile uint32_t*)(RTC_BASE + 0x18))
#define RTC_IER    (*(volatile uint32_t*)(RTC_BASE + 0x1C))

#define RTC_CR_SC2P  (1<<13)
#define RTC_CR_SC4P  (1<<12)
#define RTC_CR_SC8P  (1<<11)
#define RTC_CR_SC16P (1<<10)
#define RTC_CR_CLKO  (1<<9)
#define RTC_CR_OSCE  (1<<8)
#define RTC_CR_SWR   (1<<0)

#define RTC_SR_TCE   (1<<4)
#define RTC_SR_TAF   (1<<2)
#define RTC_SR_TOF   (1<<1)
#define RTC_SR_TIF   (1<<0)


#define RTC_IER_TSIE (1<<4)
#define RTC_IER_TAIE (1<<2)
#define RTC_IER_TOIE (1<<1)
#define RTC_IER_TIIE (1<<0)


void RTC_EnableOSC(BOOL enable);
void RTC_EnableCounter(BOOL enable);
void RTC_EnableERCLK32K(BOOL enable);
void RTC_SWReset();
void RTC_EnableSecondsInt(BOOL enable, void (*handler)(void));
void RTC_EnableAlarmInt(BOOL enable, void (*handler)(void));



#endif
