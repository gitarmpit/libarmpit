
#include <stdint.h>
#include "common.h"
#include "_rcc.h"
#include "_gpio.h"
#include "nvic.h"

extern uint32_t* __Vectors;
extern uint32_t* SysTick_Handler;

volatile uint32_t* p;

void func()
{
    while(1);
}



int main()
{
    //_test();
    
    //0-15
    NVIC_SetPriority(IRQn_USBWakeUp, 0x8);
    volatile uint8_t p = NVIC_GetPriority(IRQn_USBWakeUp);
    
    GPIO_CFG gpio;
    GPIO_init (GPIOC, &gpio);

    
   *((uint64_t*)gpio.crl) = 0x3333333333333333llu;
    
    uint8_t s = GPIO_is_set (&gpio, GPIO_PIN0);
    s = GPIO_is_set (&gpio, GPIO_PIN8);

    GPIO_cfg_in_float(&gpio, GPIO_PIN0);
    GPIO_cfg_out_alt_pp(&gpio, GPIO_PIN0);
    GPIO_cfg_in_float(&gpio, GPIO_PIN15);
    GPIO_cfg_out_alt_pp(&gpio, GPIO_PIN15);
    GPIO_cfg_in_pullup(&gpio, GPIO_PIN7);
    GPIO_cfg_in_pulldown(&gpio, GPIO_PIN7);
    GPIO_cfg_in_anal(&gpio, GPIO_PIN2);

    GPIO_set_output(&gpio, 0xffff);
    GPIO_set_output(&gpio, 0x00);
    GPIO_set_pin(&gpio, GPIO_PIN15);
    GPIO_reset_pin(&gpio, GPIO_PIN15);
    GPIO_set_pin(&gpio, GPIO_PIN15);
    GPIO_reset_pin(&gpio, GPIO_PIN15);


    while(1)
        ;

   // test();

}
