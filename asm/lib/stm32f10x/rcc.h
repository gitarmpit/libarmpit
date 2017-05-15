#ifndef _RCC_H
#define _RCC_H

#include "../common.h"

#ifndef HSE_CLOCK_FREQ 
#define HSE_CLOCK_FREQ 4000000
#endif

#define HSI_CLOCK_FREQ 8000000



#if defined(__GNUC__) && defined(__ASSEMBLY__)



#define RCC_BASE    0x40021000


#define RCC_CR        (RCC_BASE + 0x00)
#define RCC_CFGR      (RCC_BASE + 0x04)
#define RCC_APB2RSTR  (RCC_BASE + 0x0c)
#define RCC_APB1RSTR  (RCC_BASE + 0x10)
#define RCC_AHBENR    (RCC_BASE + 0x14)
#define RCC_APB2ENR   (RCC_BASE + 0x18)
#define RCC_APB1ENR   (RCC_BASE + 0x1c)
#define RCC_BDCR      (RCC_BASE + 0x20)
#define RCC_CSR       (RCC_BASE + 0x24)
#define RCC_CFGR2     (RCC_BASE + 0x2c)



#define RCC_CR_HSION  (1<<0)
#define RCC_CR_HSIRDY (1<<1)
#define RCC_CR_HSEON  (1<<16)        //set by sw (enable)
#define RCC_CR_HSERDY (1<<17)        //set by hw
#define RCC_CR_HSEBYP (1<<18)
#define RCC_CR_CSSON  (1<<19)
#define RCC_CR_PLLON  (1<<24)        //set by sw (enable)
#define RCC_CR_PLLRDY (1<<25)        //set by hw
    

//clock source
#define RCC_CFGR_SW_HSI      (0<<0)
#define RCC_CFGR_SW_HSE      (1<<0)
#define RCC_CFGR_SW_PLL      (2<<0)
#define RCC_CFGR_SW_CLEARMASK    (3<<0)

#define RCC_CFGR_SWS_HSI      (0<<2)
#define RCC_CFGR_SWS_HSE      (1<<2)
#define RCC_CFGR_SWS_PLL      (2<<2)


//AHB prescaler (up to 72Mhz)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV1   (0<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV2   (8<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV4   (9<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV8   (10<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV16  (11<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV64  (12<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV128 (13<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV256 (14<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV512 (15<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_CLEARMASK (15<<4)

//APB1 low speed prescaler (up to 32Mhz) AHB / div
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV1  (0<<8)
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV2  (4<<8)
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV4  (5<<8)
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV8  (6<<8)
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV16 (7<<8)
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_CLEARMASK (7<<8)


//APB2 high speed prescaler (up to 72 Mhz)  AHB / div
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV1  (0<<11)
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV2  (4<<11)
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV4  (5<<11)
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV8  (6<<11)
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV16 (7<<11)
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_CLEARMASK (7<<11)


//ADC presc:  APB2 / DIV
#define RCC_CFGR_ADCPRE_APB2_PCLK2_DIV2 (0<<14)
#define RCC_CFGR_ADCPRE_APB2_PCLK2_DIV4 (1<<14)
#define RCC_CFGR_ADCPRE_APB2_PCLK2_DIV6 (2<<14)
#define RCC_CFGR_ADCPRE_APB2_PCLK2_DIV8 (3<<14)
#define RCC_CFGR_ADCPRE_APB2_PCLK2_CLEARMASK (3<<14)

#define RCC_CFGR_PLLSRC_HSI_DIV2    (0<<16)
#define RCC_CFGR_PLLSRC_PREDIV1     (1<<16)
#define RCC_CFGR_PLLSRC_HSE         (1<<16)
#define RCC_CFGR_PLLSRC_CLEARMASK   (1<<16)

#define RCC_CFGR_PLLXTPRE      (1<<17)

