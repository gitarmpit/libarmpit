#include "rcc.h"
#include "flash.h"

static void test_options() 
{
    FLASH_Unlock();
    
    volatile uint32_t opt = FLASH_OPTCR;
    FLASH_OptUnlock();
    opt = FLASH_OPTCR;
		opt &= ~FLASH_OPTCR_nWRP_MASK;
	  opt |= 0xFFF<<16;

    while (FLASH_IsBusy())
        ;

    FLASH_OPTCR = opt;
    FLASH_OPTCR |= FLASH_OPTCR_OPTSTRT;
   // uint32_t wprot  &= ~0xFFF<<16;
    
    while (FLASH_IsBusy())
        ;


    opt = FLASH_OPTCR;

    FLASH_OptLock();
		
    opt = FLASH_OPTCR;
		
    while(1);
    
    
}

static void test_write() 
{
    uint32_t flash_size = 0x100000;

    uint32_t my_addr = (FLASH_BASE + flash_size - 16);
    

    uint32_t my_data = *((uint32_t *)my_addr);
    
    my_data = 0xdeadbeef;
    
    FLASH_Unlock();
    volatile uint32_t status;
    
    FLASH_SetPsize_x32();
    
    for (int i = 0; i < 280000; ++i) 
    {
        status = FLASH_WriteWord (my_addr - i*4, my_data);
        if (status != 0) 
        {
            volatile uint32_t err = status;
        }
    }
    
    while(1)
        ;
    
}


static void test_erase_sector() 
{
    FLASH_Unlock();
    FLASH_SetPsize_x32();
    uint8_t status = FLASH_EraseSector(0); 
    
}


static void test_mass_erase() 
{
    FLASH_Unlock();
    FLASH_MassErase();
}


    
int main(void) 
{
    RCC_EnableHSI(true);
    test_options();
    //test_write();
}
