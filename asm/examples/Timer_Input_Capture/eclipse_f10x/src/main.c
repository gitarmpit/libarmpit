#if 1
#include "rcc.h"
#include "gpio.h"
#include "debug.h"

int main()
{
    volatile uint32_t sysclk = RCC_GetSystemClock();

    FLASH_SetWaitState(2);
    uint8_t ws = FLASH_GetWaitState();


    RCC_EnablePLL(16);


    GPIO_CFG gpio;
    GPIO_init (GPIOA, &gpio);

    GPIO_cfg_out_pp(&gpio, GPIO_PIN0);
    GPIO_set_pin(&gpio, GPIO_PIN0);
    GPIO_reset_pin(&gpio, GPIO_PIN0);
    GPIO_toggle_pin(&gpio, GPIO_PIN0);
    GPIO_toggle_pin(&gpio, GPIO_PIN0);

    Debug_EnableCYCCNT(TRUE);

    while(1)
    {
        GPIO_toggle_pin(&gpio, GPIO_PIN0);
        delay_us(1000000);
    }
}

#endif
