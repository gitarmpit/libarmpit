#include "spi_cpp.h"
#include "gpio_cpp.h"
#include "GPIO_Helper.h"
#include "debug.h"
#include "RCC_Helper.h"


#define ERROR_BIT 1<<16

GPIO_PIN* ssPin;
SPI* spi;

static BOOL read_temp(float* temp)
{
    BOOL rc = false;
    uint32_t temp_reg = 0;
    float t_int;  // reference junction temp

    ssPin->Reset();

    temp_reg = 0;
    for (uint8_t i = 0; i < 4; ++i)
    {
        temp_reg |= (spi->TransmitByte(0x0) << ((3 - i) * 8)
                & (0xff << ((3 - i) * 8)));
    }

    ssPin->Set();

    if (!(temp_reg & ERROR_BIT))
    {
        *temp = (temp_reg >> 18 & 0x3fff) * 0.25;
        t_int = (temp_reg >> 4 & 0xfff) * 0.0625;
        rc = TRUE;
    }

    return rc;
}

static void run()
{
    GPIO_PIN* ssPin = GPIO_Helper::GetPin("A4");
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

    spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_128);

    float t;

    delay(150);

    read_temp (&t);
    read_temp (&t);
    read_temp (&t);
    read_temp (&t);
    
    USART* usart = GPIO_Helper::SetupUSART(USART1_PA_9_10, 9600);

    volatile uint8_t in = 0;
    
    while (true)
    {
        usart->WaitRXNE();
        in = usart->Receive();
        if (in == 23) 
        {
            usart->WaitTXE();
            read_temp (&t);
            usart->SendByte((uint8_t) t);
        }
    }

}

int main(void)
{
#ifdef STM32F1
	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#else
	RCC_EnableHSI_168Mhz();
#endif
	Debug_EnableCYCCNT(TRUE);

    run();

}
