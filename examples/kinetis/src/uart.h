#ifndef _UART_H
#define _UART_H

#include "common.h"
#include "nvic.h"

#ifndef UART0_BASE
#define UART0_BASE     0x4006A000
#endif

#ifndef UART1_BASE
#define UART1_BASE     0x4006B000
#endif

#ifndef UART2_BASE
#define UART2_BASE     0x4006C000
#endif

#define UART0_BDH  (*(volatile uint8_t*)(UART0_BASE + 0x0)) 
#define UART0_BDL  (*(volatile uint8_t*)(UART0_BASE + 0x1)) 
#define UART0_C1   (*(volatile uint8_t*)(UART0_BASE + 0x2)) 
#define UART0_C2   (*(volatile uint8_t*)(UART0_BASE + 0x3)) 
#define UART0_S1   (*(volatile uint8_t*)(UART0_BASE + 0x4)) 
#define UART0_S2   (*(volatile uint8_t*)(UART0_BASE + 0x5)) 
#define UART0_C3   (*(volatile uint8_t*)(UART0_BASE + 0x6)) 
#define UART0_D    (*(volatile uint8_t*)(UART0_BASE + 0x7)) 
#define UART0_MA1  (*(volatile uint8_t*)(UART0_BASE + 0x8)) 
#define UART0_MA2  (*(volatile uint8_t*)(UART0_BASE + 0x9)) 
#define UART0_C4   (*(volatile uint8_t*)(UART0_BASE + 0xA)) 
#define UART0_C5   (*(volatile uint8_t*)(UART0_BASE + 0xB)) 

#define UART1_BDH  (*(volatile uint8_t*)(UART1_BASE + 0x0)) 
#define UART1_BDL  (*(volatile uint8_t*)(UART1_BASE + 0x1)) 
#define UART1_C1   (*(volatile uint8_t*)(UART1_BASE + 0x2)) 
#define UART1_C2   (*(volatile uint8_t*)(UART1_BASE + 0x3)) 
#define UART1_S1   (*(volatile uint8_t*)(UART1_BASE + 0x4)) 
#define UART1_S2   (*(volatile uint8_t*)(UART1_BASE + 0x5)) 
#define UART1_C3   (*(volatile uint8_t*)(UART1_BASE + 0x6)) 
#define UART1_D    (*(volatile uint8_t*)(UART1_BASE + 0x7)) 
#define UART1_C4   (*(volatile uint8_t*)(UART1_BASE + 0x8)) 

#define UART2_BDH  (*(volatile uint8_t*)(UART2_BASE + 0x0)) 
#define UART2_BDL  (*(volatile uint8_t*)(UART2_BASE + 0x1)) 
#define UART2_C1   (*(volatile uint8_t*)(UART2_BASE + 0x2)) 
#define UART2_C2   (*(volatile uint8_t*)(UART2_BASE + 0x3)) 
#define UART2_S1   (*(volatile uint8_t*)(UART2_BASE + 0x4)) 
#define UART2_S2   (*(volatile uint8_t*)(UART2_BASE + 0x5)) 
#define UART2_C3   (*(volatile uint8_t*)(UART2_BASE + 0x6)) 
#define UART2_D    (*(volatile uint8_t*)(UART2_BASE + 0x7)) 
#define UART2_C4   (*(volatile uint8_t*)(UART2_BASE + 0x8)) 

#define UART_BDH_SBNS (1<<5) // Stop Bit Number Select. 0: 1 stop bit, 1: 2 stop bits.

#define UART_C1_LOOPS      (1<<7) // Loop Mode Select. TX output is internally connected to RX input.
#define UART_C1_UARTSWAI   (1<<6) // 1: 1 UART clocks freeze while CPU is in wait mode
#define UART_C1_RSRC       (1<<5) // Used in LOOP mode
#define UART_C1_M          (1<<4) // 0: 8 bit mode  1: 9 bit mode
#define UART_C1_WAKE       (1<<3) // Receiver Wakeup Method Select
#define UART_C1_ILT        (1<<2) // Idle Line Type Select
#define UART_C1_PE         (1<<1) // Parity Enable
#define UART_C1_PT         (1<<0) // Parity Type: 0: Even 1: Odd

