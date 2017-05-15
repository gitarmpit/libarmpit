/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 igorS
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "dma.h"
#include "rcc.h"
#include "nvic.h"

#define ISR_POS(CHANNEL) ((CHANNEL%2) * 6  + (((CHANNEL%4) / 2)*16))

static IRQn_Type DMA1_nvicIrqNumbers[8] =
{ IRQn_DMA1_Stream0, IRQn_DMA1_Stream1, IRQn_DMA1_Stream2, IRQn_DMA1_Stream3,
        IRQn_DMA1_Stream4, IRQn_DMA1_Stream5, IRQn_DMA1_Stream6,
        IRQn_DMA1_Stream7 };

static IRQn_Type DMA2_nvicIrqNumbers[8] =
{ IRQn_DMA2_Stream0, IRQn_DMA2_Stream1, IRQn_DMA2_Stream2, IRQn_DMA2_Stream3,
        IRQn_DMA2_Stream4, IRQn_DMA2_Stream5, IRQn_DMA2_Stream6,
        IRQn_DMA2_Stream7 };

static DMA_Type DMA_list[] =
{
        { &RCC_AHB1ENR, &RCC_AHB1RSTR, RCC_AHB1ENR_DMA1EN, DMA1_nvicIrqNumbers, {{0}} },
        { &RCC_AHB1ENR, &RCC_AHB1RSTR, RCC_AHB1ENR_DMA2EN, DMA2_nvicIrqNumbers, {{0}} },
};

DMA_Type* DMA_GetInstance(DMA_N dmaNumber)
{
    DMA_Type* dma =  &DMA_list[(uint8_t) dmaNumber];
    volatile uint32_t* pDMAx_LISR;
    volatile uint32_t* pDMAx_HISR;
    volatile uint32_t* pDMAx_LIFCR;
    volatile uint32_t* pDMAx_HIFCR;

    volatile uint32_t* pDMAx_SCR;
    volatile uint32_t* pDMAx_SNDTR;
    volatile uint32_t* pDMAx_SPAR;
    volatile uint32_t* pDMAx_SM0AR;
    volatile uint32_t* pDMAx_SM1AR;
    volatile uint32_t* pDMAx_SFCR;

    if (dmaNumber == DMA_1)
    {
        pDMAx_LISR = &DMA1_LISR;
        pDMAx_HISR = &DMA1_HISR;
        pDMAx_LIFCR = &DMA1_LIFCR;
        pDMAx_HIFCR = &DMA1_HIFCR;
        pDMAx_SCR = &DMA1_S0CR;
        pDMAx_SNDTR = &DMA1_S0NDTR;
        pDMAx_SPAR = &DMA1_S0PAR;
        pDMAx_SM0AR = &DMA1_S0M0AR;
        pDMAx_SM1AR = &DMA1_S0M1AR;
        pDMAx_SFCR = &DMA1_S0FCR;
    }
    else
    {
        pDMAx_LISR = &DMA2_LISR;
        pDMAx_HISR = &DMA2_HISR;
        pDMAx_LIFCR = &DMA2_LIFCR;
        pDMAx_HIFCR = &DMA2_HIFCR;
        pDMAx_SCR = &DMA2_S0CR;
        pDMAx_SNDTR = &DMA2_S0NDTR;
        pDMAx_SPAR = &DMA2_S0PAR;
        pDMAx_SM0AR = &DMA2_S0M0AR;
        pDMAx_SM1AR = &DMA2_S0M1AR;
        pDMAx_SFCR = &DMA2_S0FCR;
    }

    for (uint8_t i = 0; i < 8; ++i)
    {
        dma->_dmaStreams[i]._streamNumber = i;
        if (i < 4 )
        {
            dma->_dmaStreams[i]._pDMA_ISR = pDMAx_LISR;
            dma->_dmaStreams[i]._pDMA_IFCR = pDMAx_LIFCR;
        }
        else
        {
            dma->_dmaStreams[i]._pDMA_ISR = pDMAx_HISR;
            dma->_dmaStreams[i]._pDMA_IFCR = pDMAx_HIFCR;
        }

        dma->_dmaStreams[i]._pDMA_SCR = (uint32_t*) ((uint32_t)pDMAx_SCR + (i * 0x18));
        dma->_dmaStreams[i]._pDMA_SNDTR = (uint32_t*) ((uint32_t)pDMAx_SNDTR + (i * 0x18));
        dma->_dmaStreams[i]._pDMA_SPAR = (uint32_t*) ((uint32_t)pDMAx_SPAR + (i * 0x18));
        dma->_dmaStreams[i]._pDMA_SM0AR = (uint32_t*) ((uint32_t)pDMAx_SM0AR + (i * 0x18));
        dma->_dmaStreams[i]._pDMA_SM1AR = (uint32_t*) ((uint32_t)pDMAx_SM1AR + (i * 0x18));
        dma->_dmaStreams[i]._pDMA_SFCR = (uint32_t*) ((uint32_t)pDMAx_SFCR + (i * 0x24));

        dma->_dmaStreams[i]._nvicIrqNumber = dma->_irqNumbers[i];

        dma->_dmaStreams[i]._FEIF = 1<<DMA_IFCR_FEIF0_OFFSET<<ISR_POS(i);
        dma->_dmaStreams[i]._DMEIF = 1<<DMA_IFCR_DMEIF0_OFFSET<<ISR_POS(i);
        dma->_dmaStreams[i]._TEIF = 1<<DMA_IFCR_TEIF0_OFFSET<<ISR_POS(i);
        dma->_dmaStreams[i]._HTIF = 1<<DMA_IFCR_HTIF0_OFFSET<<ISR_POS(i);
        dma->_dmaStreams[i]._TCIF = 1<<DMA_IFCR_TCIF0_OFFSET<<ISR_POS(i);

    }
    return dma;
}

