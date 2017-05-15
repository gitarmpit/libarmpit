#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "usart_cpp.h"
#include "GPIO_Helper.h"
#include "fp2.h"
#include "afio.h"

static void run()
{
    RCC_EnableAFIO(true);

    uint32_t clockSpeed = 400000;
    //USART* usart = GPIO_Helper::SetupUSART1_PA_9_10(clockSpeed);
    USART* usart = GPIO_Helper::SetupUSART(USART2_PA_2_3, clockSpeed);
    //USART* usart = GPIO_Helper::SetupUSART3_PB_10_11(clockSpeed);
    //USART* usart = GPIO_Helper::SetupUSART1_PB_6_7(clockSpeed);

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
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
   run();
}
