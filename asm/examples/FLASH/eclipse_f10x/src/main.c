#if 0
#include "rcc.h"
#include "flash.h"

int main()
{
    volatile uint32_t sysclk = RCC_GetSystemClock();

    FLASH_SetWaitState(2);
    uint8_t ws = FLASH_GetWaitState();


    RCC_EnablePLL(16);


    RCC_EnablePWR(TRUE);
    RCC_EnablePWR(FALSE);

    FLASH_Unlock();
    uint8_t page = 63;
    FLASH_ErasePage(page);
    //FLASH_Write (0x800FC00, 0x1234);
    FLASH_Write (FLASH_BASE + page*1024, 0xdead);
    FLASH_ErasePage(page);


    while(1)
        ;
}

#endif
