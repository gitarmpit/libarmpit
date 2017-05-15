#ifndef RCC_HELPER_H
#define RCC_HELPER_H

#include "flash.h"
#include "rcc.h"

#ifdef __cplusplus
extern "C"
{
#endif

void RCC_EnableHSI_8Mhz(void);
void RCC_EnableHSI_16Mhz(void);
void RCC_EnableHSI_24Mhz(void);
void RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz(void);
void RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz(void);

#ifdef __cplusplus
}
#endif



#endif
