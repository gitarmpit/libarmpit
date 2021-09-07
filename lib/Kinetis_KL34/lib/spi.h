#ifndef _SPI_H
#define _SPI_H 

#include "common.h"
#include "nvic.h"

#ifndef SPI0_BASE
#define SPI0_BASE     0x40076000
#endif

#ifndef SPI1_BASE
#define SPI1_BASE     0x40077000
#endif

#define SPI0_S   (*(volatile uint8_t*)(SPI0_BASE + 0x0)) // Status
#define SPI0_BR  (*(volatile uint8_t*)(SPI0_BASE + 0x1)) // Baud Rate
#define SPI0_C2  (*(volatile uint8_t*)(SPI0_BASE + 0x2)) // Control2
#define SPI0_C1  (*(volatile uint8_t*)(SPI0_BASE + 0x3)) // Control1
#define SPI0_ML  (*(volatile uint8_t*)(SPI0_BASE + 0x4)) // Match, low
#define SPI0_MH  (*(volatile uint8_t*)(SPI0_BASE + 0x5)) // Match, high 
#define SPI0_DL  (*(volatile uint8_t*)(SPI0_BASE + 0x6)) // Data, low 
#define SPI0_DH  (*(volatile uint8_t*)(SPI0_BASE + 0x7)) // Data, high 
#define SPI0_CI  (*(volatile uint8_t*)(SPI0_BASE + 0xA)) // Clear Interrupt 
#define SPI0_C3  (*(volatile uint8_t*)(SPI0_BASE + 0xB)) // Control3

#define SPI1_S   (*(volatile uint8_t*)(SPI1_BASE + 0x0)) // Status
#define SPI1_BR  (*(volatile uint8_t*)(SPI1_BASE + 0x1)) // Baud Rate
#define SPI1_C2  (*(volatile uint8_t*)(SPI1_BASE + 0x2)) // Control2
#define SPI1_C1  (*(volatile uint8_t*)(SPI1_BASE + 0x3)) // Control1
#define SPI1_ML  (*(volatile uint8_t*)(SPI1_BASE + 0x4)) // Match, low
#define SPI1_MH  (*(volatile uint8_t*)(SPI1_BASE + 0x5)) // Match, high 
#define SPI1_DL  (*(volatile uint8_t*)(SPI1_BASE + 0x6)) // Data, low 
#define SPI1_DH  (*(volatile uint8_t*)(SPI1_BASE + 0x7)) // Data, high 
#define SPI1_CI  (*(volatile uint8_t*)(SPI1_BASE + 0xA)) // Clear Interrupt: FIFO 
#define SPI1_C3  (*(volatile uint8_t*)(SPI1_BASE + 0xB)) // Control3: FIFO

#define SPI_S_SPRF  (1<<7)  // SPI Read Buffer Full Flag
#define SPI_S_SPMF  (1<<6)  // Match flag
#define SPI_S_SPTEF (1<<5)  // SPI Transmit Buffer Empty Flag 
#define SPI_S_MODF  (1<<4)  // Master Mode Fault Flag
// FIFO:
#define SPI_S_RNFULLF     (1<<3)
#define SPI_S_TNEAREF     (1<<2)
#define SPI_S_TXFULLF     (1<<1)
#define SPI_S_RFIFOEF     (1<<0)


// Baud rate, prescaler divisor 0-7 (div 1-8):
#define SPI_BR_SPPR_BIT 4
#define SPI_BR_SPPR_CLEARMASK (3<<SPI_BR_SPPR_BIT)
// Baud rate divisor 0-7 (div 2/4/8/16/32/64/128/256/512)
#define SPI_BR_SPR_BIT 0
#define SPI_BR_SPR_CLEARMASK (3<<SPI_BR_SPR_BIT)

// Control2
#define SPI_C2_SPMIE   (1<<7) // Match Interrupt Enable
#define SPI_C2_SPIMODE (1<<6) // SPI mode: 0=8-bit, 1=16-bit
#define SPI_C2_TXDMAE  (1<<5) // Transmit DMA enable
#define SPI_C2_MODFEN  (1<<4) // Master Mode-Fault Function Enable (SS pin)
#define SPI_C2_BIDIROE (1<<3) // For bidirictional cfg (SPC0)
#define SPI_C2_RXDMAE  (1<<2) // Receive DMA enable
#define SPI_C2_SPISWAI (1<<1) // SPI Stop in Wait Mode
#define SPI_C2_SPC0    (1<<0) // 0: separate MOSI MISO  1: Single wire bidirectional

// Control1
#define SPI_C1_SPIE   (1<<7) // SPI Interrupt Enable for SPRF/MODF
#define SPI_C1_SPE    (1<<6) // SPI System Enable
#define SPI_C1_SPTIE  (1<<5) // SPI Transmit Interrupt Enable  (SPTEF)
#define SPI_C1_MSTR   (1<<4) // 0: slave, 1: master
#define SPI_C1_CPOL   (1<<3) // Clock Polarity. 0: Active high, 1: Active low
#define SPI_C1_CPHA   (1<<2) // Clock Phase: First edge on SPSCK 0: middle, 1: start  of the first cycle of a data transfer.
#define SPI_C1_SSOE   (1<<1) // Slave Select Output Enable. SS pin. 
#define SPI_C1_LSBFE  (1<<0) // 0: MSB, 1: LSB

typedef enum
{
    SPI0 = 0,
    SPI1 = 1,
} SPI_N;

typedef enum
{
    SPI_DIV2   = 0,
    SPI_DIV4   = 1,
    SPI_DIV8   = 2,
    SPI_DIV16  = 3,
    SPI_DIV32  = 4,
    SPI_DIV64  = 5,
    SPI_DIV128 = 6,
    SPI_DIV256 = 7,
    SPI_DIV512 = 8,

} SPI_BRDIV;

typedef struct _SPI
{
    volatile uint32_t* SIM_enableReg;
    uint32_t           SIM_enableMask;

    volatile uint8_t*  SPI_S;
    volatile uint8_t*  SPI_BR;
    volatile uint8_t*  SPI_C2;
    volatile uint8_t*  SPI_C1;
    volatile uint8_t*  SPI_DL;
    volatile uint8_t*  SPI_DH;

    IRQn_Type          irq;
    void               (*handler)(struct _SPI*);

} SPI;

#ifdef __cplusplus
extern "C"
{
#endif

SPI*    SPI_GetInstance(SPI_N n);
void    SPI_Enable(SPI* spi, BOOL enable);
void    SPI_Initialize(SPI* spi, BOOL isMaster, uint8_t presc, SPI_BRDIV div, BOOL isPhaseStart, BOOL isPolarityLow, BOOL isLSB);
uint8_t SPI_TransmitByte (SPI* spi, uint8_t byte);
uint8_t SPI_TransmitRegister (SPI* spi, uint8_t reg, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif
