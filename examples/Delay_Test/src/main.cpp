#include "gpio_cpp.h"
#include "RCC_helper.h"
#include "GPIO_Helper.h"
#include <stdio.h>
#include "debug.h"

int main()
{
#ifdef STM32F4
    //RCC_EnableHSI_84Mhz();
#else
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#endif
    //RCC_SetAHBPrescalerDiv2();
    GPIO_PIN* pin = GPIO_Helper::GetPin("B13");
    pin->SetupGPIO_OutPP();
    pin->SetSpeedHigh();
    Debug_EnableCYCCNT(true);

    uint32_t p1 = 1<<12;
    uint32_t p2 = p1<<16;
    while(1)
    {
        pin->Set();
        //GPIOB_BSRR = p1;
        delay_us(6);
        //GPIOB_BSRR =  p2;
        pin->Reset();
        delay_us(6);


    }

}
