#include <stdint.h>
#include "_rcc.h"
#include "_gpio.h"

int main()
{
    //FLASH_SetWaitState(2);
    RCC_EnableHSI(TRUE);
    RCC_EnablePLL(6);
    uint8_t v1 = 1, v2 = 2, v3 = 3, v4 = 4;
    char* buf = "  ";

    RCC_EnablePWR(TRUE);
    RCC_EnableBKP(TRUE);

    volatile uint32_t rc;

    rc = RCC_GetSystemClock();

    RCC_SetAHBPrescalerDiv4();

    rc = RCC_GetAHB_CLK();

    RCC_SetAPB2PrescalerDiv2();

    rc = RCC_GetAPB2_CLK();

    //RCC_EnableGPIOA(TRUE);
    GPIO_CFG gpio;
    gpio_init (GPIOA, &gpio);

//    *((uint64_t*)gpio.crl) = 0x3333333333333333llu;

    uint8_t s = gpio_is_set (&gpio, GPIO_PIN0);
    s = gpio_is_set (&gpio, GPIO_PIN8);

    gpio_cfg_in_pulldown(&gpio, 0);
    gpio_cfg_in_pullup(&gpio, 1);

    gpio_set_output(&gpio, 0xffff);
    gpio_set_output(&gpio, 0x00);
    gpio_set_pin(&gpio, GPIO_PIN15);
    gpio_reset_pin(&gpio, GPIO_PIN15);
    gpio_set_pin(&gpio, GPIO_PIN15);
    gpio_reset_pin(&gpio, GPIO_PIN15);



   // test();

}
