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
	void testSSD1306_c();
}

#ifdef SEMIHOSTING
    extern "C" void initialise_monitor_handles();
#include <stdio.h>
#endif

void testSSD1306();
void test_nokia5110();
void test_tpm_cpp_pwm();
void test_rotary_handler();
void pwm_gen();
void LCD_9225_test();
void LCD_932x_test();
void LCD_HX8357_test();

int main(void)
{
#ifdef SEMIHOSTING
    initialise_monitor_handles();
#endif

    SIM_DisableWatchDog();


    //test_i2c();
    //testSSD1306();
    //testSSD1306_c();
    //test_nokia5110();
    //test_tpm_cpp_pwm();
    //test_nokia5110();
    //test_rotary_handler();
    //test_adc();
    //pwm_gen();

    // LCD_9225_test();
    //LCD_932x_test();
    LCD_HX8357_test();

    while(1)
        ;
}