#define RCC_CFGR_PLL_MUL2  (0<<18)
#define RCC_CFGR_PLL_MUL3  (1<<18)
#define RCC_CFGR_PLL_MUL4  (2<<18)
#define RCC_CFGR_PLL_MUL5  (3<<18)
#define RCC_CFGR_PLL_MUL6  (4<<18)
#define RCC_CFGR_PLL_MUL7  (5<<18)
#define RCC_CFGR_PLL_MUL8  (6<<18)
#define RCC_CFGR_PLL_MUL9  (7<<18)
#define RCC_CFGR_PLL_MUL10 (8<<18)
#define RCC_CFGR_PLL_MUL11 (9<<18)
#define RCC_CFGR_PLL_MUL12 (10<<18)
#define RCC_CFGR_PLL_MUL13 (11<<18)
#define RCC_CFGR_PLL_MUL14 (12<<18)
#define RCC_CFGR_PLL_MUL15 (13<<18)
#define RCC_CFGR_PLL_MUL16 (14<<18)
#define RCC_CFGR_PLL_CLEARMASK (14<<18)

//0xx: no clock, 100: sysclk, 101: HSI, 110: HSE, 111: PLL/2
#define MCO_OFF       (0<<24)
#define MCO_SYSCLK    (4<<24)
#define MCO_HSI       (5<<24)
#define MCO_HSE       (6<<24)
#define MCO_PLL_DIV2  (7<<24)
#define MCO_CLEARMASK (7<<24)


#define RCC_APB2RSTR_AFIORST   (1<<0)
#define RCC_APB2RSTR_GIOPARST  (1<<2)
#define RCC_APB2RSTR_GIOPBRST  (1<<3)
#define RCC_APB2RSTR_GIOPCRST  (1<<4)
#define RCC_APB2RSTR_GIOPDRST  (1<<5)
#define RCC_APB2RSTR_GIOPERST  (1<<6)
#define RCC_APB2RSTR_GIOPFRST  (1<<7)
#define RCC_APB2RSTR_GIOPGRST  (1<<8)
#define RCC_APB2RSTR_ADC1RST   (1<<9)
#define RCC_APB2RSTR_ADC2RST   (1<<10)
#define RCC_APB2RSTR_TIM1RST   (1<<11)
#define RCC_APB2RSTR_SPI1RST   (1<<12)
#define RCC_APB2RSTR_TIM8RST   (1<<13)
#define RCC_APB2RSTR_USART1RST (1<<14)
#define RCC_APB2RSTR_ADC3RST   (1<<15)

//100x vl
#define RCC_APB2RSTR_TIM15RST  (1<<16)
#define RCC_APB2RSTR_TIM16RST  (1<<17)
#define RCC_APB2RSTR_TIM17RST  (1<<18)

#define RCC_APB2RSTR_TIM9RST  (1<<19)
#define RCC_APB2RSTR_TIM10RST (1<<20)
#define RCC_APB2RSTR_TIM11RST (1<<21)


#define RCC_APB1RSTR_TIM2RST  (1<<0)
#define RCC_APB1RSTR_TIM3RST  (1<<1)
#define RCC_APB1RSTR_TIM4RST  (1<<2)
#define RCC_APB1RSTR_TIM5RST  (1<<3)
#define RCC_APB1RSTR_TIM6RST  (1<<4)
#define RCC_APB1RSTR_TIM7RST  (1<<5)
#define RCC_APB1RSTR_TIM12RST (1<<6)
#define RCC_APB1RSTR_TIM13RST (1<<7)
#define RCC_APB1RSTR_TIM14RST (1<<8)
#define RCC_APB1RSTR_WWDGRST (1<<11)
#define RCC_APB1RSTR_SPI2RST (1<<14)
#define RCC_APB1RSTR_SPI3RST (1<<15)
#define RCC_APB1RSTR_USART2RST (1<<17)
#define RCC_APB1RSTR_USART3RST (1<<18)
#define RCC_APB1RSTR_UART4RST (1<<19)
#define RCC_APB1RSTR_UART5RST (1<<20)
#define RCC_APB1RSTR_I2C1RST (1<<21)
#define RCC_APB1RSTR_I2C2RST (1<<22)
#define RCC_APB1RSTR_USBRST (1<<23)
#define RCC_APB1RSTR_CANRST (1<<25)
#define RCC_APB1RSTR_BKPRST   (1<<27)
#define RCC_APB1RSTR_PWRRST   (1<<28)
#define RCC_APB1RSTR_DACRST   (1<<29)
#define RCC_APB1RSTR_CECRST   (1<<30)


