#include "usart_cpp.h"
#include "RCC_Helper.h"
#include "debug.h"
#include "GPIO_Helper.h"
#include <stdio.h>

//send an incrementing counter value continuously
static void usart_test_tx()
{
	//USART* usart = GPIO_Helper::SetupUSART(USART1_PA_9_10, 9600);
	USART* usart = GPIO_Helper::SetupUSART(USART1_PB_6_7, 9600);

    uint8_t i = 0;
    while (1)
    {
        usart->WaitTXE();
        usart->SendByte(++i);
        delay(1000);
    }

}

//receive one byte at a time continuously
static void usart_test_rx()
{
	USART* usart = GPIO_Helper::SetupUSART(USART1_PA_9_10, 9600);

    volatile uint8_t in = 0;
    while(1)
    {
        usart->WaitRXNE();
        in = usart->Receive();
        printf ("%d\n", in);
    }
}



#ifdef STM32F1

class DMA1_IRQ_Handler: public DMA_IRQ_Handler
{
public:
    virtual void HandleInterrupt(DMA_Channel* channel, bool isTransferComplete,
            bool isHalfTransfer, bool isTransferError)
    {
        static volatile bool complete;
        static volatile bool half;
        static volatile bool err;

        complete = isTransferComplete;
        half = isHalfTransfer;
        err = isTransferError;
    }

    virtual ~DMA1_IRQ_Handler()
    {
    }

};



//send 1024 bytes once via DMA
static void usart_test_DMA_Tx()
{
    static const int ndata = 1024;
    uint8_t data[ndata];

    DMA1_IRQ_Handler irqHandler;

	USART* usart = GPIO_Helper::SetupUSART(USART1_PA_9_10, 9600);
    usart->EnableTransmitter(false);
    usart->ConfigureDMATX(data, ndata, &irqHandler);
    usart->EnableTransmitter(true);

    while (1)
        ;
}

//receive 16 bytes once via DMA
static void usart_test_DMA_Rx()
{
    static const uint8_t ndata = 16;
    static uint8_t data[ndata];

    DMA1_IRQ_Handler irqHandler;

	USART* usart = GPIO_Helper::SetupUSART(USART1_PA_9_10, 9600);
    usart->EnableReceiver(false);
    usart->ConfigureDMARX(data, ndata, &irqHandler);
    usart->EnableReceiver(true);

    while (1)
        ;

}

#endif

int main()
{
#ifdef STM32F1
	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#else
	RCC_EnableHSI_168Mhz();
#endif
	Debug_EnableCYCCNT(TRUE);
	//usart_test_tx();
    usart_test_rx();
    //usart_test_DMA_Rx();
    //usart_test_DMA_Tx();

}
