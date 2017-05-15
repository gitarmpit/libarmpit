#ifndef _TIMER_H
#define _TIMER_H

#include "common.h"

#if defined (__GNUC__) && defined(__ASSEMBLY__)


#define TIM2_BASE  0x40000000  //APB1
#define TIM3_BASE  0x40000400  //APB1
#define TIM6_BASE  0x40001000  //APB1
#define TIM7_BASE  0x40001400  //APB1

#if defined(STM32F1) || defined(STM32F2) || defined(STM32F4)

#define TIM4_BASE  0x40000800  //APB1
#define TIM5_BASE  0x40000C00  //APB1
#define TIM12_BASE 0x40001800  //APB1
#define TIM13_BASE 0x40001C00  //APB1
#define TIM14_BASE 0x40002000  //APB1

#if defined(STM32F1) 
#define TIM1_BASE   0x40012C00  //APB2
#define TIM8_BASE   0x40013400  //APB2
#define TIM9_BASE   0x40014C00  //APB2
#define TIM10_BASE  0x40015000  //APB2
#define TIM11_BASE  0x40015400  //APB2
#else 
#define TIM1_BASE   0x40010000   //APB2
#define TIM8_BASE   0x40010000   //APB2
#define TIM9_BASE   0x40014000  //APB2
#define TIM10_BASE  0x40014400  //APB2
#define TIM11_BASE  0x40014800  //APB2
#endif


#if defined(STM32F1)
#define TIM15_BASE  0x40014000  //APB2
#define TIM16_BASE  0x40014400  //APB2
#define TIM17_BASE  0x40014800  //APB2
#endif

#elif defined(STM32L0)

#define TIM21_BASE  0x40010800 //APB2
#define TIM22_BASE  0x40011400 //APB2

#else

#error "device not defined"

#endif

#define TIM_CR1_OFFSET   0x00
#define TIM_CR2_OFFSET   0x04
#define TIM_SMCR_OFFSET  0x08
#define TIM_DIER_OFFSET  0x0c
#define TIM_SR_OFFSET    0x10
#define TIM_EGR_OFFSET   0x14
#define TIM_CCMR1_OFFSET 0x18
#define TIM_CCMR2_OFFSET 0x1c
#define TIM_CCER_OFFSET  0x20
#define TIM_CNT_OFFSET   0x24
#define TIM_PSC_OFFSET   0x28
#define TIM_ARR_OFFSET   0x2c
#define TIM_RCR_OFFSET   0x30
#define TIM_CCR1_OFFSET  0x34
#define TIM_CCR2_OFFSET  0x38
#define TIM_CCR3_OFFSET  0x3c
#define TIM_CCR4_OFFSET  0x40
#define TIM_DCR_OFFSET   0x48
#define TIM_BDTR_OFFSET  0x44
#define TIM_DMAR_OFFSET  0x4c


#define TIM_CR1_CEN (1<<0) //counter enable
#define TIM_CR1_UDIS (1<<1) //update disable: set/cleared by software
                            //0:Update event enabled. 1: UEV disabled
#define TIM_CR1_URS  (1<<2) //set/cleared by sw to select the UEV sources
//0: any of these generated an update interrupt: counter overflow/underflow, setting the UG bit, 
//1: only counter overflow/underflow 

#define TIM_CR1_OPM  (1<<3) //one pulse mode
//0: counter is not stopped at update event 
//1: counter stops counting at the next update event and CEN is cleared
#define TIM_CR1_DIR  (1<<4)  //direction: 0: upcounter, 1: downcounter
#define TIM_CR1_CMS_EDGE_ALIGNED (0<<5)
#define TIM_CR1_CMS_CENTER_ALIGNED_DOWN (1<<5)
#define TIM_CR1_CMS_CENTER_ALIGNED_UP (2<<5)
#define TIM_CR1_CMS_CENTER_ALIGNED_UP_DOWN (3<<5)
#define TIM_CR1_CMS_CLEARMASK       (3<<5)
#define TIM_CR1_ARPE  (1<<7)  //auto preload enable, 0: TIMx_ARR is not buffered, 1: buffered
#define TIM_CR1_CKD_CKINT (0<<8)
#define TIM_CR1_CKD_CKINT2 (1<<8)
#define TIM_CR1_CKD_CKINT4 (2<<8)
#define TIM_CR1_CKD_CLEARMASK (3<<8)

