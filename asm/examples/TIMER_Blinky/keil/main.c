#include "gpio.h"
#include "timer.h"
#include "flash.h"
#include "rcc.h"

GPIO_CFG gpio;


TIM_CFG cfg;


void TIM2_IRQHandler2() 
{
    if (TIM_IsUIF(&cfg)) 
    {
        GPIO_toggle_pin(&gpio, GPIO_PIN0);
        TIM_ClearUIF(&cfg);
    }
}

int main()
{
    //FLASH_SetWaitState(2); 
    //RCC_EnablePLL(16); 
    RCC_SetAHBPrescaler(4);
    RCC_SetAPB1Prescaler(16);
    GPIO_init (GPIOA, &gpio);

    GPIO_cfg_out_pp(&gpio, GPIO_PIN0);
    //GPIO_set_pin(&gpio, GPIO_PIN0);
    //GPIO_reset_pin(&gpio, GPIO_PIN0);
    GPIO_toggle_pin(&gpio, GPIO_PIN0);
    GPIO_toggle_pin(&gpio, GPIO_PIN0);
    
    
    TIM_Init (TIM2, &cfg);
    
    TIM_SetupPWM1 (&cfg, 1, 1000000, 500000);
    
    TIM_EnableAutoPreload (&cfg, TRUE);
    
    TIM_EnableOCPreload (&cfg, 1, TRUE);
    TIM_EnableOCPreload (&cfg, 2, TRUE);
    TIM_EnableOCPreload (&cfg, 3, TRUE);
    TIM_EnableOCPreload (&cfg, 4, TRUE);
    TIM_EnableOCPreload (&cfg, 1, 0);
    TIM_EnableOCPreload (&cfg, 2, 0);
    TIM_EnableOCPreload (&cfg, 3, 0);
    TIM_EnableOCPreload (&cfg, 4, 0);
    
    TIM_SetOCMode_PWM2 (&cfg, 1);
    TIM_SetOCMode_PWM2 (&cfg, 2);
    TIM_SetOCMode_PWM2 (&cfg, 3);
    TIM_SetOCMode_PWM2 (&cfg, 4);
    TIM_SetOCMode_PWM1 (&cfg, 1);
    TIM_SetOCMode_PWM1 (&cfg, 2);
    TIM_SetOCMode_PWM1 (&cfg, 3);
    TIM_SetOCMode_PWM1 (&cfg, 4);
    
    TIM_EnableNVICInterrupt (&cfg, TRUE);
    TIM_EnableUpdateInterrupt (&cfg, TRUE);
    TIM_SetCounterValue (&cfg, 0x0);
    TIM_SetUpdatePeriod_us (&cfg, 1000000);
    
    TIM_EnableCounter (&cfg, TRUE);

   

    while(1)
    {
        volatile uint32_t i = 0; 
        ++i;
        //Debug_EnableCYCCNT(TRUE);        
    }

   // test();

}
