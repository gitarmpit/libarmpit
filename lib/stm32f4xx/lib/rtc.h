#ifndef _RTC_H
#define _RTC_H

#include "common.h"

// ********************  RTC APB1  *****************************
#define RTC_OFFSET 0x2800
#define RTC_BASE (PERIPH_BASE + RTC_OFFSET)
#define RTC_TR    (*(volatile uint32_t*)(RTC_BASE + 0x0))
#define RTC_DR    (*(volatile uint32_t*)(RTC_BASE + 0x4))
#define RTC_CR    (*(volatile uint32_t*)(RTC_BASE + 0x8))
#define RTC_ISR   (*(volatile uint32_t*)(RTC_BASE + 0xc))
#define RTC_PRER  (*(volatile uint32_t*)(RTC_BASE + 0x10))
#define RTC_WUTR  (*(volatile uint32_t*)(RTC_BASE + 0x14))
#define RTC_CALIBR  (*(volatile uint32_t*)(RTC_BASE + 0x18))
#define RTC_ALRMAR  (*(volatile uint32_t*)(RTC_BASE + 0x1c))
#define RTC_ALRMBR  (*(volatile uint32_t*)(RTC_BASE + 0x20))
#define RTC_WPR     (*(volatile uint32_t*)(RTC_BASE + 0x24))
#define RTC_SSR     (*(volatile uint32_t*)(RTC_BASE + 0x28))
#define RTC_SHIFTR  (*(volatile uint32_t*)(RTC_BASE + 0x2c))
#define RTC_TSTR    (*(volatile uint32_t*)(RTC_BASE + 0x30))
#define RTC_TSDR    (*(volatile uint32_t*)(RTC_BASE + 0x34))
#define RTC_TSSSR   (*(volatile uint32_t*)(RTC_BASE + 0x38))
#define RTC_CALR    (*(volatile uint32_t*)(RTC_BASE + 0x3c))
#define RTC_TAFCR   (*(volatile uint32_t*)(RTC_BASE + 0x40))
#define RTC_ALRMASSR  (*(volatile uint32_t*)(RTC_BASE + 0x44))
#define RTC_ALRMBSSR  (*(volatile uint32_t*)(RTC_BASE + 0x48))
#define RTC_RTC_BKPxR (*(volatile uint32_t*)(RTC_BASE + 0x50))


#define RTC_TR_SU 0
#define RTC_TR_SU_CLEARMASK 0xf
#define RTC_TR_ST 4
#define RTC_TR_ST_CLEARMASK (0x7<<RTC_TR_ST)
#define RTC_TR_MNU 8
#define RTC_TR_MNU_CLEARMASK (0xf<<RTC_TR_MNU)
#define RTC_TR_MNT 12
#define RTC_TR_MNT_CLEARMASK (0x7<<RTC_TR_MNT)
#define RTC_TR_HU 16
#define RTC_TR_HU_CLEARMASK (0xf<<RTC_TR_HU)
#define RTC_TR_HT 20
#define RTC_TR_HT_CLEARMASK (0x3<<RTC_TR_HT)
#define RTC_TR_PM 22

#define RTC_DR_DU 0
#define RTC_DR_DU_CLEARMASK 0xf
#define RTC_DR_DT 4
#define RTC_DR_DT_CLEARMASK (0x3<<RTC_DR_DT)
#define RTC_DR_MU 8
#define RTC_DR_MU_CLEARMASK (0xf<<RTC_DR_MU)
#define RTC_DR_MT 12
#define RTC_DR_MT_CLEARMASK (0x1<<RTC_DR_MT)
#define RTC_DR_WDU 13
#define RTC_DR_WDU_CLEARMASK (0x7<<RTC_DR_WDU)
#define RTC_DR_YU 16
#define RTC_DR_YU_CLEARMASK (0xf<<RTC_DR_YU)
#define RTC_DR_YT 20
#define RTC_DR_YT_CLEARMASK (0xf<<RTC_DR_YT)

#define RTC_CR_WUCKSE 0
#define RTC_CR_TSEDGE  (1<<3)
#define RTC_CR_REFCKON (1<<4)
#define RTC_CR_BYPSHAD (1<<5)
#define RTC_CR_FMT  (1<<6)
#define RTC_CR_DCE  (1<<7)
#define RTC_CR_ALRAE  (1<<8)
#define RTC_CR_ALRBE  (1<<9)
#define RTC_CR_WUTE  (1<<10)
#define RTC_CR_TSE  (1<<11)
#define RTC_CR_ALRAIE  (1<<12)
#define RTC_CR_ALRBIE  (1<<13)
#define RTC_CR_WUTIE  (1<<14)
#define RTC_CR_TSIE  (1<<15)
#define RTC_CR_ADD1H  (1<<16)
#define RTC_CR_SUB1H  (1<<17)
#define RTC_CR_BKP  (1<<18)
#define RTC_CR_COSEL  (1<<19)
#define RTC_CR_POL  (1<<20)

#define RTC_CR_OSEL_NONE  (0<<21)
#define RTC_CR_OSEL_ALA  (1<<21)
#define RTC_CR_OSEL_ALB  (2<<21)
#define RTC_CR_OSEL_WUP  (3<<21)
#define RTC_CR_OSEL_CLEARMASK  (3<<21)

#define RTC_CR_COE  (1<<23)

#define RTC_ISR_ALRAWF  (1<<0)
#define RTC_ISR_ALRBWF  (1<<1)
#define RTC_ISR_WUTWF   (1<<2)
#define RTC_ISR_SHPF    (1<<3)
#define RTC_ISR_INITS   (1<<4)
#define RTC_ISR_RSF     (1<<5)
#define RTC_ISR_INITF   (1<<6)
#define RTC_ISR_INIT    (1<<7)
#define RTC_ISR_ALRAF   (1<<8)
#define RTC_ISR_ALRBF   (1<<9)
#define RTC_ISR_WUTF    (1<<10)
#define RTC_ISR_TSF     (1<<11)
#define RTC_ISR_TSOVF   (1<<12)
#define RTC_ISR_TAMP1F  (1<<13)
#define RTC_ISR_TAMP2F  (1<<14)
#define RTC_ISR_RECALP  (1<<16)

#define RTC_PRER_PREDIV_S 0
#define RTC_PRER_PREDIV_S_MASK 0x7fff
#define RTC_PRER_PREDIV_A 16
#define RTC_PRER_PREDIV_A_MASK 0x7f


#define RTC_CALIBR_DC 0
#define RTC_CALIBR_DC_MASK 0x1f
#define RTC_CALIBR_DC_CDCS (1<<7)


#ifdef __cplusplus
extern "C"
{
#endif

void RTC_EnableWrite(BOOL enable);
void RTC_SetCalendar (uint16_t year, uint16_t month, uint16_t day, uint16_t hour, uint16_t min, uint16_t sec, BOOL isPM);
void RTC_GetCalendar(uint16_t* year, uint16_t* month, uint16_t* day, uint16_t* hour, uint16_t* min, uint16_t* sec, BOOL* isPM);
void RTC_GetDate(uint16_t* year, uint16_t* month, uint16_t* day);
void RTC_GetTime(uint16_t* hour, uint16_t* min, uint16_t* sec, BOOL* isPM);
void RTC_EnterInit(void);
void RTC_LeaveInit(void);

void RTC_Set_BYPSHAD(BOOL set);

#ifdef __cplusplus
}
#endif


#endif
