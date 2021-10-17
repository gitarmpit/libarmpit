#ifndef _CLOCK_H 
#define _CLOCK_H

#include "common.h"

#ifndef CLOCK_BASE
#define CLOCK_BASE     0x40000000 
#endif

#define CLOCK_REG1   (*(volatile uint32_t*)(CLOCK_BASE + 0x0))


#define CLOCK_REG_MASK   (1<<7) // Low-Voltage Detect Flag

#ifdef __cplusplus
 extern "C" {
#endif



#ifdef __cplusplus
}
#endif


#endif
