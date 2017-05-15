#include "flash.h"

#ifdef __cplusplus
extern "C"
{
#endif


void FLASH_SetWaitState(uint8_t wait_state)
{
    FLASH_ACR &= ~FLASH_ACR_LATENCY_MASK;
    FLASH_ACR |= wait_state<<FLASH_ACR_LATENCY_OFFSET;
}

uint8_t FLASH_GetWaitState()
{
    return (0x7 & (FLASH_ACR >>FLASH_ACR_LATENCY_OFFSET));
}

void FLASH_EnablePrefetchBuffer()
{
    FLASH_ACR |= FLASH_ACR_PRFTEN;
}

void FLASH_Unlock(void) 
{
    FLASH_KEYR = FLASH_KEY1;   
    FLASH_KEYR = FLASH_KEY2;   
//    while (FLASH_IsLocked())
//        ; 
}

void FLASH_Lock(void) 
{
    FLASH_CR |= FLASH_CR_LOCK;    
}

BOOL FLASH_IsLocked(void) 
{
    return (FLASH_CR & FLASH_CR_LOCK);    
}


BOOL FLASH_IsBusy() 
{
    return (FLASH_SR & FLASH_SR_BSY);    
}

static uint32_t GetStatus(void) 
{
    if (FLASH_SR & FLASH_SR_WRPERR) 
    {
        return FLASH_SR_WRPERR;
    }
    else if (FLASH_SR & FLASH_SR_PGPERR) 
    {
        return FLASH_SR_PGPERR;
    }
    else 
    {
        return 0;
    }
}

int FLASH_ErasePage(uint8_t page) 
{
    while (FLASH_IsBusy())
        ;


    FLASH_CR |= FLASH_CR_PER;
    
    
    FLASH_AR = FLASH_BASE + page * 1024;

 //   FLASH_CR &= FLASH_CR_SNB_MASK;
 //   FLASH_CR |= (sector<<FLASH_CR_SNB_OFFSET & FLASH_CR_SNB_MASK);

    FLASH_CR |= FLASH_CR_STRT;

    while (FLASH_IsBusy())
        ;

    return GetStatus();
         
}

int FLASH_MassErase(void) 
{
    while (FLASH_IsBusy())
        ;

    FLASH_CR |= FLASH_CR_MER;

    FLASH_CR |= FLASH_CR_STRT;

    while (FLASH_IsBusy())
        ;

    //shouldn't even get this far on success
    return GetStatus();

}


int FLASH_Write (uint32_t addr, uint16_t data) 
{
    while (FLASH_IsBusy())
        ;

    FLASH_CR |= FLASH_CR_PG;

    *(volatile uint16_t *)addr = data;

    while (FLASH_IsBusy())
        ;

    
    FLASH_CR &= ~FLASH_CR_PG;
    
    return GetStatus();
}



#ifdef __cplusplus
}
#endif
