

#ifndef _DMA_H
#define _DMA_H

#include "common.h"


#define DMA1_BASE 0x40026000  
#define DMA2_BASE 0x40026400  

//interrupt status register
#define DMA1_LISR    (*(volatile uint32_t*)(DMA1_BASE + 0x00))
#define DMA2_LISR    (*(volatile uint32_t*)(DMA2_BASE + 0x00))

#define DMA1_HISR    (*(volatile uint32_t*)(DMA1_BASE + 0x04))
#define DMA2_HISR    (*(volatile uint32_t*)(DMA2_BASE + 0x04))

#define DMA_ISR_FEIF0_OFFSET  0
#define DMA_ISR_DMEIF0_OFFSET 2
#define DMA_ISR_TEIF0_OFFSET  3
#define DMA_ISR_HTIF0_OFFSET  4
#define DMA_ISR_TCIF0_OFFSET  5
//
#define DMA_ISR_FEIF1_OFFSET  6
#define DMA_ISR_DMEIF1_OFFSET 8
#define DMA_ISR_TEIF1_OFFSET  9
#define DMA_ISR_HTIF1_OFFSET  10
#define DMA_ISR_TCIF1_OFFSET  11


//interrupt clear register
#define DMA1_LIFCR    (*(volatile uint32_t*)(DMA1_BASE + 0x08))
#define DMA2_LIFCR    (*(volatile uint32_t*)(DMA2_BASE + 0x08))

#define DMA1_HIFCR    (*(volatile uint32_t*)(DMA1_BASE + 0x0c))
#define DMA2_HIFCR    (*(volatile uint32_t*)(DMA2_BASE + 0x0c))

#define DMA_IFCR_FEIF0_OFFSET  0
#define DMA_IFCR_DMEIF0_OFFSET 2
#define DMA_IFCR_TEIF0_OFFSET  3
#define DMA_IFCR_HTIF0_OFFSET  4
#define DMA_IFCR_TCIF0_OFFSET  5
//
#define DMA_IFCR_FEIF1_OFFSET  6
#define DMA_IFCR_DMEIF1_OFFSET 8
#define DMA_IFCR_TEIF1_OFFSET  9
#define DMA_IFCR_HTIF1_OFFSET  10
#define DMA_IFCR_TCIF1_OFFSET  11


//configuration register
#define DMA1_S0CR   (*(volatile uint32_t*)(DMA1_BASE + 0x10 + 0x18*(0)))
#define DMA1_S1CR   (*(volatile uint32_t*)(DMA1_BASE + 0x10 + 0x18*(1)))
#define DMA1_S2CR   (*(volatile uint32_t*)(DMA1_BASE + 0x10 + 0x18*(2)))
#define DMA1_S3CR   (*(volatile uint32_t*)(DMA1_BASE + 0x10 + 0x18*(3)))
#define DMA1_S4CR   (*(volatile uint32_t*)(DMA1_BASE + 0x10 + 0x18*(4)))
#define DMA1_S5CR   (*(volatile uint32_t*)(DMA1_BASE + 0x10 + 0x18*(5)))
#define DMA1_S6CR   (*(volatile uint32_t*)(DMA1_BASE + 0x10 + 0x18*(6)))
#define DMA1_S7CR   (*(volatile uint32_t*)(DMA1_BASE + 0x10 + 0x18*(7)))
#define DMA2_S0CR   (*(volatile uint32_t*)(DMA2_BASE + 0x10 + 0x18*(0)))
#define DMA2_S1CR   (*(volatile uint32_t*)(DMA2_BASE + 0x10 + 0x18*(1)))
#define DMA2_S2CR   (*(volatile uint32_t*)(DMA2_BASE + 0x10 + 0x18*(2)))
#define DMA2_S3CR   (*(volatile uint32_t*)(DMA2_BASE + 0x10 + 0x18*(3)))
#define DMA2_S4CR   (*(volatile uint32_t*)(DMA2_BASE + 0x10 + 0x18*(4)))
#define DMA2_S5CR   (*(volatile uint32_t*)(DMA2_BASE + 0x10 + 0x18*(5)))
#define DMA2_S6CR   (*(volatile uint32_t*)(DMA2_BASE + 0x10 + 0x18*(6)))
#define DMA2_S7CR   (*(volatile uint32_t*)(DMA2_BASE + 0x10 + 0x18*(7)))

