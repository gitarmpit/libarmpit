#include "_rcc.h"

const uint32_t _d = 0xdeadbeef;

//uint32_t _d2;

void test()
{
    RCC_EnablePeripheral(TRUE,
            &RCC_APB1ENR,
            RCC_APB1ENR_PWREN,
            &RCC_APB1RSTR,
            RCC_APB1RSTR_PWRRST);
}


void test64()
{
    uint64_t arg1 = 0x2324252656575859;
    uint64_t arg2 = 0xbee1bee2dea1dea2;
    _test64(arg1, arg2);
}
