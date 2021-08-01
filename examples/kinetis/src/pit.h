#ifndef _PIT_H
#define _PIT_H 
#include "common.h"

#define PIT_BASE 0x40037000  

#define PIT_MCR          (*(volatile uint32_t*)(PIT_BASE + 0x0))
#define PIT_PIT_LTMR64H  (*(volatile uint32_t*)(PIT_BASE + 0xe0))
#define PIT_PIT_LTMR64L  (*(volatile uint32_t*)(PIT_BASE + 0xe4))
#define PIT_LVAL_BASE    PIT_BASE + 0x100
#define PIT_CVAL_BASE    PIT_BASE + 0x104
#define PIT_TCTRL_BASE   PIT_BASE + 0x108
#define PIT_TFLG_BASE    PIT_BASE + 0x10C


#define PIT_MCR_MDIS    (1<<1)
#define PIT_MCR_FRZ     (1<<0)

#define PIT_TCTRL_CHN   (1<<2)
#define PIT_TCTRL_TIE   (1<<1)
#define PIT_TCTRL_TEN   (1<<0)

#define PIT_TFLG_TIF    (1<<0)


typedef enum
{
    PIT0 = 0,
    PIT1 = 1,
    PIT2 = 2,
    PIT3 = 3,
    PIT4 = 4,
} PIT_N;

typedef struct _PIT
{
    volatile uint32_t* PIT_CVAL;
    volatile uint32_t* PIT_LVAL;
    volatile uint32_t* PIT_TCTRL;
    volatile uint32_t* PIT_TFLG;
    BOOL enabled;
    void (*interrupt_handler)(struct _PIT* pit);
} PIT;

void     PIT_EnableClock(BOOL enable);
void     PIT_EnableFreeze(BOOL enable);
PIT*     PIT_GetInstance(PIT_N pitN);
void     PIT_Chain(PIT* pit);
void     PIT_SetPeriod_us(PIT* pit, uint32_t val);
void     PIT_SetLoadValue(PIT* pit, uint32_t val);
uint32_t PIT_GetCurrentValue(PIT* pit);
void     PIT_SetInterruptHandler (PIT* pit, void (*interrupt_handler)(struct _PIT* pit));
void     PIT_EnableInterrupt(PIT* pit, BOOL enable);
void     PIT_EnableTimer(PIT* pit, BOOL enable);
BOOL     PIT_IsInterrupt(PIT* pit);
void     PIT_ClearInterrupt(PIT* pit);


#endif
