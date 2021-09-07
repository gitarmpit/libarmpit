#include "sim.h"

extern "C" void HardFault_Handler()
{
    while(1)
        ;
}

extern "C" void SVC_Handler()
{
    while(1)
        ;

}

extern "C" void test_i2c();
extern "C" void test_pit();
#ifdef SEMIHOSTING
    extern "C" void initialise_monitor_handles();
#endif

int main(void)
{
#ifdef SEMIHOSTING
    initialise_monitor_handles();
#endif

    SIM_DisableWatchDog();

    test_i2c();

    while(1)
        ;
}
