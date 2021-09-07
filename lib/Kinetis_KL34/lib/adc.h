#ifndef _ADC_H
#define _ADC_H 
#include "common.h"
#include "nvic.h"

#ifndef ADC0_BASE
#define ADC0_BASE     0x4003B000 
#endif

// Status and control registers A B 2 3
#define ADC0_SC1A  (*(volatile uint32_t*)(ADC0_BASE + 0x0))
#define ADC0_SC1B  (*(volatile uint32_t*)(ADC0_BASE + 0x4))
#define ADC0_SC2   (*(volatile uint32_t*)(ADC0_BASE + 0x20))
#define ADC0_SC3   (*(volatile uint32_t*)(ADC0_BASE + 0x24))
// Configuration registers 1 & 2
#define ADC0_CFG1  (*(volatile uint32_t*)(ADC0_BASE + 0x8))
#define ADC0_CFG2  (*(volatile uint32_t*)(ADC0_BASE + 0xC))
// Result registers A & B
#define ADC0_RA    (*(volatile uint32_t*)(ADC0_BASE + 0x10))
#define ADC0_RB    (*(volatile uint32_t*)(ADC0_BASE + 0x14))
// Compare Value Registers
#define ADC0_CV1   (*(volatile uint32_t*)(ADC0_BASE + 0x18))
#define ADC0_CV2   (*(volatile uint32_t*)(ADC0_BASE + 0x1C))
// Offset Correction
#define ADC0_OFS   (*(volatile uint32_t*)(ADC0_BASE + 0x28))
// Plus/minus side gain registers
#define ADC0_PG  (*(volatile uint32_t*)(ADC0_BASE + 0x2C))
#define ADC0_MG  (*(volatile uint32_t*)(ADC0_BASE + 0x30))

// Calibration registers
#define ADC0_CLPD  (*(volatile uint32_t*)(ADC0_BASE + 0x34))
#define ADC0_CLPS  (*(volatile uint32_t*)(ADC0_BASE + 0x38))
#define ADC0_CLP4  (*(volatile uint32_t*)(ADC0_BASE + 0x3C))
#define ADC0_CLP3  (*(volatile uint32_t*)(ADC0_BASE + 0x40))
#define ADC0_CLP2  (*(volatile uint32_t*)(ADC0_BASE + 0x44))
#define ADC0_CLP1  (*(volatile uint32_t*)(ADC0_BASE + 0x48))
#define ADC0_CLP0  (*(volatile uint32_t*)(ADC0_BASE + 0x4C))
#define ADC0_CLMD  (*(volatile uint32_t*)(ADC0_BASE + 0x54))
#define ADC0_CLMS  (*(volatile uint32_t*)(ADC0_BASE + 0x58))
#define ADC0_CLM4  (*(volatile uint32_t*)(ADC0_BASE + 0x5C))
#define ADC0_CLM3  (*(volatile uint32_t*)(ADC0_BASE + 0x60))
#define ADC0_CLM2  (*(volatile uint32_t*)(ADC0_BASE + 0x64))
#define ADC0_CLM1  (*(volatile uint32_t*)(ADC0_BASE + 0x68))
#define ADC0_CLM0  (*(volatile uint32_t*)(ADC0_BASE + 0x6C))

// SC1: Status and control
#define ADC_SC1_COCO           (1<<7) // Conversion complete
#define ADC_SC1_AIEN           (1<<6) // Interrupt Enable
#define ADC_SC1_DIFF           (1<<5) // Diff mode Enable
#define ADC_SC1_ADCH           0           // Channel select: 0 - 23
#define ADC_SC1_ADCH_TEMP      26
#define ADC_SC1_ADCH_BANDGAP   27 // Firstenable the bandgap buffer by setting the PMC_REGSC[BGBE] bit.
#define ADC_SC1_ADCH_VREFSH    29
#define ADC_SC1_ADCH_VREFSL    30
#define ADC_SC1_ADCH_DISABLE   31
#define ADC_SC1_ADCH_CLEARMASK 31

// CFG1: mode of operation, clock source, clock divide, and configuration for low power or long sample time.
#define ADC_CFG1_ADLPC  (1<<7) // Low-Power mode on (at the expense of maximum clock speed)

