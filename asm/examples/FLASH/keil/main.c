
#include <stdint.h>
#include "common.h"
#include "nvic.h"
#include "flash.h"
#include "rcc.h"



int main()
{
    FLASH_SetWaitState(2);
    RCC_EnablePLL(16);
    
    //0-15
    NVIC_SetPriority(IRQn_USBWakeUp, 15);
    volatile uint8_t p = NVIC_GetPriority(IRQn_USBWakeUp);

    volatile BOOL rc = FLASH_IsLocked();
    FLASH_Unlock();
    rc = FLASH_IsLocked();
    uint8_t page = 63;
    FLASH_ErasePage(page);
    //FLASH_Write (0x800FC00, 0x1234);
    FLASH_Write (FLASH_BASE + page*1024, 0xdead);
    FLASH_ErasePage(page);

    
    while(1)
        ;


}
