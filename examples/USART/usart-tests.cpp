#include "usart_cpp.h"

//send an incrementing counter value continuously
static void usart_test_tx()
{
    USART1* usart = USART1::GetInstance();

    usart->Enable(true);
    usart->SetBaudRate(9600);
    usart->EnableTransmitter(true);

    uint8_t i = 0;
    while (1)
    {
        usart->WaitTXE();
        usart->Send(++i);
        RCC::GetInstance()->Delay_ms(1000);
    }

}

//receive one byte at a time continuously
static void usart_test_rx()
{
    USART1* usart = USART1::GetInstance();

    usart->Enable(true);
    usart->SetBaudRate(9600);
    usart->EnableReceiver(true);

    volatile uint8_t in = 0;
    while(1)
    {
        usart->WaitRXNE();
        in = usart->Receive();
    }
}




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

    USART1* usart = USART1::GetInstance();
    usart->Enable(true);
    usart->SetBaudRate(9600);
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

    USART1* usart = USART1::GetInstance();
    usart->Enable(true);
    usart->SetBaudRate(9600);
    usart->ConfigureDMARX(data, ndata, &irqHandler);
    usart->EnableReceiver(true);

    while (1)
        ;

}



int main()
{
    RCC* rcc = RCC::GetInstance();
    rcc->SetAHBPrescalerDiv4();
    rcc->SetAPB1PrescalerDiv1();
    rcc->SetAPB2PrescalerDiv1();
    rcc->SetADCPrescalerDiv2();
    rcc->EnableHSI(3);

    usart_test_tx();
    //usart_test_rx();
    //usart_test_DMA_Rx();
    //usart_test_DMA_Tx();

}