#define RCC_AHBENR_DMA1EN   (1<<0)
#define RCC_AHBENR_DMA2EN   (1<<1)
#define RCC_AHBENR_SRAMEN   (1<<2)
#define RCC_AHBENR_FLITFEN  (1<<4)
#define RCC_AHBENR_CRCEN    (1<<6)
#define RCC_AHBENR_FSMCEN   (1<<8)
#define RCC_AHBENR_SDIOEN   (1<<10)
#define RCC_AHBENR_OTGFSEN   (1<<12)
#define RCC_AHBENR_ETHMACEN   (1<<14)
#define RCC_AHBENR_ETHMACTXEN   (1<<15)
#define RCC_AHBENR_ETHMACRXEN   (1<<16)


#define RCC_APB2ENR_AFIOEN   (1<<0)
#define RCC_APB2ENR_IOPAEN  (1<<2)
#define RCC_APB2ENR_IOPBEN  (1<<3)
#define RCC_APB2ENR_IOPCEN  (1<<4)
#define RCC_APB2ENR_IOPDEN  (1<<5)
#define RCC_APB2ENR_IOPEEN  (1<<6)
#define RCC_APB2ENR_IOPFEN  (1<<7)
#define RCC_APB2ENR_IOPGEN  (1<<8)
#define RCC_APB2ENR_ADC1EN   (1<<9)
#define RCC_APB2ENR_ADC2EN   (1<<10)
#define RCC_APB2ENR_TIM1EN   (1<<11)
#define RCC_APB2ENR_SPI1EN   (1<<12)
#define RCC_APB2ENR_TIM8EN   (1<<13)
#define RCC_APB2ENR_USART1EN (1<<14)
#define RCC_APB2ENR_ADC3EN   (1<<15)

//100x vl
#define RCC_APB2ENR_TIM15EN  (1<<16)
#define RCC_APB2ENR_TIM16EN  (1<<17)
#define RCC_APB2ENR_TIM17EN  (1<<18)

#define RCC_APB2ENR_TIM9EN  (1<<19)
#define RCC_APB2ENR_TIM10EN (1<<20)
#define RCC_APB2ENR_TIM11EN (1<<21)



#define RCC_APB1ENR_TIM2EN  (1<<0)
#define RCC_APB1ENR_TIM3EN  (1<<1)
#define RCC_APB1ENR_TIM4EN  (1<<2)
#define RCC_APB1ENR_TIM5EN  (1<<3)
#define RCC_APB1ENR_TIM6EN  (1<<4)
#define RCC_APB1ENR_TIM7EN  (1<<5)
#define RCC_APB1ENR_TIM12EN (1<<6)
#define RCC_APB1ENR_TIM13EN (1<<7)
#define RCC_APB1ENR_TIM14EN (1<<8)
#define RCC_APB1ENR_WWDGEN (1<<11)
#define RCC_APB1ENR_SPI2EN (1<<14)
#define RCC_APB1ENR_SPI3EN (1<<15)
#define RCC_APB1ENR_USART2EN (1<<17)
#define RCC_APB1ENR_USART3EN (1<<18)
#define RCC_APB1ENR_UART4EN (1<<19)
#define RCC_APB1ENR_UART5EN (1<<20)
#define RCC_APB1ENR_I2C1EN (1<<21)
#define RCC_APB1ENR_I2C2EN (1<<22)
#define RCC_APB1ENR_USBEN (1<<23)
#define RCC_APB1ENR_CANEN (1<<25)
#define RCC_APB1ENR_BKPEN   (1<<27)
#define RCC_APB1ENR_PWREN   (1<<28)
#define RCC_APB1ENR_DACEN   (1<<29)
#define RCC_APB1ENR_CECEN   (1<<30)

