#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <stddef.h>
#include "stm32.h"
#include "kinetis.h"
#include "efm32.h"

#if !defined(Kinetis) && !defined(STM32) && !defined(EFM32)
 #error "Target device not defined in the project settings"
#endif

#include "irq.h" 

#define UNUSED(arg)  ((void)arg)

#define PERIPH_BASE  0x40000000


// Automaic dependencies:


#if defined(USE_RTC) && !defined(USE_BKP)
#define USE_BKP
#ifndef USE_EXTI
#define USE_EXTI
#endif
#endif

#if defined(USE_BKP) && !defined(USE_PWR)
#define USE_PWR
#endif

#if defined(USE_SPI) && !defined(USE_DMA)
#define USE_DMA
#endif

#if defined(USE_USART) && !defined(USE_DMA)
#define USE_DMA
#endif

#if defined(USE_I2C) && !defined(USE_DMA)
#define USE_DMA
#endif

typedef int BOOL;

#define TRUE 1 
#define FALSE 0

#if defined(__GNUC__) && defined (__cplusplus)
#include <bits/c++config.h>
#include <malloc.h>
//inline void* operator new (std::size_t size) {return ::malloc(size);}
#endif


#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif

