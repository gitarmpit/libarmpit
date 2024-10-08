#ifndef _SPI_CPP_H
#define _SPI_CPP_H
#include "spi.h"
#include "dma_cpp.h"


#include "nvic.h"
#include "rcc.h"

#include "peripheral.h"
#include "gpio_cpp.h"



enum SPI_BAUD_RATE
{
    SPI_BAUD_RATE_2 = SPI_CR1_BR_FPCLK_DIV_2,
    SPI_BAUD_RATE_4 = SPI_CR1_BR_FPCLK_DIV_4,
    SPI_BAUD_RATE_8 = SPI_CR1_BR_FPCLK_DIV_8,
    SPI_BAUD_RATE_16 = SPI_CR1_BR_FPCLK_DIV_16,
    SPI_BAUD_RATE_32 = SPI_CR1_BR_FPCLK_DIV_32,
    SPI_BAUD_RATE_64 = SPI_CR1_BR_FPCLK_DIV_64,
    SPI_BAUD_RATE_128 = SPI_CR1_BR_FPCLK_DIV_128,
    SPI_BAUD_RATE_256 = SPI_CR1_BR_FPCLK_DIV_256
};

class SPI_IRQ_Handler;

class SPI : public Peripheral
{
private:
    volatile uint32_t* _pSPI_CR1;
    volatile uint32_t* _pSPI_CR2;
    volatile uint32_t* _pSPI_SR;
    volatile uint32_t* _pSPI_DR;
    volatile uint32_t* _pSPI_CRCPR;
    volatile uint32_t* _pSPI_RXCRCR;
    volatile uint32_t* _pSPI_TXCRCR;
    SPI_IRQ_Handler* _irqHandler;
    IRQn_Type _irqNo;
    bool _startedSending;



#if defined(STM32F4) || defined(STM32F2)
    DMA_Stream* _dmaTxStream;
#endif

    void _EnableInterrupt(uint16_t intMask, bool enable);

protected:
    SPI(volatile uint32_t* pSPI_CR1, volatile uint32_t* pSPI_CR2,
            volatile uint32_t* pSPI_SR, volatile uint32_t* pSPI_DR,
            volatile uint32_t* pSPI_CRCPR, volatile uint32_t* pSPI_RXCRCR,
            volatile uint32_t* pSPI_TXCRCR, IRQn_Type irqNo,
            volatile uint32_t* pRCCEnableReg,
            volatile uint32_t* pRCCResetReg,
            uint32_t rccBit);
#if defined (STM32F1)
    DMA_CHANNEL _dmaRxChannel;
    DMA_CHANNEL _dmaTxChannel;
#endif

public:

    //void SetupPins (GPIO_PIN* MOSI_pin, GPIO_PIN* MISO_pin, GPIO_PIN* CLK_pin, GPIO_PIN* SS_pin);

    void Enable(bool enable);
    void SetClockPhaseFirst();
    void SetClockPhaseSecond();
    void SetClockPolarity0();
    void SetClockPolarity1();
    void SetMaster(bool isMaster);
    void SetBaudRate(SPI_BAUD_RATE rate);
    void SetBaudRateDiv2();
    void SetBaudRateDiv4();
    void SetBaudRateDiv8();
    void SetBaudRateDiv16();
    void SetBaudRateDiv32();
    void SetBaudRateDiv64();
    void SetBaudRateDiv128();
    void SetBaudRateDiv256();
    void SetMSBFirst();
    void SetLSBFirst();
    void SetSSM(bool isEnabled);
    void SetSSI (bool set);
    void SetFullDuplex(bool isFullDuplex);
    void SetFullDuplexRxOnly(bool set);
    void SetDFF_8bit();
    void SetDFF_16bit();
    void SetCRCNext(bool isCRCnext);
    void EnableCRC();
    void DisableCRC();
    void SetBidirectionalReceiveOnly();
    void SetBidirectionalTransmitOnly();
    void Enable_RX_DMA(bool rxDmaEnable);
    void Enable_TX_DMA(bool rxDmaEnable);
    void EnableSSOE(bool enable);
    void EnableErrorInterrupt(bool enable);
    void EnableRXNEInterrupt(bool enable);
    void EnableTXEInterrupt(bool enable);
    bool IsBusy();
    bool IsOverrun();
    bool IsModeFault();
    bool IsCRCError();
    bool IsTXE();
    bool IsRXNE();
    bool IsError();

    void ClearBusy();
    void ClearOverrun();
    void ClearModeFault();
    void ClearCRCError();
    void ClearTXE();
    void ClearRXNE();

    volatile uint32_t* GetDRAddr()
    {
        return _pSPI_DR;
    }