#define TIM_CR2_CCDS                (1<<3)

#define TIM_CR2_MMS_RESET           (0<<4)
#define TIM_CR2_MMS_ENABLE          (1<<4)
#define TIM_CR2_MMS_UPDATE          (2<<4)
#define TIM_CR2_MMS_COMPARE_PULSE   (3<<4)
#define TIM_CR2_MMS_COMPARE_OC1REF  (4<<4)
#define TIM_CR2_MMS_COMPARE_OC2REF  (5<<4)
#define TIM_CR2_MMS_COMPARE_OC3REF  (6<<4)
#define TIM_CR2_MMS_COMPARE_OC4REF  (7<<4)
#define TIM_CR2_MMS_CLEARMASK       (7<<4)
#define TIM_CR2_TI1S                (1<<7)  //TIM1/8 2/3/4/5, 100x: TIM15/16/17


#define TIM_DIER_UIE    (1<<0) //update interrupt enable
#define TIM_DIER_CC1IE  (1<<1) //Capture/compare int enable
#define TIM_DIER_CC2IE  (1<<2)
#define TIM_DIER_CC3IE  (1<<3)
#define TIM_DIER_CC4IE  (1<<4)
#define TIM_DIER_TIE    (1<<6) //Trigger interrupt enable 
#define TIM_DIER_UDE    (1<<8) //Update DMA request enable 
#define TIM_DIER_CC1DE  (1<<9) //Capture/compare DMA enable
#define TIM_DIER_CC2DE  (1<<10)
#define TIM_DIER_CC3DE  (1<<11)
#define TIM_DIER_CC4DE  (1<<12)
#define TIM_DIER_TDE    (1<<14) //Trigger DMA request enable


#define TIM_SR_UIF    (1<<0) //update interrupt flag
#define TIM_SR_CC1IF  (1<<1) //Capture/compare int
#define TIM_SR_CC2IF  (1<<2) //
#define TIM_SR_CC3IF  (1<<3) //
#define TIM_SR_CC4IF  (1<<4) //
#define TIM_SR_TIF    (1<<6) //
#define TIM_SR_CC1OF  (1<<9) //overcapture flag
#define TIM_SR_CC2OF  (1<<10)
#define TIM_SR_CC3OF  (1<<11)
#define TIM_SR_CC4OF  (1<<12)


#define TIM_CCMR_CC1_BITPOS         0
#define TIM_CCMR_IC1PSC_BITPOS      2
#define TIM_CCMR_IC1F_BITPOS        4
#define TIM_CCMR_OC1M_BITPOS        4

//0-1
#define TIM_CCMR1_CC1S_OUT             0
#define TIM_CCMR1_CC1S_IN_TI1          1
#define TIM_CCMR1_CC1S_IN_TI2          2
#define TIM_CCMR1_CC1S_IN_TRC          3
#define TIM_CCMR1_CC1S_CLEARMASK       3

//2-3 out
#define TIM_CCMR1_OC1FE              (1<<2) //fast enable
#define TIM_CCMR1_OC1PE              (1<<3) //preload enable

//2-3 input capture prescaler
#define TIM_CCMR1_IC1PSC_DIV1             (0<<2)
#define TIM_CCMR1_IC1PSC_DIV2             (1<<2)
#define TIM_CCMR1_IC1PSC_DIV4             (2<<2)
#define TIM_CCMR1_IC1PSC_DIV8             (3<<2)
#define TIM_CCMR1_IC1PSC_CLEARMASK        (3<<2)

//4-6 output
#define TIM_CCMR1_OC1M_FROZEN             (0<<4)
#define TIM_CCMR1_OC1M_ACTIVE_ON_MATCH    (1<<4)
#define TIM_CCMR1_OC1M_INACTIVE_ON_MATCH  (2<<4)
#define TIM_CCMR1_OC1M_TOGGLE             (3<<4)
#define TIM_CCMR1_OC1M_FORCE_LOW          (4<<4)
#define TIM_CCMR1_OC1M_FORCE_HIGH         (5<<4)
#define TIM_CCMR1_OC1M_PWM1               (6<<4)
#define TIM_CCMR1_OC1M_PWM2               (7<<4)
#define TIM_CCMR1_OC1M_CLEARMASK          (7<<4)
//7  out
#define TIM_CCMR1_OC1CE                   (1<<7)