// Clock division
#define ADC_CFG1_ADIV_DIV1  (0<<5)
#define ADC_CFG1_ADIV_DIV2  (1<<5)
#define ADC_CFG1_ADIV_DIV4  (2<<5)
#define ADC_CFG1_ADIV_DIV8  (3<<5)
#define ADC_CFG1_ADIV_CLEARMASK  (3<<5)

#define ADC_CFG1_ADLSMP  (1<<4) // 0: short, 1: long conversion time (ADLSTS value)

// Mode: bits
#define ADC_CFG1_MODE_8BIT       (0<<2)
#define ADC_CFG1_MODE_12BIT      (1<<2)
#define ADC_CFG1_MODE_10BIT      (2<<2)
#define ADC_CFG1_MODE_CLEARMASK  (3<<2)

// Select clock source
#define ADC_CFG1_ADICLK_BUS       (1<<0)
#define ADC_CFG1_ADICLK_BUS_DIV2  (1<<0)
#define ADC_CFG1_ADICLK_ALTCLK    (1<<0)  // Alternate Clock (OSCERCLK) Mhz only, OSC32 is not supported
#define ADC_CFG1_ADICLK_ADACK     (1<<0)  // Async Clock, freq  1 - 10 Mhz see datasheet 
#define ADC_CFG1_ADICLK_CLEARMASK (3<<0)

// CFG2: special high-speed configuration for very high speed conversions 
// and selects the long sample time duration during long sample mode.
#define ADC_CFG2_MUXSEL   (1<<4) // 0: ADxxa 1: ADxxb channels are selected
#define ADC_CFG2_ADACKEN  (1<<3) // Asynchronous Clock Output Enable
#define ADC_CFG2_ADHSC    (1<<2) // 0: Normal conversion: 1: high speed conversion.  Max Freq 18Mhz

// Long conversion: extra cycles
#define ADC_CFG2_ADLSTS_20        0
#define ADC_CFG2_ADLSTS_12        1
#define ADC_CFG2_ADLSTS_6         2
#define ADC_CFG2_ADLSTS_2         3
#define ADC_CFG2_ADLSTS_CLEARMASK 3   

// SC2: conversion active, hardware/software trigger select, compare function, and voltage reference select 
#define ADC_SC2_ADACT  (1<<7)  // Conversion active flag. Cleared when done.
#define ADC_SC2_ADTRG  (1<<6)  // Trigger. 0: software 1: hardware 
#define ADC_SC2_ACFE   (1<<5)  // Compare function enable
#define ADC_SC2_ACFGT  (1<<4)  // Compare Function Greater Than Enable
#define ADC_SC2_ACREN  (1<<3)  // Compare Function Range Enable
#define ADC_SC2_DMAEN  (1<<2)  // DMA Enable
#define ADC_SC2_REFSEL_DEF 0   
#define ADC_SC2_REFSEL_ALT 1

// SC3:  calibration, continuous convert, and hardware averaging functions 
#define ADC_SC3_CAL  (1<<7) // Calibration Enable 
#define ADC_SC3_CALF (1<<6) // Calibration Failed flag
#define ADC_SC3_ADCO (1<<3) // Continuous Conversion Enable
#define ADC_SC3_AVGE (1<<2) // Hardware Average Enable
// Average select number of samples
#define ADC_SC3_AVGS_4   0
#define ADC_SC3_AVGS_8   1
#define ADC_SC3_AVGS_16  2
#define ADC_SC3_AVGS_32  3
#define ADC_SC3_AVGS_CLEARMASK 3

typedef enum
{
    ADC0 = 0,
} ADC_N;

