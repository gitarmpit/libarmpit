#ifndef _SPI_H
#define _SPI_H

#define SPI1 0 
#define SPI2 1 
#define SPI3 2

#if defined(__GNUC__) && defined(__ASSEMBLY__)



#define SPI1_BASE 0x40013000  //APB2

#define SPI1_CR1    SPI1_BASE
#define SPI1_CR2    SPI1_BASE + 0x4
#define SPI1_SR     SPI1_BASE + 0x8    
#define SPI1_DR     SPI1_BASE + 0xc        
#define SPI1_CRCPR   SPI1_BASE + 0x10      
#define SPI1_RXCRCR SPI1_BASE + 0x14       
#define SPI1_TXCRCR SPI1_BASE + 0x18           


#if defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) || defined(STM32F10X_HD)\
        || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL) || defined(STM32F10X_CL)\
        || defined(STM32F4)  || defined(STM32L0)

#define SPI2_BASE 0x40003800  //APB1

#define SPI2_CR1    SPI2_BASE
#define SPI2_CR2    SPI2_BASE + 0x4
#define SPI2_SR     SPI2_BASE + 0x8    
#define SPI2_DR     SPI2_BASE + 0xc        
#define SPI2_CRCPR  SPI2_BASE + 0x10       
#define SPI2_RXCRCR SPI2_BASE + 0x14       
#define SPI2_TXCRCR SPI2_BASE + 0x18           

#endif


#if defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL) || defined(STM32F10X_CL)\
        || defined(STM32F4)



#define SPI3_BASE 0x40003C00  //APB1 (High density 100x, all 10x)
#define SPI3_CR1    SPI3_BASE
#define SPI3_CR2    SPI3_BASE + 0x4
#define SPI3_SR     SPI3_BASE + 0x8    
#define SPI3_DR     SPI3_BASE + 0xc        
#define SPI3_CRCPR  SPI3_BASE + 0x10       
#define SPI3_RXCRCR SPI3_BASE + 0x14       
#define SPI3_TXCRCR SPI3_BASE + 0x18           

#endif


#define SPI_CR1_CPHA_FIRST       (0<<0)
#define SPI_CR1_CPHA_SECOND      (1<<0)
#define SPI_CR1_CPOL_0           (0<<1)
#define SPI_CR1_CPOL_1           (1<<1)
#define SPI_CR1_SLAVE            (0<<2)
#define SPI_CR1_MASTER           (1<<2)
#define SPI_CR1_BR_FPCLK_DIV_2   (0<<3)
#define SPI_CR1_BR_FPCLK_DIV_4   (1<<3) 
#define SPI_CR1_BR_FPCLK_DIV_8   (2<<3) 
#define SPI_CR1_BR_FPCLK_DIV_16  (3<<3)
#define SPI_CR1_BR_FPCLK_DIV_32  (4<<3)
#define SPI_CR1_BR_FPCLK_DIV_64  (5<<3)
#define SPI_CR1_BR_FPCLK_DIV_128 (6<<3)
#define SPI_CR1_BR_FPCLK_DIV_256 (7<<3)
#define SPI_CR1_SPE_ENABLED      (1<<6)
#define SPI_CR1_LSBFIRST_OFF     (0<<7)
#define SPI_CR1_LSBFIRST_ON      (1<<7)
#define SPI_CR1_SSI              (1<<8)
#define SPI_CR1_SSM              (1<<9) 
#define SPI_CR1_RXONLY           (1<<10) 
#define SPI_CR1_DFF_8_BIT        (0<<11)
#define SPI_CR1_DFF_16_BIT       (1<<11)
#define SPI_CR1_CRCNEXT          (1<<12) 
#define SPI_CR1_CRCEN            (1<<13) 
#define SPI_CR1_BIDIOE           (1<<14) 
#define SPI_CR1_BIDIMODE         (1<<14) 


#define SPI_CR2_RXDMAEN         (1<<0)
#define SPI_CR2_TXDMAEN         (1<<1)
#define SPI_CR2_SSOE            (1<<2)
#define SPI_CR2_ERRIE           (1<<5)
#define SPI_CR2_RXNEIE          (1<<6) 
#define SPI_CR2_TXEIE           (1<<7) 

