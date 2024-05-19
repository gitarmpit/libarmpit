


#ifndef _RCC_H
#define _RCC_H

#include "common.h"

#define HSI_CLOCK_FREQ  16000000

#ifndef HSE_CLOCK_FREQ 
#define HSE_CLOCK_FREQ 8000000
#endif


/* RCC  ****************************************************/
#define RCC_BASE    0x40023800


#include "rcc_cr.h"
#include "rcc_pllcfgr.h"
#include "rcc_cfgr.h"
#include "rcc_ahb.h"
#include "rcc_apb1.h"
#include "rcc_apb2.h"
#include "rcc_bdcr.h"
#include "rcc_csr.h"
#include "rcc_ckgatenr.h"

#define AHB1_BASE 0x40020000
#define AHB2_BASE 0x50000000
#define AHB3_BASE 0xA0001000 
#define APB2_BASE 0x40010000
#define APB1_BASE 0x40000000

#ifdef __cplusplus
extern "C"
{
#endif


void RCC_EnableHSI(BOOL enable);
void RCC_EnableHSE(BOOL enable);
void RCC_EnablePLL(uint8_t pllm, uint16_t plln, uint8_t pllp);
void RCC_DisablePLL(void);

void RCC_BypassHSE(BOOL bypassHSE);

uint32_t RCC_GetSystemClock(void); 
uint32_t RCC_GetAHB_CLK(void);
uint32_t RCC_GetAPB1_CLK(void);
uint32_t RCC_GetAPB2_CLK(void);
uint32_t RCC_GetTIMx_CLK(BOOL isAPB1);


    
//prescalers

void RCC_SetAHBPrescalerDiv1(void);    
void RCC_SetAHBPrescalerDiv2(void);
void RCC_SetAHBPrescalerDiv4(void);
void RCC_SetAHBPrescalerDiv8(void);
void RCC_SetAHBPrescalerDiv16(void);
void RCC_SetAHBPrescalerDiv64(void);
void RCC_SetAHBPrescalerDiv128(void);
void RCC_SetAHBPrescalerDiv256(void);
void RCC_SetAHBPrescalerDiv512(void);

void RCC_SetAPB1PrescalerDiv1(void);
void RCC_SetAPB1PrescalerDiv2(void);
void RCC_SetAPB1PrescalerDiv4(void);
void RCC_SetAPB1PrescalerDiv8(void);
void RCC_SetAPB1PrescalerDiv16(void);

void RCC_SetAPB2PrescalerDiv1(void);
void RCC_SetAPB2PrescalerDiv2(void);
void RCC_SetAPB2PrescalerDiv4(void);
void RCC_SetAPB2PrescalerDiv8(void);
void RCC_SetAPB2PrescalerDiv16(void);
    

//low level functions, can do without
BOOL RCC_calculate_pll_parameters (const uint32_t inputClockFreq, const uint32_t targetFreq, uint8_t* pllm, uint16_t* plln, uint8_t* pllp) ;
void RCC_SetPLLN (uint16_t plln);
void RCC_SetPLLM (uint8_t pllm) ;
void RCC_SetPLLP(uint8_t pllm);

void RCC_EnableSYSCFG(BOOL enable);
BOOL RCC_IsEnabledSYSCFG(void);

#ifdef __cplusplus
}
#endif




#endif