//4-7 input capture filter
#define TIM_CCMR1_IC1F_FDTS        (0<<4) 
#define TIM_CCMR1_IC1F_FCK_INT_2   (1<<4)
#define TIM_CCMR1_IC1F_FCK_INT_4   (2<<4)
#define TIM_CCMR1_IC1F_FCK_INT_8   (3<<4)
#define TIM_CCMR1_IC1F_FDTS_2_6    (4<<4)
#define TIM_CCMR1_IC1F_FDTS_2_8    (5<<4)
#define TIM_CCMR1_IC1F_FDTS_4_6    (6<<4)
#define TIM_CCMR1_IC1F_FDTS_4_8    (7<<4)
#define TIM_CCMR1_IC1F_FDTS_8_6    (8<<4)
#define TIM_CCMR1_IC1F_FDTS_8_8    (9<<4)
#define TIM_CCMR1_IC1F_FDTS_16_5   (10<<4)
#define TIM_CCMR1_IC1F_FDTS_16_6   (11<<4)
#define TIM_CCMR1_IC1F_FDTS_16_8   (12<<4)
#define TIM_CCMR1_IC1F_FDTS_32_5   (13<<4)
#define TIM_CCMR1_IC1F_FDTS_32_6   (14<<4)
#define TIM_CCMR1_IC1F_FDTS_32_8   (15<<4)
#define TIM_CCMR1_IC1F_CLEARMASK   (15<<4)
//8-9
#define TIM_CCMR1_CC2S_OUT             (0<<8)               
#define TIM_CCMR1_CC2S_IN_TI2          (1<<8)        
#define TIM_CCMR1_CC2S_IN_TI1          (2<<8)        
#define TIM_CCMR1_CC2S_IN_TRC          (3<<8)   
#define TIM_CCMR1_CC2S_CLEARMASK       (3<<8)
//10-11 out
#define TIM_CCMR1_OC2FE                   (1<<10)                 
#define TIM_CCMR1_OC2PE                   (1<<11)                 
//10-11 input capture prescaler
#define TIM_CCMR1_IC2PSC_DIV1             (0<<10)
#define TIM_CCMR1_IC2PSC_DIV2             (1<<10)
#define TIM_CCMR1_IC2PSC_DIV4             (2<<10)
#define TIM_CCMR1_IC2PSC_DIV8             (3<<10)

//12-14 output
#define TIM_CCMR1_OC2M_FROZEN             (0<<12)
#define TIM_CCMR1_OC2M_ACTIVE_ON_MATCH    (1<<12)
#define TIM_CCMR1_OC2M_INACTIVE_ON_MATCH  (2<<12)
#define TIM_CCMR1_OC2M_TOGGLE             (3<<12)
#define TIM_CCMR1_OC2M_FORCE_LOW          (4<<12)
#define TIM_CCMR1_OC2M_FORCE_HIGH         (5<<12)
#define TIM_CCMR1_OC2M_PWM1               (6<<12)
#define TIM_CCMR1_OC2M_PWM2               (7<<12)
//15 out
#define TIM_CCMR1_OC2CE                   (1<<15)


//12-15 input capture filter
#define TIM_CCMR1_IC2F_FDTS        (0<<12) 
#define TIM_CCMR1_IC2F_FCK_INT_2   (1<<12)
#define TIM_CCMR1_IC2F_FCK_INT_4   (2<<12)
#define TIM_CCMR1_IC2F_FCK_INT_8   (3<<12)
#define TIM_CCMR1_IC2F_FDTS_2_6    (4<<12)
#define TIM_CCMR1_IC2F_FDTS_2_8    (5<<12)
#define TIM_CCMR1_IC2F_FDTS_4_6    (6<<12)
#define TIM_CCMR1_IC2F_FDTS_4_8    (7<<12)
#define TIM_CCMR1_IC2F_FDTS_8_6    (8<<12)
#define TIM_CCMR1_IC2F_FDTS_8_8    (9<<12)
#define TIM_CCMR1_IC2F_FDTS_16_5   (10<<12)
#define TIM_CCMR1_IC2F_FDTS_16_6   (11<<12)
#define TIM_CCMR1_IC2F_FDTS_16_8   (12<<12)
#define TIM_CCMR1_IC2F_FDTS_32_5   (13<<12)
#define TIM_CCMR1_IC2F_FDTS_32_6   (14<<12)
#define TIM_CCMR1_IC2F_FDTS_32_8   (15<<12)


