#ifndef _LLWU_H
#define _LLWU_H
#include "common.h"

// Low-Leakage Wake-up Unit

#ifndef LLWU_BASE
#define LLWU_BASE     0x4007C000 
#endif

// Pin Enable
#define LLWU_PE1    (*(volatile uint8_t*)(LLWU_BASE + 0x0))
#define LLWU_PE2    (*(volatile uint8_t*)(LLWU_BASE + 0x1))
#define LLWU_PE3    (*(volatile uint8_t*)(LLWU_BASE + 0x2))
#define LLWU_PE4    (*(volatile uint8_t*)(LLWU_BASE + 0x3))
#define LLWU_ME     (*(volatile uint8_t*)(LLWU_BASE + 0x4))
#define LLWU_F1     (*(volatile uint8_t*)(LLWU_BASE + 0x5))
#define LLWU_F2     (*(volatile uint8_t*)(LLWU_BASE + 0x6))
#define LLWU_F3     (*(volatile uint8_t*)(LLWU_BASE + 0x7))
#define LLWU_FILT1  (*(volatile uint8_t*)(LLWU_BASE + 0x8))
#define LLWU_FILT2  (*(volatile uint8_t*)(LLWU_BASE + 0x9))


#define WUPEx_DISABLED  0
#define WUPEx_RISING    1
#define WUPEx_FALLING   2
#define WUPEx_ANY       3
#define WUPEx_CLEARMASK 3

//PE1: 0-3    (2bits per pin)
//PE2: 4-7    (2bits per pin)
//PE3: 8-11   (2bits per pin)
//PE4: 12-15  (2bits per pin)

// Internal Wakep Module Enable
//ME:  0-7 

// Flags:
//F1: Pins 0-7 
//F2: Pins 8-15
//F3: Internal 0-7

/* WakeUp Sources on KL34
LLWU_P5   PTB0
LLWU_P6   PTC1
LLWU_P7   PTC3
LLWU_P8   PTC4
LLWU_P9   PTC5
LLWU_P10  PTC6
LLWU_P14  PTD4
LLWU_P15  PTD6
LLWU_M0IF LPTMR0
LLWU_M1IF CMP0
LLWU_M2IF Reserved
LLWU_M3IF Reserved
LLWU_M4IF TSI0
LLWU_M5IF RTC Alarm
LLWU_M6IF Reserved
LLWU_M7IF RTC Seconds
*/

typedef enum 
{
    LLWU_P0  = 0,
    LLWU_P1  = 1,
    LLWU_P2  = 2,
    LLWU_P3  = 3,
    LLWU_P4  = 4,
    LLWU_P5  = 5,  // PTB0
    LLWU_P6  = 6,  // PTC1
    LLWU_P7  = 7,  // PTC3
    LLWU_P8  = 8,  // PTC4
    LLWU_P9  = 9,  // PTC5
    LLWU_P10 = 10, // PTC6
    LLWU_P11 = 11,
    LLWU_P12 = 12,
    LLWU_P13 = 13,
    LLWU_P14 = 14, // PTD4
    LLWU_P15 = 15  // PTD6

} LLWU_PIN;

typedef enum 
{
    LLWU_LPTMR0    = 0, // LPTMR0
    LLWU_CMP0      = 1, // CMP0
    LLWU_M2IF      = 2,
    LLWU_M3IF      = 3,
    LLWU_TSI0      = 4, // Touch Sense
    LLWU_RTC_ALARM = 5, // RTC Alarm
    LLWU_M6IF      = 6,
    LLWU_RTC_SEC   = 7  // RTC Seconds
} LLWU_MOD;

#ifdef __cplusplus
 extern "C" {
#endif

void LLWU_SetWakeupPin_Rising     (LLWU_PIN in);
void LLWU_SetWakeupPin_Falling    (LLWU_PIN in);
void LLWU_SetWakeupPin_Any        (LLWU_PIN in);
void LLWU_SetWakeupModule         (LLWU_MOD in);

#ifdef __cplusplus
}
#endif


#endif
