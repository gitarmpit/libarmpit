#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "usart_cpp.h"
#include "GPIO_Helper.h"
#include "fp2.h"

static void run()
{

    uint32_t clockSpeed = 400000;
    //USART* usart = GPIO_Helper::SetupUSART1_PA_9_10(clockSpeed);
    //USART* usart = GPIO_Helper::SetupUSART1_PB_6_7(clockSpeed);
    //USART* usart = GPIO_Helper::SetupUSART2_PA_2_3(clockSpeed);
    //USART* usart = GPIO_Helper::SetupUSART2_PD_5_6(clockSpeed);
    //USART* usart = GPIO_Helper::SetupUSART3_PB_10_11(clockSpeed);
    //USART* usart = GPIO_Helper::SetupUSART3_PD_8_9(clockSpeed);
    //USART* usart = GPIO_Helper::SetupUSART3_PC_10_11(clockSpeed);
    //USART* usart = GPIO_Helper::SetupUSART6_PC_6_7(clockSpeed);

    USART* usart = GPIO_Helper::SetupUSART(USART6_PC_6_7, clockSpeed);
    unsigned char buf[1024];

    while(1)
    {
        while (!usart->IsRXNE())
            ;

        {
            if (usart->Receive() == 23)
            {
                ///for (uint16_t i = 257; i < 64; ++i)
                 usart->SendDWord(fix16_from_dbl(-31000.12345));

                //break;
            }
        }

    }
    while(1)
        ;
}

int main()
{
   RCC_EnableHSI_100Mhz();
   run();
}
