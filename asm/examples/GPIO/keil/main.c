#include "gpio.h"



int main()
{
    GPIO_CFG gpio;
    GPIO_init (GPIOA, &gpio);

    GPIO_cfg_out_pp(&gpio, GPIO_PIN0);
    GPIO_set_pin(&gpio, GPIO_PIN0);
    GPIO_reset_pin(&gpio, GPIO_PIN0);
    GPIO_toggle_pin(&gpio, GPIO_PIN0);
    GPIO_toggle_pin(&gpio, GPIO_PIN0);


    while(1)
        ;

   // test();

}