#define RCC_BDCR_LSEON  (1<<0)         //set by sw (enable)
#define RCC_BDCR_LSERDY (1<<1)         //set by hw


#define RCC_BDCR_RTC_NOCLK        (0<<8)
#define RCC_BDCR_RTC_LSE          (1<<8)
#define RCC_BDCR_RTC_LSI          (2<<8)
#define RCC_BDCR_RTC_HSE128       (3<<8)
#define RCC_BDCR_RTC_CLEARMASK    (3<<8)

#define RCC_BDCR_RTCEN   (1<<15)
#define RCC_BDCR_BDRST   (1<<16)   //1: resets the entire backup domain


#define RCC_CSR_LSION  (1<<0)         //set by sw (enable)
#define RCC_CSR_LSIRDY (1<<1)         //set by hw

//HSE prescaler  for f100xx and f10x connectivity
#define RCC_CFGR2_PREDIV1_DIV1   0
#define RCC_CFGR2_PREDIV1_DIV2   1
#define RCC_CFGR2_PREDIV1_DIV3   2
#define RCC_CFGR2_PREDIV1_DIV4   3
#define RCC_CFGR2_PREDIV1_DIV5   4
#define RCC_CFGR2_PREDIV1_DIV6   5
#define RCC_CFGR2_PREDIV1_DIV7   6
#define RCC_CFGR2_PREDIV1_DIV8   7
#define RCC_CFGR2_PREDIV1_DIV9   8
#define RCC_CFGR2_PREDIV1_DIV10  9
#define RCC_CFGR2_PREDIV1_DIV11  10
#define RCC_CFGR2_PREDIV1_DIV12  11
#define RCC_CFGR2_PREDIV1_DIV13  12
#define RCC_CFGR2_PREDIV1_DIV14  13
#define RCC_CFGR2_PREDIV1_DIV15  14
#define RCC_CFGR2_PREDIV1_DIV16  15
#define RCC_CFGR2_PREDIV1_CLEARMASK 15


#else 


void RCC_SetAHBPrescaler(uint16_t);
void RCC_SetAPB1Prescaler(uint8_t);
void RCC_SetAPB2Prescaler(uint8_t);
void RCC_SetADCPrescaler(uint8_t); 

void RCC_EnableHSI(BOOL enable);
void RCC_EnableHSE(BOOL enable);
void RCC_EnablePLL(uint8_t pllm_mul);

void RCC_BypassHSE(BOOL bypassHSE);


void RCC_EnableLSI(void);
void RCC_EnableLSE(void);


uint32_t RCC_GetSystemClock(void);
uint32_t RCC_GetAHB_CLK(void);
uint32_t RCC_GetAPB1_CLK(void);
uint32_t RCC_GetAPB2_CLK(void);
uint32_t RCC_GetTIMx_CLK(BOOL isAPB1);
uint32_t RCC_GetRTC_CLK(void);
  
void RCC_EnablePWR(BOOL enable);
void RCC_EnableBKP(BOOL enable);
void RCC_EnableRTC(BOOL enable);
void RCC_SetRTCClockSourceLSE(void);
void RCC_SetRTCClockSourceLSI(void);
void RCC_SetRTCClockSourceHSE_DIV128(void);
void RCC_SetRTCClockSourceOFF(void);

void RCC_ResetBKP(BOOL enable);

void RCC_EnablePeripheral (BOOL enable,
                            uint32_t* en_addr,
                            uint32_t en_bit,
                            uint32_t* rst_addr,
                            uint32_t rst_bit);


#endif

#endif
