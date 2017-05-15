#ifndef _DMA_H
#define _DMA_H 

#define DMA1 0 
#define DMA2 1 


#if defined(__GNUC__) && defined(__ASSEMBLY__)

#define DMA1_BASE 0x40020000  //AHB
#define DMA2_BASE 0x40020400  //AHB, HD

//interrupt status register
#define DMA1_ISR    DMA1_BASE
#define DMA1_IFCR   DMA1_BASE + 0x4

//configuration register
#define DMA1_CCR1   DMA1_BASE + 0x08 + 20*(1-1)
#define DMA1_CCR2   DMA1_BASE + 0x08 + 20*(2-1)
#define DMA1_CCR3   DMA1_BASE + 0x08 + 20*(3-1)
#define DMA1_CCR4   DMA1_BASE + 0x08 + 20*(4-1)
#define DMA1_CCR5   DMA1_BASE + 0x08 + 20*(5-1)
#define DMA1_CCR6   DMA1_BASE + 0x08 + 20*(6-1)
#define DMA1_CCR7   DMA1_BASE + 0x08 + 20*(7-1)

//data register
#define DMA1_CNDTR1   DMA1_BASE + 0x0c + 20*(1-1)
#define DMA1_CNDTR2   DMA1_BASE + 0x0c + 20*(2-1)
#define DMA1_CNDTR3   DMA1_BASE + 0x0c + 20*(3-1)
#define DMA1_CNDTR4   DMA1_BASE + 0x0c + 20*(4-1)
#define DMA1_CNDTR5   DMA1_BASE + 0x0c + 20*(5-1)
#define DMA1_CNDTR6   DMA1_BASE + 0x0c + 20*(6-1)
#define DMA1_CNDTR7   DMA1_BASE + 0x0c + 20*(7-1)

//peripheral address register
#define DMA1_CPAR1   DMA1_BASE + 0x10 + 20*(1-1)
#define DMA1_CPAR2   DMA1_BASE + 0x10 + 20*(2-1)
#define DMA1_CPAR3   DMA1_BASE + 0x10 + 20*(3-1)
#define DMA1_CPAR4   DMA1_BASE + 0x10 + 20*(4-1)
#define DMA1_CPAR5   DMA1_BASE + 0x10 + 20*(5-1)
#define DMA1_CPAR6   DMA1_BASE + 0x10 + 20*(6-1)
#define DMA1_CPAR7   DMA1_BASE + 0x10 + 20*(7-1)

#define DMA1_CMAR1   DMA1_BASE + 0x14 + 20*(1-1)
#define DMA1_CMAR2   DMA1_BASE + 0x14 + 20*(2-1)
#define DMA1_CMAR3   DMA1_BASE + 0x14 + 20*(3-1)
#define DMA1_CMAR4   DMA1_BASE + 0x14 + 20*(4-1)
#define DMA1_CMAR5   DMA1_BASE + 0x14 + 20*(5-1)
#define DMA1_CMAR6   DMA1_BASE + 0x14 + 20*(6-1)
#define DMA1_CMAR7   DMA1_BASE + 0x14 + 20*(7-1)

#define DMA2_ISR    DMA2_BASE
#define DMA2_IFCR   DMA2_BASE + 0x4

//configuration register
#define DMA2_CCR1   DMA2_BASE + 0x08 + 20*(1-1)
#define DMA2_CCR2   DMA2_BASE + 0x08 + 20*(2-1)
#define DMA2_CCR3   DMA2_BASE + 0x08 + 20*(3-1)
#define DMA2_CCR4   DMA2_BASE + 0x08 + 20*(4-1)
#define DMA2_CCR5   DMA2_BASE + 0x08 + 20*(5-1)
#define DMA2_CCR6   DMA2_BASE + 0x08 + 20*(6-1)
#define DMA2_CCR7   DMA2_BASE + 0x08 + 20*(7-1)

