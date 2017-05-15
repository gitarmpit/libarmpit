#include "gpio.h"
#include "timer.h"
#include "flash.h"
#include "rcc.h"
#include "debug.h"

GPIO_CFG gpio;


TIM_CFG cfg;


void TIM2_IRQHandler() 
{
    if (TIM_IsUIF(&cfg)) 
    {
        GPIO_toggle_pin(&gpio, GPIO_PIN0);
        TIM_ClearUIF(&cfg);
    }
}

int main()
{
    FLASH_SetWaitState(2); 
    RCC_EnablePLL(16); 
    //RCC_SetAHBPrescaler(2);
    GPIO_init (GPIOA, &gpio);

    GPIO_cfg_out_alt_pp(&gpio, GPIO_PIN0);
    
    
    TIM_Init (TIM2, &cfg);
    
    TIM_SetupPWM1 (&cfg, 1, 1000000, 500000);
    
    
    TIM_EnableChannel (&cfg, 1, TRUE);
    
    TIM_EnableCounter (&cfg, TRUE);

    uint32_t duty = 20000; 
    uint32_t inc = 50000;
    uint32_t i = 0; 
    Debug_EnableCYCCNT(TRUE);
    while (1) 
    {
        delay_us(5000000);
        TIM_UpdatePWMDuty (&cfg, 1, duty);
        duty += inc; 
        if (duty >= 500000) 
        {
            duty = 20000;
        }
    }

    while(1)
        ;

   // test();

}
