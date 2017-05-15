#include "flash.h"

void flash_test()
{
    FLASH_Unlock();

    //407VG - 1M flash
    //4 sectors of 16K, 1 sec of 64K, 7 sectors of 128K

    uint32_t flash_size = 0x100000;
    uint32_t sector11_offset = 0x080e0000;
    uint32_t my_addr = (FLASH_BASE + flash_size - 32);
    uint32_t my_data = *((uint32_t *)my_addr);

    volatile int status = 0;

    status = FLASH_EraseSector(11);

    FLASH_SetPsize_x32();

    my_data = 0xdeadbeef;

    status = FLASH_WriteWord(my_addr, my_data);
//    status = FLASH_WriteWord(my_addr, 0x11111111);
//    status = FLASH_WriteWord(my_addr, 0x11111111);
//    status = FLASH_WriteWord(my_addr, 0x11111111);
//    status = FLASH_WriteWord(my_addr, 0x11111111);
//    status = FLASH_WriteWord(my_addr, 0x11111111);
//    status = FLASH_WriteWord(my_addr, 0x11111111);
//    status = FLASH_WriteWord(my_addr, 0x22222222);

    FLASH_Lock();

    while(1)
        ;
}
