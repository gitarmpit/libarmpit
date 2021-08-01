#ifndef RCC_HELPER_H
#define RCC_HELPER_H

#include "flash.h"
#include "rcc.h"

#ifdef __cplusplus
extern "C"
{
#endif

void RCC_EnableHSI_SetMhz(uint8_t mhz);
void RCC_EnableHSE_168Mhz(void);
void RCC_EnableHSI_168Mhz(void);
void RCC_EnableHSI_100Mhz(void);
void RCC_EnableHSI_84Mhz(void);
void RCC_EnableHSI_56Mhz(void);
void RCC_EnableHSI_72Mhz(void);
void RCC_EnableHSI_32Mhz(void);
void RCC_EnableHSI_64Mhz(void);
void RCC_EnableHSI_128Mhz(void);

#ifdef __cplusplus
}
#endif



#endif