//0-1
#define TIM_CCMR2_CC3S_OUT             (0<<0)               
#define TIM_CCMR2_CC3S_IN_TI3          (1<<0)        
#define TIM_CCMR2_CC3S_IN_TI4          (2<<0)        
#define TIM_CCMR2_CC3S_IN_TRC          (3<<0)   



//2-3 out
#define TIM_CCMR2_OC3FE                   TIM_CCMR1_OC1FE                 
#define TIM_CCMR2_OC3PE                   TIM_CCMR1_OC1PE    

//2-3 input capture prescaler
#define TIM_CCMR2_IC3PSC_DIV1             TIM_CCMR1_IC1PSC_DIV1
#define TIM_CCMR2_IC3PSC_DIV2             TIM_CCMR1_IC1PSC_DIV2
#define TIM_CCMR2_IC3PSC_DIV4             TIM_CCMR1_IC1PSC_DIV4
#define TIM_CCMR2_IC3PSC_DIV8             TIM_CCMR1_IC1PSC_DIV8

//4-6 output
#define TIM_CCMR2_OC3M_FROZEN             TIM_CCMR1_OC1M_FROZEN           
#define TIM_CCMR2_OC3M_ACTIVE_ON_MATCH    TIM_CCMR1_OC1M_ACTIVE_ON_MATCH  
#define TIM_CCMR2_OC3M_INACTIVE_ON_MATCH  TIM_CCMR1_OC1M_INACTIVE_ON_MATCH
#define TIM_CCMR2_OC3M_TOGGLE             TIM_CCMR1_OC1M_TOGGLE           
#define TIM_CCMR2_OC3M_FORCE_LOW          TIM_CCMR1_OC1M_FORCE_LOW        
#define TIM_CCMR2_OC3M_FORCE_HIGH         TIM_CCMR1_OC1M_FORCE_HIGH       
#define TIM_CCMR2_OC3M_PWM1               TIM_CCMR1_OC1M_PWM1             
#define TIM_CCMR2_OC3M_PWM2               TIM_CCMR1_OC1M_PWM2             
//7  out
#define TIM_CCMR2_OC3CE                   TIM_CCMR1_OC1CE                 

//4-7 input capture filter
#define TIM_CCMR2_IC3F_FDTS        TIM_CCMR1_IC1F_FDTS     
#define TIM_CCMR2_IC3F_FCK_INT_2   TIM_CCMR1_IC1F_FCK_INT_2
#define TIM_CCMR2_IC3F_FCK_INT_4   TIM_CCMR1_IC1F_FCK_INT_4
#define TIM_CCMR2_IC3F_FCK_INT_8   TIM_CCMR1_IC1F_FCK_INT_8
#define TIM_CCMR2_IC3F_FDTS_2_6    TIM_CCMR1_IC1F_FDTS_2_6 
#define TIM_CCMR2_IC3F_FDTS_2_8    TIM_CCMR1_IC1F_FDTS_2_8 
#define TIM_CCMR2_IC3F_FDTS_4_6    TIM_CCMR1_IC1F_FDTS_4_6 
#define TIM_CCMR2_IC3F_FDTS_4_8    TIM_CCMR1_IC1F_FDTS_4_8 
#define TIM_CCMR2_IC3F_FDTS_8_6    TIM_CCMR1_IC1F_FDTS_8_6 
#define TIM_CCMR2_IC3F_FDTS_8_8    TIM_CCMR1_IC1F_FDTS_8_8 
#define TIM_CCMR2_IC3F_FDTS_16_5   TIM_CCMR1_IC1F_FDTS_16_5
#define TIM_CCMR2_IC3F_FDTS_16_6   TIM_CCMR1_IC1F_FDTS_16_6
#define TIM_CCMR2_IC3F_FDTS_16_8   TIM_CCMR1_IC1F_FDTS_16_8
#define TIM_CCMR2_IC3F_FDTS_32_5   TIM_CCMR1_IC1F_FDTS_32_5
#define TIM_CCMR2_IC3F_FDTS_32_6   TIM_CCMR1_IC1F_FDTS_32_6
#define TIM_CCMR2_IC3F_FDTS_32_8   TIM_CCMR1_IC1F_FDTS_32_8