typedef enum
{
    ADC_CH_SE0  = 0,  // E20
    ADC_CH_SE1  = 1,  // E16
    ADC_CH_SE2  = 2,  // E18
    ADC_CH_SE3  = 3,  // E22
    ADC_CH_SE4A = 4,  // E21
    ADC_CH_SE4B = -4,  // E29
    ADC_CH_SE5A = 5,  // E17 
    ADC_CH_SE5B = -5,  // D1
    ADC_CH_SE6A = 6,  // E19
    ADC_CH_SE6B = -6,  // D5
    ADC_CH_SE7A = 7,  // E23
    ADC_CH_SE7B = -7,  // D6
    ADC_CH_SE8  = 8,  // B0
    ADC_CH_SE9  = 9,  // B1
    ADC_CH_SE11 = 11, // C2
    ADC_CH_SE12 = 12, // B2
    ADC_CH_SE13 = 13, // B3
    ADC_CH_SE14 = 14, // C0
    ADC_CH_SE15 = 15, // C1
    ADC_CH_SE23 = 23, // E30
    ADC_CH_TEMP = ADC_SC1_ADCH_TEMP,
    ADC_CH_VREFSH  = ADC_SC1_ADCH_VREFSH,
    ADC_CH_VREFSL  = ADC_SC1_ADCH_VREFSL,
    ADC_CH_BANDGAP = ADC_SC1_ADCH_BANDGAP

} ADC_CH_N;

typedef struct _ADC
{
    volatile uint32_t* SIM_enableReg;
    uint32_t           SIM_enableMask;

    volatile uint32_t* ADC_SC1A;
    volatile uint32_t* ADC_SC1B;
    volatile uint32_t* ADC_CFG1;
    volatile uint32_t* ADC_CFG2;
    volatile uint32_t* ADC_RA;
    volatile uint32_t* ADC_RB;
    volatile uint32_t* ADC_SC2;
    volatile uint32_t* ADC_SC3;

    IRQn_Type          irq;
    void               (*handler)(struct _ADC*);

} ADC;


typedef struct _ADC_Channel
{
    volatile uint32_t* ADC_SC1;
    volatile uint32_t* ADC_R;
    uint8_t            ch_no;
    BOOL               is_A;
    ADC*               adc;

} ADC_Channel;


#ifdef __cplusplus
extern "C"
{
#endif


ADC* ADC_GetInstance(ADC_N n);

void ADC_EnableClock(ADC* adc, BOOL enable);
ADC_Channel ADC_GetChannel(ADC* adc, ADC_CH_N);
BOOL ADC_IsConversionComplete (ADC_Channel* ch);
void ADC_EnableInterrupt(ADC_Channel* ch, BOOL enable);
void ADC_SetInterruptHandler(ADC* adc, void(*handler)(ADC*));
void ADC_SetChannel(ADC_Channel* ch);

void ADC_SetClockSource_Bus(ADC* adc);
void ADC_SetClockSource_BusDiv2(ADC* adc);
void ADC_SetClockSource_Alt(ADC* adc);
void ADC_SetClockSource_Async(ADC* adc);

void ADC_SetClock_Div1(ADC* adc);
void ADC_SetClock_Div2(ADC* adc);
void ADC_SetClock_Div4(ADC* adc);
void ADC_SetClock_Div8(ADC* adc);

void ADC_SetMode_8bit(ADC* adc);
void ADC_SetMode_10bit(ADC* adc);
void ADC_SetMode_12bit(ADC* adc);

void ADC_SetHighSpeedConversion(ADC* adc, BOOL highSpeed);
void ADC_SetLongConversion_20(ADC* adc);
void ADC_SetLongConversion_12(ADC* adc);
void ADC_SetLongConversion_6(ADC* adc);
void ADC_SetLongConversion_2(ADC* adc);
void ADC_SetShortConversion(ADC* adc);

void ADC_EnableHardwareTrigger(ADC* adc, BOOL hardware);
void ADC_EnableDMA(ADC* adc, BOOL enable);

void ADC_SetHardwareAveraging_0(ADC* adc);
void ADC_SetHardwareAveraging_4(ADC* adc);
void ADC_SetHardwareAveraging_8(ADC* adc);
void ADC_SetHardwareAveraging_16(ADC* adc);
void ADC_SetHardwareAveraging_32(ADC* adc);

BOOL ADC_EnableCalibration(ADC* adc);
void ADC_EnableContinuousConversion(ADC* adc, BOOL enable);
uint16_t ADC_SingleConversion(ADC_Channel* ch);

#ifdef __cplusplus
}
#endif


#endif