#define DMA_SCR_EN   (1<<0)
#define DMA_SCR_DMEIE   (1<<1)
#define DMA_SCR_TEIE   (1<<2)
#define DMA_SCR_HTIE   (1<<3)
#define DMA_SCR_TCIE   (1<<4)
#define DMA_SCR_PFCTRL   (1<<5)

#define DMA_SCR_DIR_P2M         (0<<6)
#define DMA_SCR_DIR_M2P         (1<<6)
#define DMA_SCR_DIR_M2M         (2<<6)
#define DMA_SCR_DIR_CLEARMASK   (3<<6)

#define DMA_SCR_CIRC   (1<<8)
#define DMA_SCR_PINC   (1<<9)
#define DMA_SCR_MINC   (1<<10)

#define DMA_SCR_PSIZE_8  (0<<11)
#define DMA_SCR_PSIZE_16 (1<<11)
#define DMA_SCR_PSIZE_32 (2<<11)
#define DMA_SCR_PSIZE_CLEARMASK (3<<11)

#define DMA_SCR_MSIZE_8  (0<<13)
#define DMA_SCR_MSIZE_16 (1<<13)
#define DMA_SCR_MSIZE_32 (2<<13)
#define DMA_SCR_MSIZE_CLEARMASK (3<<13)

#define DMA_SCR_PINCOS   (1<<15)

#define DMA_SCR_PL_LOW   (0<<16)
#define DMA_SCR_PL_MED   (1<<16)
#define DMA_SCR_PL_HI    (2<<16)
#define DMA_SCR_PL_VHI   (3<<16)
#define DMA_SCR_PL_CLEARMASK   (3<<16)


#define DMA_SCR_DBM   (1<<18)
#define DMA_SCR_CT    (1<<19)

#define DMA_SCR_PBURST_SINGLE      (0<<21)
#define DMA_SCR_PBURST_INCR4       (1<<21)
#define DMA_SCR_PBURST_INCR8       (2<<21)
#define DMA_SCR_PBURST_INCR16      (3<<21)
#define DMA_SCR_PBURST_CLEARMASK   (3<<21)

#define DMA_SCR_MBURST_SINGLE      (0<<23)
#define DMA_SCR_MBURST_INCR4       (1<<23)
#define DMA_SCR_MBURST_INCR8       (2<<23)
#define DMA_SCR_MBURST_INCR16      (3<<23)
#define DMA_SCR_MBURST_CLEARMASK   (3<<23)

#define DMA_SCR_CHSEL_OFFSET   25
#define DMA_SCR_CHSEL_MASK     7


//data register
#define DMA1_S0NDTR   (*(volatile uint32_t*)(DMA1_BASE + 0x14 + 0x18*(0)))
#define DMA1_S1NDTR   (*(volatile uint32_t*)(DMA1_BASE + 0x14 + 0x18*(1)))
#define DMA1_S2NDTR   (*(volatile uint32_t*)(DMA1_BASE + 0x14 + 0x18*(2)))
#define DMA1_S3NDTR   (*(volatile uint32_t*)(DMA1_BASE + 0x14 + 0x18*(3)))
#define DMA1_S4NDTR   (*(volatile uint32_t*)(DMA1_BASE + 0x14 + 0x18*(4)))
#define DMA1_S5NDTR   (*(volatile uint32_t*)(DMA1_BASE + 0x14 + 0x18*(5)))
#define DMA1_S6NDTR   (*(volatile uint32_t*)(DMA1_BASE + 0x14 + 0x18*(6)))
#define DMA1_S7NDTR   (*(volatile uint32_t*)(DMA1_BASE + 0x14 + 0x18*(7)))
#define DMA2_S0NDTR   (*(volatile uint32_t*)(DMA2_BASE + 0x14 + 0x18*(0)))
#define DMA2_S1NDTR   (*(volatile uint32_t*)(DMA2_BASE + 0x14 + 0x18*(1)))
#define DMA2_S2NDTR   (*(volatile uint32_t*)(DMA2_BASE + 0x14 + 0x18*(2)))
#define DMA2_S3NDTR   (*(volatile uint32_t*)(DMA2_BASE + 0x14 + 0x18*(3)))
#define DMA2_S4NDTR   (*(volatile uint32_t*)(DMA2_BASE + 0x14 + 0x18*(4)))
#define DMA2_S5NDTR   (*(volatile uint32_t*)(DMA2_BASE + 0x14 + 0x18*(5)))
#define DMA2_S6NDTR   (*(volatile uint32_t*)(DMA2_BASE + 0x14 + 0x18*(6)))
#define DMA2_S7NDTR   (*(volatile uint32_t*)(DMA2_BASE + 0x14 + 0x18*(7)))

//peripheral address register
#define DMA1_S0PAR   (*(volatile uint32_t*)(DMA1_BASE + 0x18 + 0x18*(0)))
#define DMA1_S1PAR   (*(volatile uint32_t*)(DMA1_BASE + 0x18 + 0x18*(1)))
#define DMA1_S2PAR   (*(volatile uint32_t*)(DMA1_BASE + 0x18 + 0x18*(2)))
#define DMA1_S3PAR   (*(volatile uint32_t*)(DMA1_BASE + 0x18 + 0x18*(3)))
#define DMA1_S4PAR   (*(volatile uint32_t*)(DMA1_BASE + 0x18 + 0x18*(4)))
#define DMA1_S5PAR   (*(volatile uint32_t*)(DMA1_BASE + 0x18 + 0x18*(5)))
#define DMA1_S6PAR   (*(volatile uint32_t*)(DMA1_BASE + 0x18 + 0x18*(6)))
#define DMA1_S7PAR   (*(volatile uint32_t*)(DMA1_BASE + 0x18 + 0x18*(7)))
#define DMA2_S0PAR   (*(volatile uint32_t*)(DMA2_BASE + 0x18 + 0x18*(0)))
#define DMA2_S1PAR   (*(volatile uint32_t*)(DMA2_BASE + 0x18 + 0x18*(1)))
#define DMA2_S2PAR   (*(volatile uint32_t*)(DMA2_BASE + 0x18 + 0x18*(2)))
#define DMA2_S3PAR   (*(volatile uint32_t*)(DMA2_BASE + 0x18 + 0x18*(3)))
#define DMA2_S4PAR   (*(volatile uint32_t*)(DMA2_BASE + 0x18 + 0x18*(4)))
#define DMA2_S5PAR   (*(volatile uint32_t*)(DMA2_BASE + 0x18 + 0x18*(5)))
#define DMA2_S6PAR   (*(volatile uint32_t*)(DMA2_BASE + 0x18 + 0x18*(6)))
#define DMA2_S7PAR   (*(volatile uint32_t*)(DMA2_BASE + 0x18 + 0x18*(7)))

//memory0 address register
#define DMA1_S0M0AR   (*(volatile uint32_t*)(DMA1_BASE + 0x1c + 0x18*(0)))
#define DMA1_S1M0AR   (*(volatile uint32_t*)(DMA1_BASE + 0x1c + 0x18*(1)))
#define DMA1_S2M0AR   (*(volatile uint32_t*)(DMA1_BASE + 0x1c + 0x18*(2)))
#define DMA1_S3M0AR   (*(volatile uint32_t*)(DMA1_BASE + 0x1c + 0x18*(3)))
#define DMA1_S4M0AR   (*(volatile uint32_t*)(DMA1_BASE + 0x1c + 0x18*(4)))
#define DMA1_S5M0AR   (*(volatile uint32_t*)(DMA1_BASE + 0x1c + 0x18*(5)))
#define DMA1_S6M0AR   (*(volatile uint32_t*)(DMA1_BASE + 0x1c + 0x18*(6)))
#define DMA1_S7M0AR   (*(volatile uint32_t*)(DMA1_BASE + 0x1c + 0x18*(7)))
#define DMA2_S0M0AR   (*(volatile uint32_t*)(DMA2_BASE + 0x1c + 0x18*(0)))
#define DMA2_S1M0AR   (*(volatile uint32_t*)(DMA2_BASE + 0x1c + 0x18*(1)))
#define DMA2_S2M0AR   (*(volatile uint32_t*)(DMA2_BASE + 0x1c + 0x18*(2)))
#define DMA2_S3M0AR   (*(volatile uint32_t*)(DMA2_BASE + 0x1c + 0x18*(3)))
#define DMA2_S4M0AR   (*(volatile uint32_t*)(DMA2_BASE + 0x1c + 0x18*(4)))
#define DMA2_S5M0AR   (*(volatile uint32_t*)(DMA2_BASE + 0x1c + 0x18*(5)))
#define DMA2_S6M0AR   (*(volatile uint32_t*)(DMA2_BASE + 0x1c + 0x18*(6)))
#define DMA2_S7M0AR   (*(volatile uint32_t*)(DMA2_BASE + 0x1c + 0x18*(7)))

