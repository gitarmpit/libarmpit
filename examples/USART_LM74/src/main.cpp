#include "LM74.h"
#include "gpio_cpp.h"
#include "spi_cpp.h"
#include "usart_cpp.h"
#include "dma_cpp.h"
#include "RCC_Helper.h"
#include "GPIO_Helper.h"

//Not used, just an example how an interrupt handler could be used
//for async communication, reacting to RXNE/TXE

class USART1_IRQ_Handler: public USART_IRQ_Handler
{
private:

    LM74 *_lm74;

public:

    USART1_IRQ_Handler(LM74 *lm74)
    {
        _lm74 = lm74;
    }

    virtual void Handle(USART *usart, USART_IRQ_Info info)
    {
        static int t = 0;

        if (info.isFrameError || info.isOverrun || info.isNoiseError
                || info.isPE)
        {
            volatile int i = 0;
        }

        if (info.isTXE)
        {
            t = (int) _lm74->GetTemp();
            if (t > -60)
                usart->SendByte((uint8_t) t);
        }

        if (info.isRXNE)
        {
            volatile int i = 0;
        }

        if (info.isTC)
        {

            volatile int i = 0;
        }

    }

};

//reads LM74 temperature via SPI and sends it to peer. 
//peer initiates read by sending byte with a value of 23 (decimal)
static void temp_read()
{
    GPIO_PIN *ssPin = GPIO_Helper::GetPin("A4");
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

    SPI *spi1 = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false,
            SPI_BAUD_RATE_128);

    LM74 lm74(spi1, ssPin);
    lm74.Init();

    USART1 *usart = USART1::GetInstance();

    usart->Enable(true);
    usart->SetBaudRate(9600);
    usart->EnableTransmitter(true);
    usart->EnableReceiver(true);

    volatile uint8_t in = 0;
    while (1)
    {
        usart->WaitRXNE();
        in = usart->Receive();
        if (in == 23)
        {
            usart->WaitTXE();
            uint32_t t = (int) lm74.GetTemp();
            usart->SendByte((uint8_t) t);
        }
    }
}

int main()
{
#ifdef STM32F1
	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#else
    RCC_EnableHSI_168Mhz();
#endif
    temp_read();
}