// C2 Interrupts
#define UART_C2_TIE   (1<<7) // Transmit Interrupt Enable for TDRE
#define UART_C2_TCIE  (1<<6) // Transmission Complete Interrupt Enable for TC
#define UART_C2_RIE   (1<<5) // Receiver Interrupt Enable for RDRF
#define UART_C2_ILIE  (1<<4) // Idle Line Interrupt Enable for IDLE
#define UART_C2_TE    (1<<3) // Transmitter Enable
#define UART_C2_RE    (1<<2) // Receiver Enable
#define UART_C2_RWU   (1<<1) // Receiver Wakeup Control
#define UART_C2_SBK   (1<<0) // Send Break

#define UART_S1_TDRE  (1<<7) // Transmit Data Register Empty Flag
#define UART_S1_TC    (1<<6) // Transmission Complete Flag. 0: Transmitter active, 1: Transmitter Iddle
#define UART_S1_RDRF  (1<<5) // Receive Data Register Full Flag. 0: Empty, 1: Full
#define UART_S1_IDLE  (1<<4) // Idle Line Flag
#define UART_S1_OR    (1<<3) // Receiver Overrun Flag
#define UART_S1_NF    (1<<2) // Noise Flag
#define UART_S1_FE    (1<<1) // Framing Error Flag
#define UART_S1_PF    (1<<0) // Parity Error Flag

#define UART_S2_RAF   (1<<0) // Receiver Active Flag.  

#define UART_C3_ORIE  (1<<3) // Overrun Interrupt Enable
#define UART_C3_NEIE  (1<<2) // Noise Error Interrupt Enable
#define UART_C3_FEIE  (1<<1) // Framing Error Interrupt Enable
#define UART_C3_PEIE  (1<<0) // Parity Error Interrupt Enable

// UART0 : C5, UART1 & UART2 : C4
#define UART_TDMAS (1<<7) // Transmitter DMA Select   
#define UART_RDMAS (1<<5) // Receiver Full DMA Select

typedef enum
{
    UART0 = 0,
    UART1 = 1,
    UART2 = 2
} UART_N;


// Some common baud rates.
typedef enum
{
    UART_BAUD_1200   = 1200,
    UART_BAUD_2400   = 2400,
    UART_BAUD_4800   = 4800,
    UART_BAUD_9600   = 9600,
    UART_BAUD_19200  = 19200,
    UART_BAUD_38400  = 38400,
    UART_BAUD_57600  = 57600,
    UART_BAUD_115200 = 115200

} UART_BAUD_RATE;

typedef struct _UART
{
    BOOL               isUART0;
    volatile uint32_t* SIM_enableReg;
    uint32_t           SIM_enableMask;

    volatile uint8_t*  UART_BDH;
    volatile uint8_t*  UART_BDL;
    volatile uint8_t*  UART_C1;
    volatile uint8_t*  UART_C2;
    volatile uint8_t*  UART_C3;
    volatile uint8_t*  UART_C4;
    volatile uint8_t*  UART_C5;
    volatile uint8_t*  UART_D;
    volatile uint8_t*  UART_S1;
    volatile uint8_t*  UART_S2;

    IRQn_Type          irq;
    void               (*tx_handler)();
    void               (*rx_handler)();
    void               (*error_handler)();

} UART;

UART* UART_GetInstance(UART_N n);

void UART_Initialize(UART* uart, uint32_t baudRate, BOOL enableRx, BOOL enableTx);
void UART_SendByte(UART* uart, uint8_t val);
uint8_t UART_ReceiveByte(UART* uart);

#endif
