#include "gpio.h"
#include "timer.h"
#include "flash.h"
#include "rcc.h"
#include "debug.h"
#include "afio.h"
#include "exti.h"

GPIO_CFG gpio;


void EXTI3_IRQHandler(void) 
{
    EXTI_IsPending (3, TRUE);
}


int main()
{
    FLASH_SetWaitState(2); 
    RCC_EnablePLL(16); 
    GPIO_init (GPIOB, &gpio);

    GPIO_cfg_in_pulldown(&gpio, GPIO_PIN3);
    
    
    AFIO_Enable(TRUE);
    AFIO_Remap_SWJ_CFG (FALSE, TRUE, FALSE);
    AFIO_Remap_EXTI (3, GPIOB);

    EXTI_EnableInterruptRisingEdge (3, TRUE);
    
    while(1)
        ;


}