DMA_Stream_Type* DMA_GetStream(DMA_Type* dma, DMA_STREAM_N streamNo)
{
    return &dma->_dmaStreams[streamNo];
}

void DMA_Stream_SetChannel(DMA_Stream_Type* stream, DMA_CHANNEL channel)
{
    *stream->_pDMA_SCR |= (channel << DMA_SCR_CHSEL_OFFSET);
}

void DMA_Stream_EnableStream(DMA_Stream_Type* stream, BOOL enable)
{
    if (enable)
    {
        *stream->_pDMA_SCR |= DMA_SCR_EN;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_EN;
    }
}

void DMA_Stream_EnableDirectModeErrorInterrupt(DMA_Stream_Type* stream, BOOL enable)
{
    if (enable)
    {
        *stream->_pDMA_SCR |= DMA_SCR_DMEIE;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_DMEIE;
    }
    NVIC_SetEnable(stream->_nvicIrqNumber);
}

BOOL DMA_Stream_IsDirectModeError(DMA_Stream_Type* stream)
{
    return *stream->_pDMA_ISR & stream->_DMEIF;
}

void DMA_Stream_ClearDirectModeError(DMA_Stream_Type* stream)
{
    *stream->_pDMA_IFCR &= ~stream->_DMEIF;
}

void DMA_Stream_EnableTransferErrorInterrupt(DMA_Stream_Type* stream, BOOL enable)
{
    if (enable)
    {
        *stream->_pDMA_SCR |= DMA_SCR_TEIE;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_TEIE;
    }
    NVIC_SetEnable(stream->_nvicIrqNumber);
}

BOOL DMA_Stream_IsTransferError(DMA_Stream_Type* stream)
{
    return *stream->_pDMA_ISR & stream->_TEIF;
}

void DMA_Stream_ClearTransferError(DMA_Stream_Type* stream)
{
    *stream->_pDMA_IFCR |= stream->_TEIF;
}

void DMA_Stream_EnableHalfTransferInterrupt(DMA_Stream_Type* stream, BOOL enable)
{
    if (enable)
    {
        *stream->_pDMA_SCR |= DMA_SCR_HTIE;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_HTIE;
    }
    NVIC_SetEnable(stream->_nvicIrqNumber);
}

BOOL DMA_Stream_IsHalfTransferComplete(DMA_Stream_Type* stream)
{
    return *stream->_pDMA_ISR & stream->_HTIF;
}

void DMA_Stream_ClearHalfTransferComplete(DMA_Stream_Type* stream)
{
    *stream->_pDMA_IFCR |= stream->_HTIF;
}

void DMA_Stream_EnableTransferCompleteInterrupt(DMA_Stream_Type* stream, BOOL enable)
{
    if (enable)
    {
        *stream->_pDMA_SCR |= DMA_SCR_TCIE;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_TCIE;
    }
    NVIC_SetEnable(stream->_nvicIrqNumber);
}

BOOL DMA_Stream_IsTransferComplete(DMA_Stream_Type* stream)
{
    return *stream->_pDMA_ISR & stream->_TCIF;
}

void DMA_Stream_ClearTransferComplete(DMA_Stream_Type* stream)
{
    *stream->_pDMA_IFCR |= stream->_TCIF;
}

void DMA_Stream_EnableFIFOErrorInterrupt(DMA_Stream_Type* stream, BOOL enable)
{
    if (enable)
    {
        *stream->_pDMA_SFCR |= DMA_SFCR_FEIE;
    }
    else
    {
        *stream->_pDMA_SFCR &= ~DMA_SFCR_FEIE;
    }
}

BOOL DMA_Stream_IsFIFOError(DMA_Stream_Type* stream)
{
    return *stream->_pDMA_ISR & stream->_FEIF;
}
void DMA_Stream_ClearFIFOError(DMA_Stream_Type* stream)
{
    *stream->_pDMA_IFCR |= stream->_FEIF;
}

void DMA_Stream_SetPFCTR(DMA_Stream_Type* stream, BOOL set)
{
    if (set)
    {
        *stream->_pDMA_SCR |= DMA_SCR_PFCTRL;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_PFCTRL;
    }
}

