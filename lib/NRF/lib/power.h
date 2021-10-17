#ifndef _POWER_H 
#define _POWER_H

#include "common.h"

#ifndef POWER_BASE
#define POWER_BASE     0x40000000 
#endif

#define POWER_REG1   (*(volatile uint32_t*)(POWER_BASE + 0x0))


#define POWER_REG_MASK   (1<<7) // Low-Voltage Detect Flag

#ifdef __cplusplus
 extern "C" {
#endif



#ifdef __cplusplus
}
#endif


#endif
