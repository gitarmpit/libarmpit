#if 0
#include "rcc.h"
#include "gpio.h"
#include "debug.h"
#include "systick.h"
#include "flash.h"
#include "rcc.h"

GPIO_CFG gpio;

void SysTick_Handler()
{
    GPIO_toggle_pin(&gpio, GPIO_PIN0);
}

int main()
{

    FLASH_SetWaitState(2);
    uint8_t ws = FLASH_GetWaitState();


    RCC_EnablePLL(4);
    volatile uint32_t sysclk = RCC_GetSystemClock();


    GPIO_init (GPIOA, &gpio);

    GPIO_cfg_out_pp(&gpio, GPIO_PIN0);
    GPIO_set_pin(&gpio, GPIO_PIN0);
    GPIO_reset_pin(&gpio, GPIO_PIN0);
    GPIO_toggle_pin(&gpio, GPIO_PIN0);
    GPIO_toggle_pin(&gpio, GPIO_PIN0);

    uint32_t ahb_clk = RCC_GetAHB_CLK();
    SystickEnable (ahb_clk, TRUE, FALSE);

    while(1)
        ;

}

#endif