//data register
#define DMA2_CNDTR1   DMA2_BASE + 0x0c + 20*(1-1)
#define DMA2_CNDTR2   DMA2_BASE + 0x0c + 20*(2-1)
#define DMA2_CNDTR3   DMA2_BASE + 0x0c + 20*(3-1)
#define DMA2_CNDTR4   DMA2_BASE + 0x0c + 20*(4-1)
#define DMA2_CNDTR5   DMA2_BASE + 0x0c + 20*(5-1)
#define DMA2_CNDTR6   DMA2_BASE + 0x0c + 20*(6-1)
#define DMA2_CNDTR7   DMA2_BASE + 0x0c + 20*(7-1)

//peripheral address register
#define DMA2_CPAR1   DMA2_BASE + 0x10 + 20*(1-1)
#define DMA2_CPAR2   DMA2_BASE + 0x10 + 20*(2-1)
#define DMA2_CPAR3   DMA2_BASE + 0x10 + 20*(3-1)
#define DMA2_CPAR4   DMA2_BASE + 0x10 + 20*(4-1)
#define DMA2_CPAR5   DMA2_BASE + 0x10 + 20*(5-1)
#define DMA2_CPAR6   DMA2_BASE + 0x10 + 20*(6-1)
#define DMA2_CPAR7   DMA2_BASE + 0x10 + 20*(7-1)

#define DMA2_CMAR1   DMA2_BASE + 0x14 + 20*(1-1)
#define DMA2_CMAR2   DMA2_BASE + 0x14 + 20*(2-1)
#define DMA2_CMAR3   DMA2_BASE + 0x14 + 20*(3-1)
#define DMA2_CMAR4   DMA2_BASE + 0x14 + 20*(4-1)
#define DMA2_CMAR5   DMA2_BASE + 0x14 + 20*(5-1)
#define DMA2_CMAR6   DMA2_BASE + 0x14 + 20*(6-1)
#define DMA2_CMAR7   DMA2_BASE + 0x14 + 20*(7-1)

#define DMA_ISR_GIF1  (1<<0)
#define DMA_ISR_TCIF1 (1<<1)
#define DMA_ISR_HTIF1 (1<<2)
#define DMA_ISR_TEIF1 (1<<3)
#define DMA_ISR_GIF2  (1<<4)
#define DMA_ISR_TCIF2 (1<<5)
#define DMA_ISR_HTIF2 (1<<6)
#define DMA_ISR_TEIF2 (1<<7)
#define DMA_ISR_GIF3  (1<<8)
#define DMA_ISR_TCIF3 (1<<9)
#define DMA_ISR_HTIF3 (1<<10)
#define DMA_ISR_TEIF3 (1<<11)
#define DMA_ISR_GIF4  (1<<12)
#define DMA_ISR_TCIF4 (1<<13)
#define DMA_ISR_HTIF4 (1<<14)
#define DMA_ISR_TEIF4 (1<<15)
#define DMA_ISR_GIF5  (1<<16)
#define DMA_ISR_TCIF5 (1<<17)
#define DMA_ISR_HTIF5 (1<<18)
#define DMA_ISR_TEIF5 (1<<19)
#define DMA_ISR_GIF6  (1<<20)
#define DMA_ISR_TCIF6 (1<<21)
#define DMA_ISR_HTIF6 (1<<22)
#define DMA_ISR_TEIF6 (1<<23)
#define DMA_ISR_GIF7  (1<<24)
#define DMA_ISR_TCIF7 (1<<25)
#define DMA_ISR_HTIF7 (1<<26)
#define DMA_ISR_TEIF7 (1<<27)

