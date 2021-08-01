#ifndef __CMSIS_COMPILER_H
#define __CMSIS_COMPILER_H 

// included from core_cm4.h, includes arm or gnu gcc etc.

#include <stdint.h>
#if defined ( __CC_ARM )
  #include "cmsis_armcc.h"
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #include "cmsis_armclang.h"
#elif defined ( __GNUC__ )
  #include "cmsis_gcc.h"
#else
  #error Unknown compiler.
#endif


#endif
