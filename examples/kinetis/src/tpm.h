#ifndef _TPM_H
#define _TPM_H

#include "common.h"
#include "port.h"
#include "nvic.h"

#define TPM0_BASE 0x40038000 
#define TPM1_BASE 0x40039000 
#define TPM2_BASE 0x4003A000 

#define TPM0_SC     (*(volatile uint32_t*)(TPM0_BASE + 0x0))
#define TPM1_SC     (*(volatile uint32_t*)(TPM1_BASE + 0x0))
#define TPM2_SC     (*(volatile uint32_t*)(TPM2_BASE + 0x0))

#define TPM0_CNT    (*(volatile uint32_t*)(TPM0_BASE + 0x4))
#define TPM1_CNT    (*(volatile uint32_t*)(TPM1_BASE + 0x4))
#define TPM2_CNT    (*(volatile uint32_t*)(TPM2_BASE + 0x4))

#define TPM0_MOD    (*(volatile uint32_t*)(TPM0_BASE + 0x8))
#define TPM1_MOD    (*(volatile uint32_t*)(TPM1_BASE + 0x8))
#define TPM2_MOD    (*(volatile uint32_t*)(TPM2_BASE + 0x8))

#define TPM0_STATUS (*(volatile uint32_t*)(TPM0_BASE + 0x50))
#define TPM1_STATUS (*(volatile uint32_t*)(TPM1_BASE + 0x50))
#define TPM2_STATUS (*(volatile uint32_t*)(TPM2_BASE + 0x50))

#define TPM0_CONF   (*(volatile uint32_t*)(TPM0_BASE + 0x84))
#define TPM1_CONF   (*(volatile uint32_t*)(TPM1_BASE + 0x84))
#define TPM2_CONF   (*(volatile uint32_t*)(TPM2_BASE + 0x84))

// #define TPM0_C0SC_BASE (TPM0_BASE + 0xC) 
// #define TPM0_C0SC_C0V  (TPM0_C0SC_BASE +  

#define TPM_SC_DMA   (1<<8)
#define TPM_SC_TOF   (1<<7)
#define TPM_SC_TOIE  (1<<6)
#define TPM_SC_CPWMS (1<<5)
#define TPM_SC_CMOD_OFF         (0<<3)
#define TPM_SC_CMOD_ON          (1<<3)
#define TPM_SC_CMOD_TPM_EXTCLK  (2<<3)
#define TPM_SC_CMOD_CLEARMASK   (3<<3)
#define TPM_SC_PS_CLEARMASK     (3<<0)

#define TPM_STATUS_TOF  (1<<8) 
#define TPM_STATUS_CH5F (1<<5)
#define TPM_STATUS_CH4F (1<<4)
#define TPM_STATUS_CH3F (1<<3)
#define TPM_STATUS_CH2F (1<<2)
#define TPM_STATUS_CH1F (1<<1)
#define TPM_STATUS_CH0F (1<<0)

// #define TPM_CONF_TRGSEL 
#define TPM_CONF_TRGSEL_CLEARMASK (4<<24) 

#define TPM_CONF_CROT    (1<<18)
#define TPM_CONF_CSOO    (1<<17)
#define TPM_CONF_CSOT    (1<<16)
#define TPM_CONF_GTBEEN  (1<<9)
#define TPM_CONF_DBGMODE (1<<6)
#define TPM_CONF_DOZEEN  (1<<5)

#define TPM0_CnCS_BASE (TPM0_BASE + 0xC)
#define TPM0_CnV_BASE  (TPM0_BASE + 0x10)
#define TPM1_CnCS_BASE (TPM1_BASE + 0xC)
#define TPM1_CnV_BASE  (TPM1_BASE + 0x10)
#define TPM2_CnCS_BASE (TPM2_BASE + 0xC)
#define TPM2_CnV_BASE  (TPM2_BASE + 0x10)

#define TPM_CnCS_CHF   (1<<7)
#define TPM_CnCS_CHIE  (1<<6)
#define TPM_CnCS_MSB   (1<<5)
#define TPM_CnCS_MSA   (1<<4)
#define TPM_CnCS_ELSB  (1<<3)
#define TPM_CnCS_ELSA  (1<<2)
#define TPM_CnCS_DMA   (1<<0)



typedef enum
{
    TPM0 = 0,
    TPM1 = 1,
    TPM2 = 2,
    TPM3 = 3,
    TPM4 = 4,
} TPM_N;

typedef enum
{
    TPM_DIV1   = 0,
	TPM_DIV2   = 1,
	TPM_DIV4   = 2,
	TPM_DIV8   = 3,
	TPM_DIV16  = 4,
	TPM_DIV32  = 5,
	TPM_DIV64  = 6,
	TPM_DIV128 = 7
} TPM_PRESC;

typedef struct _TPM
{
    volatile uint32_t* SIM_enableReg;
    uint32_t           SIM_enableMask;

    volatile uint32_t* TPM_SC;
    volatile uint32_t* TPM_CNT;
    volatile uint32_t* TPM_MOD;
    volatile uint32_t* TPM_STATUS;
    volatile uint32_t* TPM_CONF;
    uint32_t           TPM_CnCS_BASE;
    uint32_t           TPM_CnV_BASE;
    IRQn_Type          irq;
    void               (*handler)(struct _TPM*);

} TPM;

typedef struct
{
	volatile uint32_t* TPM_CnSC;
	volatile uint32_t* TPM_CnV;
	TPM*               tpm;
    void               (*handler)(void);

} TPM_Channel;


TPM* TPM_GetInstance(TPM_N n);
TPM_Channel TPM_GetChannel(TPM* tpm, uint8_t channel);
void TPM_EnableClock (TPM* tpm, BOOL enable);
void TPM_EnableDMA(TPM* tpm, BOOL enable);
void TPM_ClearTOF(TPM* tpm);
BOOL TPM_IsTOF(TPM* tpm);
void TPM_EnableInterrupt(TPM* tpm, BOOL enable);
void TPM_EnableCounter(TPM* tpm, BOOL enable);
void TPM_SetPrescaler(TPM* tpm, TPM_PRESC presc);
void TPM_ClearCounter (TPM* tpm);
uint16_t TPM_GetCounterValue (TPM* tpm);
void TPM_SetModulo(TPM* tpm, uint16_t mod);
void TPM_Channel_SetUpdatePeriod_us(TPM* tpm, uint32_t us);
void TPM_Channel_SetUpdatePeriod_ms(TPM* tpm, uint32_t ms);
void TPM_Channel_SetupPWM(TPM_Channel* ch, uint32_t period_us, uint32_t duty_us);
void TPM_Channel_UpdatePWMDuty(TPM_Channel* ch, uint32_t duty_us);
void TPM_Channel_ClearCHF(TPM_Channel* ch);
BOOL TPM_Channel_IsCHF(TPM_Channel* ch);
void TPM_Channel_EnableDMA(TPM_Channel* ch, BOOL enable);
void TPM_Channel_EnablePWM(TPM_Channel* ch);


#endif