//8-9 in/out
#define TIM_CCMR2_CC4S_OUT             (0<<8)               
#define TIM_CCMR2_CC4S_IN_TI4          (1<<8)        
#define TIM_CCMR2_CC4S_IN_TI3          (2<<8)        
#define TIM_CCMR2_CC4S_IN_TRC          (3<<8)   

//10-11 out
#define TIM_CCMR2_OC4FE                   TIM_CCMR1_OC2FE                 
#define TIM_CCMR2_OC4PE                   TIM_CCMR1_OC2PE    

//10-11 input capture prescaler
#define TIM_CCMR2_IC4PSC_DIV1             TIM_CCMR1_IC2PSC_DIV1
#define TIM_CCMR2_IC4PSC_DIV2             TIM_CCMR1_IC2PSC_DIV2
#define TIM_CCMR2_IC4PSC_DIV4             TIM_CCMR1_IC2PSC_DIV4
#define TIM_CCMR2_IC4PSC_DIV8             TIM_CCMR1_IC2PSC_DIV8


//12-14 output
#define TIM_CCMR2_OC4M_FROZEN             TIM_CCMR1_OC2M_FROZEN           
#define TIM_CCMR2_OC4M_ACTIVE_ON_MATCH    TIM_CCMR1_OC2M_ACTIVE_ON_MATCH  
#define TIM_CCMR2_OC4M_INACTIVE_ON_MATCH  TIM_CCMR1_OC2M_INACTIVE_ON_MATCH
#define TIM_CCMR2_OC4M_TOGGLE             TIM_CCMR1_OC2M_TOGGLE           
#define TIM_CCMR2_OC4M_FORCE_LOW          TIM_CCMR1_OC2M_FORCE_LOW        
#define TIM_CCMR2_OC4M_FORCE_HIGH         TIM_CCMR1_OC2M_FORCE_HIGH       
#define TIM_CCMR2_OC4M_PWM1               TIM_CCMR1_OC2M_PWM1             
#define TIM_CCMR2_OC4M_PWM2               TIM_CCMR1_OC2M_PWM2             
//15 out
#define TIM_CCMR2_OC4CE                   TIM_CCMR1_OC2CE                 


//12-15 input capture filter
#define TIM_CCMR2_IC4F_FDTS              TIM_CCMR1_IC2F_FDTS         
#define TIM_CCMR2_IC4F_FCK_INT_2         TIM_CCMR1_IC2F_FCK_INT_2 
#define TIM_CCMR2_IC4F_FCK_INT_4         TIM_CCMR1_IC2F_FCK_INT_4 
#define TIM_CCMR2_IC4F_FCK_INT_8         TIM_CCMR1_IC2F_FCK_INT_8 
#define TIM_CCMR2_IC4F_FDTS_2_6          TIM_CCMR1_IC2F_FDTS_2_6  
#define TIM_CCMR2_IC4F_FDTS_2_8          TIM_CCMR1_IC2F_FDTS_2_8  
#define TIM_CCMR2_IC4F_FDTS_4_6          TIM_CCMR1_IC2F_FDTS_4_6  
#define TIM_CCMR2_IC4F_FDTS_4_8          TIM_CCMR1_IC2F_FDTS_4_8  
#define TIM_CCMR2_IC4F_FDTS_8_6          TIM_CCMR1_IC2F_FDTS_8_6  
#define TIM_CCMR2_IC4F_FDTS_8_8          TIM_CCMR1_IC2F_FDTS_8_8  
#define TIM_CCMR2_IC4F_FDTS_16_5         TIM_CCMR1_IC2F_FDTS_16_5 
#define TIM_CCMR2_IC4F_FDTS_16_6         TIM_CCMR1_IC2F_FDTS_16_6 
#define TIM_CCMR2_IC4F_FDTS_16_8         TIM_CCMR1_IC2F_FDTS_16_8 
#define TIM_CCMR2_IC4F_FDTS_32_5         TIM_CCMR1_IC2F_FDTS_32_5 
#define TIM_CCMR2_IC4F_FDTS_32_6         TIM_CCMR1_IC2F_FDTS_32_6 
#define TIM_CCMR2_IC4F_FDTS_32_8         TIM_CCMR1_IC2F_FDTS_32_8 