    volatile uint32_t* GetSRAddr()
    {
        return _pSPI_SR;
    }

    void SetPolynomial(uint16_t polynomial);
    volatile uint32_t* GetRxCrcRegister()
    {
        return _pSPI_RXCRCR;
    }
    volatile uint32_t* GetTxCrcRegister()
    {
        return _pSPI_TXCRCR;
    }

    void SetIrqHandler(SPI_IRQ_Handler* handler)
    {
        _irqHandler = handler;
    }
    void HandleInterrupt();

    void Initialize(bool isMaster, bool isFullDuplex, SPI_BAUD_RATE baudRate, bool isClockPhase2,
            bool isClockPolarity1, bool isLSBFirst, bool isSetSSM, bool is16bit);

    void WaitTXE();
    void WaitRXNE();
    void WaitNotBSY();

    uint8_t TransmitByte(uint8_t byte);
    uint16_t TransmitWord(uint16_t word);
    uint8_t TransmitRegister (uint8_t reg, uint8_t data);
    uint16_t TransmitRegister (uint16_t reg, uint16_t data);
    void SendByte (uint8_t byte);
    void SendWord (uint16_t byte);
    uint8_t ReceiveByte();
    uint16_t ReceiveWord();

#if defined(STM32F4) || defined(STM32F2)
    void SetDMA_TxStream (DMA_Stream* stream) { _dmaTxStream = stream; }
    bool SendDMA(void* data_ptr, uint32_t data_size);
    DMA_Stream*  GetDMA_TxStream() { return _dmaTxStream; }
#elif defined(STM32F1)
    bool SendDMA(void* data_ptr, uint32_t data_size);
#endif

};

class SPI1: public SPI
{
public:

    static SPI1* GetInstance()
    {
        static SPI1 instance;
        return &instance;
    }

private:

    SPI1() :
    SPI(&SPI1_CR1,
            &SPI1_CR2,
            &SPI1_SR,
            &SPI1_DR,
            &SPI1_CRCPR,
            &SPI1_RXCRCR,
            &SPI1_TXCRCR,
            IRQn_SPI1,
            &RCC_APB2ENR, &RCC_APB2RSTR,
            RCC_APB2ENR_SPI1EN)
    {
#if defined (STM32F1)
        _dmaRxChannel = DMA_CHANNEL_2;
        _dmaTxChannel = DMA_CHANNEL_3;
#endif
    }

    SPI1(SPI1 const&);
    void operator=(SPI1 const&);
};

#if defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) || defined(STM32F10X_HD)\
        || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL) || defined(STM32F10X_CL)\
        || defined(STM32F4) || defined(STM32F2)

class SPI2 : public SPI
{
public:

    static SPI2* GetInstance()
    {
        static SPI2 instance;
        return &instance;
    }

private:

    SPI2() : SPI(&SPI2_CR1, &SPI2_CR2, &SPI2_SR, &SPI2_DR, &SPI2_CRCPR, &SPI2_RXCRCR, &SPI2_TXCRCR,
            IRQn_SPI2,
            &RCC_APB1ENR, &RCC_APB1RSTR,
            RCC_APB1ENR_SPI2EN)
    {
#if defined (STM32F1)
        _dmaRxChannel = DMA_CHANNEL_4;
        _dmaTxChannel = DMA_CHANNEL_5;
#endif
    }

    SPI2(SPI2 const&);
    void operator=(SPI2 const&);
};

#endif

#if defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL) || defined(STM32F10X_CL)\
|| defined(STM32F4) || defined(STM32F2)

class SPI3 : public SPI
{
public:

    static SPI3* GetInstance()
    {
        static SPI3 instance;
        return &instance;
    }

private:

    SPI3() : SPI(&SPI3_CR1, &SPI3_CR2, &SPI3_SR, &SPI3_DR, &SPI3_CRCPR, &SPI3_RXCRCR, &SPI3_TXCRCR,
            IRQn_SPI3,
            &RCC_APB1ENR, &RCC_APB1RSTR,
#if defined (STM32F1)
#endif
            RCC_APB1ENR_SPI3EN)

    {
#if defined (STM32F1)
        _dmaRxChannel = DMA_CHANNEL_NOT_SUPPORTED;
        _dmaTxChannel = DMA_CHANNEL_NOT_SUPPORTED;
#endif
    }

    SPI3(SPI3 const&);
    void operator=(SPI3 const&);
};

#endif

class SPI_IRQ_Handler
{
public:
    virtual void Handle(SPI* spi, bool isCRCError, bool isOverrunError,
            bool isModeError, bool isTXE, bool isRXNE) = 0;
    virtual ~SPI_IRQ_Handler()
    {
    }
};

#endif