//memory1 address register
#define DMA1_S0M1AR   (*(volatile uint32_t*)(DMA1_BASE + 0x20 + 0x18*(0)))
#define DMA1_S1M1AR   (*(volatile uint32_t*)(DMA1_BASE + 0x20 + 0x18*(1)))
#define DMA1_S2M1AR   (*(volatile uint32_t*)(DMA1_BASE + 0x20 + 0x18*(2)))
#define DMA1_S3M1AR   (*(volatile uint32_t*)(DMA1_BASE + 0x20 + 0x18*(3)))
#define DMA1_S4M1AR   (*(volatile uint32_t*)(DMA1_BASE + 0x20 + 0x18*(4)))
#define DMA1_S5M1AR   (*(volatile uint32_t*)(DMA1_BASE + 0x20 + 0x18*(5)))
#define DMA1_S6M1AR   (*(volatile uint32_t*)(DMA1_BASE + 0x20 + 0x18*(6)))
#define DMA1_S7M1AR   (*(volatile uint32_t*)(DMA1_BASE + 0x20 + 0x18*(7)))
#define DMA2_S0M1AR   (*(volatile uint32_t*)(DMA2_BASE + 0x20 + 0x18*(0)))
#define DMA2_S1M1AR   (*(volatile uint32_t*)(DMA2_BASE + 0x20 + 0x18*(1)))
#define DMA2_S2M1AR   (*(volatile uint32_t*)(DMA2_BASE + 0x20 + 0x18*(2)))
#define DMA2_S3M1AR   (*(volatile uint32_t*)(DMA2_BASE + 0x20 + 0x18*(3)))
#define DMA2_S4M1AR   (*(volatile uint32_t*)(DMA2_BASE + 0x20 + 0x18*(4)))
#define DMA2_S5M1AR   (*(volatile uint32_t*)(DMA2_BASE + 0x20 + 0x18*(5)))
#define DMA2_S6M1AR   (*(volatile uint32_t*)(DMA2_BASE + 0x20 + 0x18*(6)))
#define DMA2_S7M1AR   (*(volatile uint32_t*)(DMA2_BASE + 0x20 + 0x18*(7)))

//FIFO
#define DMA1_S0FCR   (*(volatile uint32_t*)(DMA1_BASE + 0x24 + 0x18*(0)))
#define DMA1_S1FCR   (*(volatile uint32_t*)(DMA1_BASE + 0x24 + 0x18*(1)))
#define DMA1_S2FCR   (*(volatile uint32_t*)(DMA1_BASE + 0x24 + 0x18*(2)))
#define DMA1_S3FCR   (*(volatile uint32_t*)(DMA1_BASE + 0x24 + 0x18*(3)))
#define DMA1_S4FCR   (*(volatile uint32_t*)(DMA1_BASE + 0x24 + 0x18*(4)))
#define DMA1_S5FCR   (*(volatile uint32_t*)(DMA1_BASE + 0x24 + 0x18*(5)))
#define DMA1_S6FCR   (*(volatile uint32_t*)(DMA1_BASE + 0x24 + 0x18*(6)))
#define DMA1_S7FCR   (*(volatile uint32_t*)(DMA1_BASE + 0x24 + 0x18*(7)))
#define DMA2_S0FCR   (*(volatile uint32_t*)(DMA2_BASE + 0x24 + 0x18*(0)))
#define DMA2_S1FCR   (*(volatile uint32_t*)(DMA2_BASE + 0x24 + 0x18*(1)))
#define DMA2_S2FCR   (*(volatile uint32_t*)(DMA2_BASE + 0x24 + 0x18*(2)))
#define DMA2_S3FCR   (*(volatile uint32_t*)(DMA2_BASE + 0x24 + 0x18*(3)))
#define DMA2_S4FCR   (*(volatile uint32_t*)(DMA2_BASE + 0x24 + 0x18*(4)))
#define DMA2_S5FCR   (*(volatile uint32_t*)(DMA2_BASE + 0x24 + 0x18*(5)))
#define DMA2_S6FCR   (*(volatile uint32_t*)(DMA2_BASE + 0x24 + 0x18*(6)))
#define DMA2_S7FCR   (*(volatile uint32_t*)(DMA2_BASE + 0x24 + 0x18*(7)))