#define TIM_CCER_CC1E  (1<<0)
#define TIM_CCER_CC1P  (1<<1)

#ifdef STM32F4
#define TIM_CCER_CC1NP  (1<<3)
#endif

#define TIM_CCER_CC2E  (1<<4)
#define TIM_CCER_CC2P  (1<<5)
#define TIM_CCER_CC3E  (1<<8)
#define TIM_CCER_CC3P  (1<<9)
#define TIM_CCER_CC4E  (1<<12)
#define TIM_CCER_CC4P  (1<<13)


#define TIM_SMCR_SMS_OFF        0
#define TIM_SMCR_SMS_ENC1       1  
#define TIM_SMCR_SMS_ENC2       2  
#define TIM_SMCR_SMS_ENC3       3 
#define TIM_SMCR_SMS_RESET      4 
#define TIM_SMCR_SMS_GATED      5
#define TIM_SMCR_SMS_TRIGGER    6
#define TIM_SMCR_SMS_EXT        7
#define TIM_SMCR_SMS_CLEARMASK 7
//100x
#define TIM_SMCR_OCCS        (1<<3)

#define TIM_SMCR_TS_ITR0        (0<<4)
#define TIM_SMCR_TS_ITR1        (1<<4)
#define TIM_SMCR_TS_ITR2        (2<<4)
#define TIM_SMCR_TS_ITR3        (3<<4)
#define TIM_SMCR_TS_TI1F_ED     (4<<4)
#define TIM_SMCR_TS_TI1FP1      (5<<4)
#define TIM_SMCR_TS_TI1FP2      (6<<4)
#define TIM_SMCR_TS_ETRF        (7<<4)
#define TIM_SMCR_TS_CLEARMASK   (7<<4)
#define TIM_SMCR_MSM            (1<<7)
//external trigger stuff


#define TIM_EGR_UG    (1<<0)
#define TIM_EGR_CC1G  (1<<1)
#define TIM_EGR_CC2G  (1<<2)
#define TIM_EGR_CC3G  (1<<3)
#define TIM_EGR_CC4G  (1<<4)
#define TIM_EGR_TG    (1<<6)

#define TIM_DCR_DBA_OFFSET   0
#define TIM_DCR_DBL_OFFSET   8

#define TIM_BDTR_MOE  (1<<15)


#else

#include <stdint.h>

#define TIM1 0

#define TIM2 1
#define TIM3 2

#if !defined(STM32F10X_LD) && !defined(STM32F10X_LD_VL)
#define TIM4 3
#endif

#if defined(STM32F4) || defined(STM32F2) || defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL) || defined(STM32F10X_CL)
#define TIM5 4
#endif


#if (defined(STM32F4) || defined(STM32F2) || defined(VALUE_LINE)  \
|| defined(STM32F10X_HD)  || defined(STM32F10X_XL) || defined(STM32F10X_CL)) \
&& !defined(STM32F401xC)  && !defined(STM32F401xE) && !defined(STM32F401xx) && !defined(STM32F411xE)

#define TIM6 5
#define TIM7 6

#endif


#if (defined(STM32F10X_XL) || defined(STM32F10X_HD) || defined(STM32F4) || defined(STM32F2)) \
&& !defined(STM32F401xC)  && !defined(STM32F401xE) && !defined(STM32F401xx) && !defined(STM32F411xE)
#define TIM8 7
#endif

#if defined(STM32F2) || defined(STM32F4) || defined(STM32F10X_XL)
#define TIM9  8
#define TIM10 9
#define TIM11 10
#endif



#if  defined(STM32F10X_HD_VL) || (defined(STM32F10X_XL) || defined(STM32F2) || defined(STM32F4)) \
&& !defined(STM32F401xC)  && !defined(STM32F401xE) && !defined(STM32F401xx) && !defined(STM32F411xE)

#define TIM12 11
#endif



#if (defined(STM32F2) || defined(STM32F4) || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL)) \
&& !defined(STM32F401xC)  && !defined(STM32F401xE) && !defined(STM32F401xx) && !defined(STM32F411xE)

#define TIM13 12
#define TIM14 13
#endif

#if defined(VALUE_LINE)
#define TIM15 14
#define TIM16 15
#define TIM17 16
#endif


typedef uint32_t TIM;

