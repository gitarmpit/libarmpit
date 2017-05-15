#ifndef _FLASH_H
#define _FLASH_H

#include "../common.h"

#define FLASH_BASE      0x08000000

#if defined(__GNUC__) && defined(__ASSEMBLY__)


#define FLASH_REG_BASE 0x40022000
#define FLASH_ACR      FLASH_REG_BASE + 0x00   
#define FLASH_KEYR     FLASH_REG_BASE + 0x04  
#define FLASH_OPTKEYR  FLASH_REG_BASE + 0x08   
#define FLASH_SR       FLASH_REG_BASE + 0x0c   
#define FLASH_CR       FLASH_REG_BASE + 0x10   
#define FLASH_AR       FLASH_REG_BASE + 0x14   
#define FLASH_OBR      FLASH_REG_BASE + 0x1c   
#define FLASH_WRPR     FLASH_REG_BASE + 0x20   


#define FLASH_ACR_LATENCY_OFFSET  0
#define FLASH_ACR_LATENCY_MASK  (0x7 << FLASH_ACR_LATENCY_OFFSET)
#define FLASH_ACR_HLFCYA  (1<<3)
#define FLASH_ACR_PRFTEN  (1<<4)
#define FLASH_ACR_PRFTBS  (1<<5)




#define FLASH_KEY1  0x45670123
#define FLASH_KEY2  0xCDEF89AB


#define FLASH_OPTKEY1   0x08192A3B
#define FLASH_OPTKEY2   0x4C5D6E7F


#define FLASH_SR_BSY     (1<<0)
#define FLASH_SR_PGPERR  (1<<2)
#define FLASH_SR_WRPERR  (1<<4)
#define FLASH_SR_EOP     (1<<5)



#define FLASH_CR_PG     (1<<0)
#define FLASH_CR_PER    (1<<1)
#define FLASH_CR_MER    (1<<2)
#define FLASH_CR_OPTPG  (1<<4)
#define FLASH_CR_OPTER  (1<<5)
#define FLASH_CR_STRT   (1<<6)
#define FLASH_CR_LOCK   (1<<7)
#define FLASH_CR_OPTWRE  (1<<9)
#define FLASH_CR_ERRIE  (1<<10)
#define FLASH_CR_EOPIE  (1<<12)



#else 



#ifdef __cplusplus
extern "C"
{
#endif


void FLASH_SetWaitState(uint8_t wait_state);
uint8_t FLASH_GetWaitState(void);
void FLASH_EnablePrefetchBuffer(void);

void FLASH_Unlock(void);
void FLASH_Lock(void);
BOOL FLASH_IsLocked(void);
BOOL FLASH_IsBusy(void);  

int FLASH_Write (uint32_t addr, uint16_t data);
int FLASH_MassErase(void);
int FLASH_ErasePage(uint8_t page);


#ifdef __cplusplus
}
#endif

#endif

#endif