#define DMA_SFCR_FTH_1_4       (0<<0)
#define DMA_SFCR_FTH_1_2       (1<<0)
#define DMA_SFCR_FTH_3_4       (2<<0)
#define DMA_SFCR_FTH_FULL      (3<<0)
#define DMA_SFCR_FTH_CLEARMASK (3<<0)

#define DMA_SFCR_DMDIS (1<<2)

#define DMA_SFCR_FS_1_4       (0<<3)
#define DMA_SFCR_FS_1_2       (1<<3)
#define DMA_SFCR_FS_3_4       (2<<3)
#define DMA_SFCR_FS_FULL      (3<<3)
#define DMA_SFCR_FS_CLEARMASK  (3<<3)

#define DMA_SFCR_FEIE (1<<7)

typedef enum
{
	DMA_1,
	DMA_2
}DMA_N;

typedef enum
{
    DMA_STREAM_0 = 0,
    DMA_STREAM_1 = 1,
    DMA_STREAM_2 = 2,
    DMA_STREAM_3 = 3,
    DMA_STREAM_4 = 4,
    DMA_STREAM_5 = 5,
    DMA_STREAM_6 = 6,
    DMA_STREAM_7 = 7

}DMA_STREAM_N;


typedef enum
{
    DMA_CHANNEL_0 = 0,
    DMA_CHANNEL_1 = 1,
    DMA_CHANNEL_2 = 2,
    DMA_CHANNEL_3 = 3,
    DMA_CHANNEL_4 = 4,
    DMA_CHANNEL_5 = 5,
    DMA_CHANNEL_6 = 6,
    DMA_CHANNEL_7 = 7

} DMA_CHANNEL;



typedef struct _DMA_STREAM
{
	volatile uint32_t* _pDMA_ISR;
	volatile uint32_t* _pDMA_IFCR;

	volatile uint32_t* _pDMA_SCR;
	volatile uint32_t* _pDMA_SNDTR;
	volatile uint32_t* _pDMA_SPAR;
	volatile uint32_t* _pDMA_SM0AR;
	volatile uint32_t* _pDMA_SM1AR;
	volatile uint32_t* _pDMA_SFCR;

	uint32_t  _FEIF;
	uint32_t  _DMEIF;
	uint32_t  _TEIF;
	uint32_t  _HTIF;
	uint32_t  _TCIF;

	IRQn_Type _nvicIrqNumber;

	uint8_t  _streamNumber;
	uint8_t  _channelNumber;
} DMA_Stream_Type;


typedef struct _DMA
{
	volatile uint32_t*  enableReg;
	volatile uint32_t*  resetReg;
	uint32_t             rccEnableBit;
    IRQn_Type*          _irqNumbers;
    DMA_Stream_Type     _dmaStreams[8];
} DMA_Type;


DMA_Type* DMA_GetInstance (DMA_N dmaNumber);
DMA_Stream_Type* DMA_GetStream(DMA_Type* dma, DMA_STREAM_N streamNo);


void DMA_Stream_EnableStream(DMA_Stream_Type* stream, BOOL enable);

