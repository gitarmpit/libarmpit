#ifndef __FSMC_SRAM_H
#define __FSMC_SRAM_H

#include "stm32f10x.h"

void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);

#endif /* __FSMC_SRAM_H */

