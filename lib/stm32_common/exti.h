#ifndef _EXTI_H
#define _EXTI_H

#include "common.h"


typedef enum
{
    EXTI_LINE0 = 0,
    EXTI_LINE1 = 1,
    EXTI_LINE2 = 2,
    EXTI_LINE3 = 3,
    EXTI_LINE4 = 4,
    EXTI_LINE5 = 5,
    EXTI_LINE6 = 6,
    EXTI_LINE7 = 7,
    EXTI_LINE8 = 8,
    EXTI_LINE9 = 9,
    EXTI_LINE10 = 10,
    EXTI_LINE11 = 11,
    EXTI_LINE12 = 12,
    EXTI_LINE13 = 13,
    EXTI_LINE14 = 14,
    EXTI_LINE15 = 15,
    EXTI_LINE16 = 16,
    EXTI_LINE17 = 17,
    EXTI_LINE18 = 18,
    EXTI_LINE19 = 19,
#if defined (STM32F2) || defined (STM32F4) || defined (STM32L0)
    EXTI_LINE20 = 20,
    EXTI_LINE21 = 21,
    EXTI_LINE22 = 22,
#if defined (STM32L0)
    EXTI_LINE23 = 23,
    EXTI_LINE24 = 24,
    EXTI_LINE25 = 25,
    EXTI_LINE26 = 26,
    EXTI_LINE28 = 28,
    EXTI_LINE29 = 29,

#endif

#endif

} EXTI_LINE;




#include "nvic.h"

#if defined (STM32F1) || defined (STM32L0)
#define EXTI_BASE 0x40010400  //APB2
#elif defined (STM32F2) || defined (STM32F4)
#define EXTI_BASE 0x40013C00  //APB2
#else 
#error "STM32 target device not defined"
#endif


#define EXTI_IMR   (*(volatile uint32_t*)(EXTI_BASE))         //interrupt mask
#define EXTI_EMR   (*(volatile uint32_t*)(EXTI_BASE + 0x4))   //event mask
#define EXTI_RTSR  (*(volatile uint32_t*)(EXTI_BASE + 0x8))   //rising  edge
#define EXTI_FTSR  (*(volatile uint32_t*)(EXTI_BASE + 0xc))   //falling edge
#define EXTI_SWIER (*(volatile uint32_t*)(EXTI_BASE + 0x10))  //software int event
#define EXTI_PR    (*(volatile uint32_t*)(EXTI_BASE + 0x14))  //pending


//void EnableEXTI(bool enable, EXTI_LINE line);


#endif
