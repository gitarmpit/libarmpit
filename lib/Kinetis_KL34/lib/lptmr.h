#ifndef _LPTMR_H
#define _LPTMR_H

#include "common.h"
#include "nvic.h"

#ifndef LPTMR_BASE
#define LPTMR_BASE     0x40040000 
#endif

#define LPTMR_CSR    (*(volatile uint32_t*)(LPTMR_BASE + 0x0))  // Control Status
#define LPTMR_PSR    (*(volatile uint32_t*)(LPTMR_BASE + 0x4))  // Prescale
#define LPTMR_CMR    (*(volatile uint32_t*)(LPTMR_BASE + 0x8))  // Compare Register
#define LPTMR_CNR    (*(volatile uint32_t*)(LPTMR_BASE + 0xC))  // Counter


#define LPTMR_CSR_TCF     (1<<7) // Timer Compare Flag (set when CNR reaches CMR)
#define LPTMR_CSR_TIE     (1<<6) // Interrupt is generated whenever TCF is also set.

// Timer Pin Select. Configures the input source to be used in Pulse Counter mode
#define LPTMR_CSR_TPS_IN0        (0<<4) // CMP0 output
#define LPTMR_CSR_TPS_IN1        (1<<4) // LPTMR_ALT1 pin
#define LPTMR_CSR_TPS_IN2        (2<<4) // LPTMR_ALT2 pin
#define LPTMR_CSR_TPS_IN3        (3<<4) // LPTMR_ALT3 pin
#define LPTMR_CSR_TPS_CLEARMASK  (3<<4)

// Timer Pin Polarity of input source in pulse counter mode.  
#define LPTMR_CSR_TPP     (1<<3) // 0/1 = active high/low, CNR will increment on the raising/falling edge

// Free running counter config.
#define LPTMR_CSR_TFC     (1<<2) // 0: CNR is reset whenever TCF is set, 1: CNR is reset on overflow. 
#define LPTMR_CSR_TMS     (1<<1) // Time Mode select: 0: Time Counter, 1: Pulse Counter
#define LPTMR_CSR_TEN     (1<<0) // Timer Enable

// Clock Source
#define LPTMR_PSR_PCS_MCGIRCLK  0 //  (not available in LLS and VLLS modes)
#define LPTMR_PSR_PCS_LPO       1 //  (not available in VLLS0 mode)
#define LPTMR_PSR_PCS_ERCLK32K  2 //  (not available in VLLS0 mode when using 32 kHz oscillator)
#define LPTMR_PSR_PCS_OSCERCLK  3 // External reference clock (not available in VLLS0 mode)
#define LPTMR_PSR_PCS_CLEARMASK 3
#define LPTMR_PSR_PRESCALE_CLEARMASK (0xf<<3)

typedef enum
{
    LPTMR_DIV2     = 0,
    LPTMR_DIV4     = 1,
    LPTMR_DIV8     = 2,
    LPTMR_DIV16    = 3,
    LPTMR_DIV32    = 4,
    LPTMR_DIV64    = 5,
    LPTMR_DIV128   = 6,
    LPTMR_DIV256   = 7,
    LPTMR_DIV512   = 8,
    LPTMR_DIV1024  = 9,
    LPTMR_DIV2048  = 10,
    LPTMR_DIV4096  = 11,
    LPTMR_DIV8912  = 12,
    LPTMR_DIV16384 = 13,
    LPTMR_DIV32768 = 14,
    LPTMR_DIV65536 = 15,
} LPTMR_PRESC;


#ifdef __cplusplus
 extern "C" {
#endif

BOOL LPTMPR_IsTCF();
void LPTMPR_ClearTCF();
void LPTMPR_EnableInterrupt(BOOL enable);
void LPTMPR_PinSelect(uint8_t pin);
void LPTMPR_EnableClock (BOOL enable);
void LPTMPR_SelectMode_TimeCounter();
void LPTMPR_SelectMode_PulseCounter();
void LPTMPR_EnableCounter(BOOL enable);
void LPTMPR_SetInterruptHandler(void (*handler)());
void LPTMPR_SetCMR(uint16_t cmr);
void LPTMR_SetPrescaler(LPTMR_PRESC presc);
uint16_t LPTMR_GetCounterValue ();
void LPTMR_SelectClock_MCGIRCLK(uint32_t clockFreq);
void LPTMR_SelectClock_LPO();
void LPTMR_SelectClock_ERCLK32K(uint32_t clockFreq);
void LPTMR_SelectClock_OSCERCLK(uint32_t clockFreq);
void LPTMR_SetTimer_ms (uint32_t ms);
void LPTMR_SetTimer_sec (uint32_t sec);
void LPTMR_SetTimer_hours (uint32_t hours);

#ifdef __cplusplus
}
#endif


#endif