#define SPI_SR_RXNE             (1<<0) 
#define SPI_SR_TXE              (1<<1) 
#define SPI_SR_CRCERR           (1<<4)
#define SPI_SR_MODF             (1<<5) 
#define SPI_SR_OVR              (1<<6) 
#define SPI_SR_BSY              (1<<7) 


#else
 
//c
#include "common.h"


typedef struct 
{
    uint32_t SPI_base;
    uint32_t irqNo;
    uint32_t  RCC_EN_bit;
    BOOL     isSPI1;
} SPI_CFG;

void SPI_Init(uint8_t spi_no,  SPI_CFG* cfg);
void SPI_Enable(SPI_CFG* cfg, bool enable);
void SPI_SetClockPhaseFirst(SPI_CFG* cfg);
void SPI_SetClockPhaseSecond(SPI_CFG* cfg);
void SPI_SetClockPolarity0(SPI_CFG* cfg);
void SPI_SetClockPolarity1(SPI_CFG* cfg);
void SPI_SetMaster(SPI_CFG* cfg, bool isMaster);
void SPI_SetBaudRate(SPI_CFG* cfg, SPI_BAUD_RATE rate);
void SPI_SetBaudRateDiv2(SPI_CFG* cfg);
void SPI_SetBaudRateDiv4(SPI_CFG* cfg);
void SPI_SetBaudRateDiv8(SPI_CFG* cfg);
void SPI_SetBaudRateDiv16(SPI_CFG* cfg);
void SPI_SetBaudRateDiv32(SPI_CFG* cfg);
void SPI_SetBaudRateDiv64(SPI_CFG* cfg);
void SPI_SetBaudRateDiv128(SPI_CFG* cfg);
void SPI_SetBaudRateDiv256(SPI_CFG* cfg);
void SPI_SetFrameFormatMSBFirst(SPI_CFG* cfg);
void SPI_SetFrameFormatLSBFirst(SPI_CFG* cfg);
void SPI_SetSSM(SPI_CFG* cfg, bool isEnabled);
void SPI_SetSSI (SPI_CFG* cfg, bool set);
void SPI_SetFullDuplex(SPI_CFG* cfg, bool isFullDuplex);
void SPI_SetFullDuplexRxOnly(SPI_CFG* cfg, bool set);
void SPI_SetDFF_8bit(SPI_CFG* cfg);
void SPI_SetDFF_16bit(SPI_CFG* cfg);
void SPI_SetCRCNext(SPI_CFG* cfg, bool isCRCnext);
void SPI_EnableCRC(SPI_CFG* cfg);
void SPI_DisableCRC(SPI_CFG* cfg);
void SPI_SetBidirectionalReceiveOnly(SPI_CFG* cfg);
void SPI_SetBidirectionalTransmitOnly(SPI_CFG* cfg);
void SPI_Enable_RX_TX_DMA(SPI_CFG* cfg, bool rxDmaEnable, bool txDmaEnable);
void SPI_EnableSSOE(SPI_CFG* cfg, bool enable);
void SPI_EnableErrorInterrupt(SPI_CFG* cfg, bool enable);
void SPI_EnableRXNEInterrupt(SPI_CFG* cfg, bool enable);
void SPI_EnableTXEInterrupt(SPI_CFG* cfg, bool enable);
bool SPI_IsBusy(SPI_CFG* cfg);
bool SPI_IsOverrun(SPI_CFG* cfg);
bool SPI_IsModeFault(SPI_CFG* cfg);
bool SPI_IsCRCError(SPI_CFG* cfg);
bool SPI_IsTXE(SPI_CFG* cfg);
bool SPI_IsRXNE(SPI_CFG* cfg);
bool SPI_IsError(SPI_CFG* cfg);

void SPI_ClearBusy(SPI_CFG* cfg);
void SPI_ClearOverrun(SPI_CFG* cfg);
void SPI_ClearModeFault(SPI_CFG* cfg);
void SPI_ClearCRCError(SPI_CFG* cfg);
void SPI_ClearTXE(SPI_CFG* cfg);
void SPI_ClearRXNE(SPI_CFG* cfg);



#endif


#endif