void DMA_Stream_EnableDirectModeErrorInterrupt(DMA_Stream_Type* stream, BOOL enable);
void DMA_Stream_EnableTransferErrorInterrupt(DMA_Stream_Type* stream, BOOL enable);
void DMA_Stream_EnableHalfTransferInterrupt(DMA_Stream_Type* stream, BOOL enable);
void DMA_Stream_EnableTransferCompleteInterrupt(DMA_Stream_Type* stream, BOOL enable);
void DMA_Stream_EnableFIFOErrorInterrupt(DMA_Stream_Type* stream, BOOL enable);

BOOL DMA_Stream_IsDirectModeError(DMA_Stream_Type* stream);
BOOL DMA_Stream_IsTransferComplete(DMA_Stream_Type* stream);
BOOL DMA_Stream_IsHalfTransferComplete(DMA_Stream_Type* stream);
BOOL DMA_Stream_IsTransferError(DMA_Stream_Type* stream);
BOOL DMA_Stream_IsFIFOError(DMA_Stream_Type* stream);

void DMA_Stream_ClearDirectModeError(DMA_Stream_Type* stream);
void DMA_Stream_ClearTransferComplete(DMA_Stream_Type* stream);
void DMA_Stream_ClearHalfTransferComplete(DMA_Stream_Type* stream);
void DMA_Stream_ClearTransferError(DMA_Stream_Type* stream);
void DMA_Stream_ClearFIFOError(DMA_Stream_Type* stream);


void DMA_Stream_SetChannel (DMA_Stream_Type* stream, DMA_CHANNEL channel);
void DMA_Stream_SetPFCTR (DMA_Stream_Type* stream, BOOL set);

void DMA_Stream_SetDirection_MemoryToPeripheral(DMA_Stream_Type* stream);
void DMA_Stream_SetDirection_PeripheralToMemory(DMA_Stream_Type* stream);
void DMA_Stream_SetDirection_MemoryToMemory(DMA_Stream_Type* stream);
void DMA_Stream_SetCircularMode(DMA_Stream_Type* stream, BOOL set);
void DMA_Stream_SetMemoryIncrementMode(DMA_Stream_Type* stream, BOOL set);
void DMA_Stream_SetPeripheralIncrementMode(DMA_Stream_Type* stream, BOOL set);
void DMA_Stream_SetPeripheralSize_8bits(DMA_Stream_Type* stream);
void DMA_Stream_SetPeripheralSize_16bits(DMA_Stream_Type* stream);
void DMA_Stream_SetPeripheralSize_32bits(DMA_Stream_Type* stream);
void DMA_Stream_SetMemorySize_8bits(DMA_Stream_Type* stream);
void DMA_Stream_SetMemorySize_16bits(DMA_Stream_Type* stream);
void DMA_Stream_SetMemorySize_32bits(DMA_Stream_Type* stream);
void DMA_Stream_SetPeripheralIncOffsetSize(DMA_Stream_Type* stream, BOOL set);
void DMA_Stream_SetPriorityLow(DMA_Stream_Type* stream);
void DMA_Stream_SetPriorityMedium(DMA_Stream_Type* stream);
void DMA_Stream_SetPriorityHigh(DMA_Stream_Type* stream);
void DMA_Stream_SetPriorityVeryHigh(DMA_Stream_Type* stream);
void DMA_Stream_EnableDoubleBufferMode (DMA_Stream_Type* stream, BOOL enable);
void DMA_Stream_SetPBURST_INCR4(DMA_Stream_Type* stream);
void DMA_Stream_SetPBURST_INCR8(DMA_Stream_Type* stream);
void DMA_Stream_SetPBURST_INCR16(DMA_Stream_Type* stream);
void DMA_Stream_SetMBURST_INCR4(DMA_Stream_Type* stream);
void DMA_Stream_SetMBURST_INCR8(DMA_Stream_Type* stream);
void DMA_Stream_SetMBURST_INCR16(DMA_Stream_Type* stream);


void DMA_Stream_SetPeripheralAddress(DMA_Stream_Type* stream, uint32_t addr);
void DMA_Stream_SetMemory0Address(DMA_Stream_Type* stream, uint32_t addr);
void DMA_Stream_SetMemory1Address(DMA_Stream_Type* stream, uint32_t addr);
void DMA_Stream_SetNumberOfData(DMA_Stream_Type* stream, uint16_t);




#endif
