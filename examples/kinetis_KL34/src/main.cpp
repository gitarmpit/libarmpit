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

extern "C"
{
	void test_i2c();
	void test_pit();
	void test_adc();
}

#ifdef SEMIHOSTING
    extern "C" void initialise_monitor_handles();
#endif

void testSSD1306();
void test_nokia5110();
void test_tpm_cpp_pwm();
void test_rotary_handler();


int main(void)
{
#ifdef SEMIHOSTING
    initialise_monitor_handles();
#endif

    SIM_DisableWatchDog();


    //test_i2c();
    testSSD1306();
    //test_nokia5110();
    //test_tpm_cpp_pwm();
    //test_nokia5110();
    //test_rotary_handler();
    //test_adc();

    while(1)
        ;
}