typedef struct 
{
  uint32_t  TIM_BASE;
  BOOL      isAPB1;
  uint32_t  NVIC_TIMx_IRQ;
  uint32_t  RCC_EN_bit;
  BOOL      isAdvanced;
  uint32_t  tim_no;  //zero based TIM1 TIM2 etc   
} TIM_CFG;

typedef struct
{
    TIM_CFG*  tim_cfg;
    uint16_t  ch_no;
//    uint32_t _dier;  same as TIM
//    uint32_t _sr;    same as TIM
//    uint32_t _egr;   same as TIM
//    uint32_t _ccer;  same as CCER 
    uint32_t _ccmr;  //ccmr1 or ccmr2 
    uint32_t _ccr;    //1..4
    uint32_t _bdtr;  //advanced only or nil
    
    uint16_t _dier_CCxDE;
    uint16_t _dier_CCxIE;

    uint16_t _sr_CCxIF;
    uint16_t _sr_CCxOF;

    uint16_t _egr_CCxG;

    uint16_t _ccmr_CCxS; //input/output selection

    uint16_t _ccmr_OCxFE;//fast
    uint16_t _ccmr_OCxPE;//preload
    uint16_t _ccmr_OCxM;//mode
    uint16_t _ccmr_OCxCE;//clear enable

    uint16_t _ccmr_ICxPSC;//prescaler
    uint16_t _ccmr_ICxF;//filter sampling freq

    uint16_t _ccer_CCxE;//output/capture enable
    uint16_t _ccer_CCxP;//output/input  polarity
    
    
} TIM_CH_CFG;

void TIM_Init (TIM tim_no, TIM_CFG* cfg);
void TIM_EnableCounter (TIM_CFG* cfg, BOOL enable);
void TIM_EnableUpdateEvent (TIM_CFG* cfg, BOOL enable);
void TIM_EnableAutoPreload (TIM_CFG* cfg, BOOL enable);
void TIM_EnableUpdateInterrupt (TIM_CFG* cfg, BOOL enable);
void TIM_EnableNVICInterrupt (TIM_CFG* cfg, BOOL enable);
void TIM_EnableTriggerInterrupt (TIM_CFG* cfg, BOOL enable);
void TIM_SetCounterValue (TIM_CFG* cfg, uint16_t val);
void TIM_SetPrescalerValue (TIM_CFG* cfg, uint16_t val);
void TIM_SetAutoReloadValue (TIM_CFG* cfg, uint16_t val);
uint32_t TIM_GetCounterValue (TIM_CFG* cfg);
uint32_t TIM_GetPrescalerValue (TIM_CFG* cfg);
uint32_t TIM_GetAutoReloadValue (TIM_CFG* cfg);
void TIM_CalculateTimerValues (TIM_CFG* cfg, uint32_t us, uint16_t* cnt, uint16_t* presc);
void TIM_SetUpdatePeriod_us (TIM_CFG* cfg, uint32_t us);
BOOL TIM_IsUIF (TIM_CFG* cfg);
void TIM_ClearUIF (TIM_CFG* cfg);
void TIM_GenerateRegisterUpdate (TIM_CFG* cfg);
uint32_t TIM_GetTimerClockFreq (TIM_CFG* cfg);

void TIM_SetupPWM1(TIM_CFG* cfg, uint8_t ch, uint32_t period_us, uint32_t duty_us);
void TIM_SetupPWM2(TIM_CFG* cfg, uint8_t ch, uint32_t period_us, uint32_t duty_us);
void TIM_EnableChannel(TIM_CFG* cfg, uint8_t ch, BOOL enable);
void TIM_UpdatePWMDuty(TIM_CFG* cfg, uint8_t ch, uint32_t duty_us);
void TIM_SetHandler (TIM_CFG* cfg, void (*_irq_handler)(void));
void TIM_EnableCCInterrupt (TIM_CFG* cfg, uint8_t ch, BOOL enable);
void TIM_SetModeInputDirect(TIM_CFG* cfg, BOOL enable);
void TIM_SetICCFallingEdge(TIM_CFG* cfg, uint8_t ch);
void TIM_SetICCRisingEdge(TIM_CFG* cfg, uint8_t ch);
uint32_t TIM_GetCCR(TIM_CFG* cfg, uint8_t ch);

#endif

#endif