void DMA_Stream_SetDirection_MemoryToPeripheral(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR &= ~DMA_SCR_DIR_CLEARMASK;
    *stream->_pDMA_SCR |= DMA_SCR_DIR_M2P;
}
void DMA_Stream_SetDirection_PeripheralToMemory(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR &= ~DMA_SCR_DIR_CLEARMASK;
    *stream->_pDMA_SCR |= DMA_SCR_DIR_P2M;
}
void DMA_Stream_SetDirection_MemoryToMemory(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR &= ~DMA_SCR_DIR_CLEARMASK;
    *stream->_pDMA_SCR |= DMA_SCR_DIR_M2M;
}

void DMA_Stream_SetCircularMode(DMA_Stream_Type* stream, BOOL set)
{
    if (set)
    {
        *stream->_pDMA_SCR |= DMA_SCR_CIRC;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_CIRC;
    }
}

void DMA_Stream_SetMemoryIncrementMode(DMA_Stream_Type* stream, BOOL set)
{
    if (set)
    {
        *stream->_pDMA_SCR |= DMA_SCR_MINC;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_MINC;
    }
}
void DMA_Stream_SetPeripheralIncrementMode(DMA_Stream_Type* stream, BOOL set)
{
    if (set)
    {
        *stream->_pDMA_SCR |= DMA_SCR_PINC;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_PINC;
    }
}

void DMA_Stream_SetPeripheralSize_8bits(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR |= DMA_SCR_PSIZE_8;
}
void DMA_Stream_SetPeripheralSize_16bits(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR |= DMA_SCR_PSIZE_16;
}
void DMA_Stream_SetPeripheralSize_32bits(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR |= DMA_SCR_PSIZE_32;
}

void DMA_Stream_SetMemorySize_8bits(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR |= DMA_SCR_MSIZE_8;
}
void DMA_Stream_SetMemorySize_16bits(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR |= DMA_SCR_MSIZE_16;
}
void DMA_Stream_SetMemorySize_32bits(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR |= DMA_SCR_MSIZE_32;
}

void DMA_Stream_SetPeripheralIncOffsetSize(DMA_Stream_Type* stream, BOOL set)
{
    if (set)
    {
        *stream->_pDMA_SCR |= DMA_SCR_PINCOS;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_PINCOS;
    }
}

void DMA_Stream_SetPriorityLow(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR |= DMA_SCR_PL_LOW;
}
void DMA_Stream_SetPriorityMedium(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR |= DMA_SCR_PL_MED;
}
void DMA_Stream_SetPriorityHigh(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR |= DMA_SCR_PL_HI;

}
void DMA_Stream_SetPriorityVeryHigh(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR |= DMA_SCR_PL_VHI;
}

void DMA_Stream_EnableDoubleBufferMode(DMA_Stream_Type* stream, BOOL enable)
{
    if (enable)
    {
        *stream->_pDMA_SCR |= DMA_SCR_DBM;
    }
    else
    {
        *stream->_pDMA_SCR &= ~DMA_SCR_DBM;
    }
}

void DMA_Stream_SetPBURST_INCR4(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR &= ~DMA_SCR_PBURST_CLEARMASK;
    *stream->_pDMA_SCR |= DMA_SCR_PBURST_INCR4;
}

void DMA_Stream_SetPBURST_INCR8(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR &= ~DMA_SCR_PBURST_CLEARMASK;
    *stream->_pDMA_SCR |= DMA_SCR_PBURST_INCR8;
}

void DMA_Stream_SetPBURST_INCR16(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR &= ~DMA_SCR_PBURST_CLEARMASK;
    *stream->_pDMA_SCR |= DMA_SCR_PBURST_INCR16;
}

void DMA_Stream_SetMBURST_INCR4(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR &= ~DMA_SCR_MBURST_CLEARMASK;
    *stream->_pDMA_SCR |= DMA_SCR_MBURST_INCR4;
}

void DMA_Stream_SetMBURST_INCR8(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR &= ~DMA_SCR_MBURST_CLEARMASK;
    *stream->_pDMA_SCR |= DMA_SCR_MBURST_INCR8;
}

void DMA_Stream_SetMBURST_INCR16(DMA_Stream_Type* stream)
{
    *stream->_pDMA_SCR &= ~DMA_SCR_MBURST_CLEARMASK;
    *stream->_pDMA_SCR |= DMA_SCR_MBURST_INCR16;
}

////////////////////////////////////////////////////////

void DMA_Stream_SetPeripheralAddress(DMA_Stream_Type* stream, uint32_t addr)
{
    *stream->_pDMA_SPAR = addr;
}
void DMA_Stream_SetMemory0Address(DMA_Stream_Type* stream, uint32_t addr)
{
    *stream->_pDMA_SM0AR = addr;
}
void DMA_Stream_SetMemory1Address(DMA_Stream_Type* stream, uint32_t addr)
{
    *stream->_pDMA_SM0AR = addr;
}

void DMA_Stream_SetNumberOfData(DMA_Stream_Type* stream, uint16_t length)
{
    *stream->_pDMA_SNDTR = length;
}
