


#include "dma_cpp.h"
#include "RCC_Helper.h"

class DMA1_IRQ_Handler: public DMA_IRQ_Handler
{
public:
    virtual void HandleInterrupt(DMA_Channel* channel, bool isTransferComplete,
            bool isHalfTransfer, bool isTransferError)
    {
        //volatile bool complete = isTransferComplete;
        //volatile bool half = isHalfTransfer;
        //volatile bool err = isTransferError;
    }

    virtual ~DMA1_IRQ_Handler()
    {
    }

};

uint32_t from[3] = { 1, 2, 3 };
uint32_t to[3] =   { 0, 0, 0 };

static void DMA_Test()
{

    DMA1* dma1 = DMA1::GetInstance();
    dma1->EnablePeripheralClock(true);
    //dma1->SetIRQHandler (new DMA1_IRQ_Handler());
    DMA_Channel* ch3 = dma1->GetChannel(DMA_CHANNEL_2);
    ch3->EnableTransferCompleteInterrupt();
    ch3->EnableTransferErrorInterrupt();
    ch3->SetDirection_MemoryToMemory();
    ch3->SetCircularMode();
    ch3->SetMemorySize_32bits();
    ch3->SetPeripheralSize_32bits();
    ch3->SetMemoryIncrementMode();
    ch3->SetPeripheralIncrementMode();
    ch3->SetPriorityHigh();
    ch3->SetNumberOfData(3);
    ch3->SetMemoryAddress((uint32_t) to);
    ch3->SetPeripheralAddress((uint32_t) from);

    ch3->EnableChannel(true);
    ;
    while (1)
    {
    }

}

int main()
{
	
		RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
	 DMA_Test();
}