#define DMA_IFCR_CGIF1  (1<<0)
#define DMA_IFCR_CTCIF1 (1<<1)
#define DMA_IFCR_CHTIF1 (1<<2)
#define DMA_IFCR_CTEIF1 (1<<3)
#define DMA_IFCR_CGIF2  (1<<4)
#define DMA_IFCR_CTCIF2 (1<<5)
#define DMA_IFCR_CHTIF2 (1<<6)
#define DMA_IFCR_CTEIF2 (1<<7)
#define DMA_IFCR_CGIF3  (1<<8)
#define DMA_IFCR_CTCIF3 (1<<9)
#define DMA_IFCR_CHTIF3 (1<<10)
#define DMA_IFCR_CTEIF3 (1<<11)
#define DMA_IFCR_CGIF4  (1<<12)
#define DMA_IFCR_CTCIF4 (1<<13)
#define DMA_IFCR_CHTIF4 (1<<14)
#define DMA_IFCR_CTEIF4 (1<<15)
#define DMA_IFCR_CGIF5  (1<<16)
#define DMA_IFCR_CTCIF5 (1<<17)
#define DMA_IFCR_CHTIF5 (1<<18)
#define DMA_IFCR_CTEIF5 (1<<19)
#define DMA_IFCR_CGIF6  (1<<20)
#define DMA_IFCR_CTCIF6 (1<<21)
#define DMA_IFCR_CHTIF6 (1<<22)
#define DMA_IFCR_CTEIF6 (1<<23)
#define DMA_IFCR_CGIF7  (1<<24)
#define DMA_IFCR_CTCIF7 (1<<25)
#define DMA_IFCR_CHTIF7 (1<<26)
#define DMA_IFCR_CTEIF7 (1<<27)

#define DMA_CCR_EN   (1<<0)
#define DMA_CCR_TCIE (1<<1)
#define DMA_CCR_HTIE (1<<2)
#define DMA_CCR_TEIE (1<<3)
#define DMA_CCR_DIR  (1<<4)
#define DMA_CCR_CIRC (1<<5)
#define DMA_CCR_PINC (1<<6)
#define DMA_CCR_MINC (1<<7)
#define DMA_CCR_PSIZE_8  (0<<8)
#define DMA_CCR_PSIZE_16 (1<<8)
#define DMA_CCR_PSIZE_32 (2<<8)
#define DMA_CCR_MSIZE_8  (0<<10)
#define DMA_CCR_MSIZE_16 (1<<10)
#define DMA_CCR_MSIZE_32 (2<<10)
#define DMA_CCR_PL_LOW   (0<<12)
#define DMA_CCR_PL_MED   (1<<12)
#define DMA_CCR_PL_HI    (2<<12)
#define DMA_CCR_PL_VHI   (3<<12)
#define DMA_CCR_MEM2MEM  (1<<14)


#else 

#include "common.h"

typedef enum 
{
    DMA_CHANNEL_1 = 0,
    DMA_CHANNEL_2 = 1,
    DMA_CHANNEL_3 = 2,
    DMA_CHANNEL_4 = 3,
    DMA_CHANNEL_5 = 4,
    DMA_CHANNEL_6 = 5,
    DMA_CHANNEL_7 = 6

}DMA_CHANNEL;

typedef struct 
{
    uint32_t reg;
} DMA_CFG;

void DMA_init(uint8_t dma_no,  DMA_CFG* cfg);
void DMA_EnableChannel(DMA_CFG* cfg, uint8_t channel, bool enable);
void DMA_SetPeripheralAddress(DMA_CFG* cfg, DMA_CHANNEL channel, uint32_t addr);
void DMA_SetMemoryAddress(DMA_CFG* cfg, DMA_CHANNEL channel, uint32_t addr);
void DMA_SetPeripheralSize_8bits(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetPeripheralSize_16bits(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetPeripheralSize_32bits(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetMemorySize_8bits(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetMemorySize_16bits(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetMemorySize_32bits(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetNumberOfData(DMA_CFG* cfg, DMA_CHANNEL channel, uint16_t ndata);
void DMA_SetDirection_MemoryToPeripheral(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetDirection_PeripheralToMemory(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetDirection_MemoryToMemory(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetMemoryIncrementMode(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetPeripheralIncrementMode(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetCircularMode(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetPriorityLow(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetPriorityMedium(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetPriorityHigh(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_SetPriorityVeryHigh(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_EnableTransferErrorInterrupt(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_EnableHalfTransferInterrupt(DMA_CFG* cfg, DMA_CHANNEL channel);
void DMA_EnableTransferCompleteInterrupt(DMA_CFG* cfg, DMA_CHANNEL channel);


#endif
