#include "gpio.h"
#include "systick.h"
#include "rcc.h"

GPIO_CFG gpio;

void SysTick_Handler() 
{
    GPIO_toggle_pin(&gpio, GPIO_PIN0);
}

int main()
{
    RCC_EnablePLL(4);
    GPIO_init (GPIOA, &gpio);

    GPIO_cfg_out_pp(&gpio, GPIO_PIN0);
    GPIO_set_pin(&gpio, GPIO_PIN0);
    GPIO_reset_pin(&gpio, GPIO_PIN0);
    GPIO_toggle_pin(&gpio, GPIO_PIN0);
    GPIO_toggle_pin(&gpio, GPIO_PIN0);

    SystickEnable (RCC_GetAHB_CLK(), TRUE, FALSE);

    while(1)
        ;


}
