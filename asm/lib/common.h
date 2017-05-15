#ifndef _COMMON_H
#define _COMMON_H

#include "stm32.h"
#include "kinetis.h"
#include "efm32.h"

#if !defined(Kinetis) && !defined(STM32) && !defined(EFM32)
 #error "Target device not defined in the project settings"
#endif


#if !defined(__GNUC__) || !defined(__ASSEMBLY__)

#include <stdint.h>
#include <stddef.h>
typedef int BOOL;



#endif 

#define TRUE 1 
#define FALSE 0


#endif

