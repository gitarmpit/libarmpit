#ifndef _I2C_H
#define _I2C_H

#include "common.h"
#include "nvic.h"

#ifndef I2C0_BASE
#define I2C0_BASE     0x40066000
#endif

#ifndef I2C1_BASE
#define I2C1_BASE     0x40067000 
#endif

#define I2C0_A1     (*(volatile uint8_t*)(I2C0_BASE + 0x0)) // Address 
#define I2C0_F      (*(volatile uint8_t*)(I2C0_BASE + 0x1)) // Frequency Divider
#define I2C0_C1     (*(volatile uint8_t*)(I2C0_BASE + 0x2)) // Control1 
#define I2C0_S      (*(volatile uint8_t*)(I2C0_BASE + 0x3)) // Status
#define I2C0_D      (*(volatile uint8_t*)(I2C0_BASE + 0x4)) // Data
#define I2C0_C2     (*(volatile uint8_t*)(I2C0_BASE + 0x5)) // Control2
#define I2C0_FLT    (*(volatile uint8_t*)(I2C0_BASE + 0x6)) // Glitch Filter
#define I2C0_RA     (*(volatile uint8_t*)(I2C0_BASE + 0x7)) // Ranger Address
#define I2C0_SMB    (*(volatile uint8_t*)(I2C0_BASE + 0x8)) // SMBus Control and Status register
#define I2C0_A2     (*(volatile uint8_t*)(I2C0_BASE + 0x9)) // Address 2
#define I2C0_SLTH   (*(volatile uint8_t*)(I2C0_BASE + 0xA)) // SCL Low Timeout Register High 
#define I2C0_SLTL   (*(volatile uint8_t*)(I2C0_BASE + 0xB)) // SCL Low Timeout Register Low

#define I2C1_A1     (*(volatile uint8_t*)(I2C1_BASE + 0x0)) // Address 
#define I2C1_F      (*(volatile uint8_t*)(I2C1_BASE + 0x1)) // Frequency Divider
#define I2C1_C1     (*(volatile uint8_t*)(I2C1_BASE + 0x2)) // Control1 
#define I2C1_S      (*(volatile uint8_t*)(I2C1_BASE + 0x3)) // Status
#define I2C1_D      (*(volatile uint8_t*)(I2C1_BASE + 0x4)) // Data
#define I2C1_C2     (*(volatile uint8_t*)(I2C1_BASE + 0x5)) // Control2
#define I2C1_FLT    (*(volatile uint8_t*)(I2C1_BASE + 0x6)) // Glitch Filter
#define I2C1_RA     (*(volatile uint8_t*)(I2C1_BASE + 0x7)) // Ranger Address
#define I2C1_SMB    (*(volatile uint8_t*)(I2C1_BASE + 0x8)) // SMBus Control and Status register
#define I2C1_A2     (*(volatile uint8_t*)(I2C1_BASE + 0x9)) // Address 2
#define I2C1_SLTH   (*(volatile uint8_t*)(I2C1_BASE + 0xA)) // SCL Low Timeout Register High 
#define I2C1_SLTL   (*(volatile uint8_t*)(I2C1_BASE + 0xB)) // SCL Low Timeout Register Low

#define I2C_F_MULT_POS 6
#define I2C_F_MULT_CLEARMASK (3<<I2C_F_MULT_POS)
#define I2C_F_ICR_CLEARMASK  0x3F

#define I2C_C1_IICEN  (1<<7)  // Enable I2C
#define I2C_C1_IICIE  (1<<6)  // Interrupt Enable
#define I2C_C1_MST    (1<<5)  // Master Mode Select
#define I2C_C1_TX     (1<<4)  // Transmit Mode Select. 0: receive, 1: transmit

#define I2C_C1_TXAK   (1<<3)  // Transmit Acknowledge Enable
                              // 0 : An acknowledge signal is sent to the bus on the following receiving byte (if FACK is cleared) or the
                              //     current receiving byte (if FACK is set).
                              // 1:  No acknowledge signal is sent to the bus on the following receiving data byte (if FACK is cleared) or
                              //     the current receiving data byte (if FACK is set).

#define I2C_C1_RSTA   (1<<2)  // Repeat START
#define I2C_C1_WUEN   (1<<1)  // Wakeup Enable
#define I2C_C1_DMAEN  (1<<0)  // DMA Enable

#define I2C_S_TCF     (1<<7)  // Transfer Complete Flag
#define I2C_S_IAAS    (1<<6)  // Addressed As A Slave
#define I2C_S_BUSY    (1<<5)  // Bus Busy
#define I2C_S_ARBL    (1<<4)  // Arbitration Lost
#define I2C_S_RAM     (1<<3)  // Range Address Match. 1: Addressed as a slave
#define I2C_S_SRW     (1<<2)  // 0: 0 Slave receive, master writing to slave. 1: Slave transmit, master reading from slave.
#define I2C_S_IICIF   (1<<1)  // InterruptFlag
#define I2C_S_RXAK    (1<<0)  // 0: Acknowledge, 1: No Ack

// Data:
// In master transmit mode, the first byte of data written to the Data register following assertion of MST (start
// bit) or assertion of RSTA (repeated start bit) is used for the address transfer and must consist of the
// calling address (in bits 7-1) concatenated with the required R/W bit (in position bit 0).

#define I2C_C2_GCAEN  (1<<7)  // General Call Address Enable
#define I2C_C2_ADEXT  (1<<6)  // Address Extension
#define I2C_C2_HDRS   (1<<5)  // High Drive Select. 1: High Drive Mode
#define I2C_C2_SBRC   (1<<4)  // Slave Baud Rate Control
#define I2C_C2_RMEN   (1<<3)  // Range Address Matching Enable

#define I2C_FLT_SHEN   (1<<7)
#define I2C_FLT_STOPF  (1<<6) // I2C Bus Stop Detect Flag
#define I2C_FLT_STOPIE (1<<7) // I2C Bus Stop Interrupt Enable

typedef enum
{
    I2C0 = 0,
    I2C1 = 1,
} I2C_N;

#define I2C_ERROR_SUCCESS 0
#define I2C_ERROR_BUSY    1
#define I2C_ERROR_RXAK    2
#define I2C_ERROR_ARBL    3
#define I2C_ERROR_TIMEOUT  4
#define I2C_ERROR_UNEXPECTED_STATE  5

typedef enum
{
	I2C_ASYNC_STATE_INIT,
	I2C_ASYNC_STATE_START,
	I2C_ASYNC_STATE_WRITE_REG,
	I2C_ASYNC_STATE_WRITE_DATA,
	I2C_ASYNC_STATE_REPEATSTART,
	I2C_ASYNC_STATE_BEGIN_READ,
	I2C_ASYNC_STATE_READ_DATA,
	I2C_ASYNC_STATE_DONE
} I2C_Async_State;


typedef struct
{
	uint8_t         slaveAddr;
	uint8_t*        tx_buf;
	uint8_t         tx_size;
	uint8_t*        data_buf;
	uint8_t         data_size;
	BOOL            isMaster;
	BOOL            isRead;
	I2C_Async_State state;
	uint32_t        waitRetry;
	uint8_t         error;
} I2C_Async_Context;

typedef struct _I2C
{
    volatile uint32_t* SIM_enableReg;
    uint32_t           SIM_enableMask;
    BOOL               isBusClock;

    volatile uint8_t*  I2C_A1;  // Slave addr
    volatile uint8_t*  I2C_F;   // Freq div
    volatile uint8_t*  I2C_C1;
    volatile uint8_t*  I2C_S;
    volatile uint8_t*  I2C_D;
    volatile uint8_t*  I2C_C2;
    volatile uint8_t*  I2C_FLT;
    volatile uint8_t*  I2C_RA;
    volatile uint8_t*  I2C_SLTH;
    volatile uint8_t*  I2C_SLTL;

    IRQn_Type          irq;
    I2C_Async_Context  *ctx; // callback context

} I2C;




#ifdef __cplusplus
 extern "C" {
#endif

I2C* I2C_GetInstance(I2C_N n);
void I2C_EnableClock(I2C* i2c, BOOL enable);
void I2C_MasterInit(I2C* i2c, uint32_t baudRate_Bps);
void I2C_Reset(I2C* i2c);
void I2C_EnableInterrupt(I2C* i2c, BOOL enable);

uint8_t I2C_MasterStart(I2C *i2c, uint8_t address, BOOL isRead);
uint8_t I2C_MasterStop(I2C *i2c);

uint8_t I2C_MasterWrite(I2C *i2c, const uint8_t *txBuff, size_t txSize);
uint8_t I2C_MasterWriteToAddr(I2C *i2c, uint8_t addr, const uint8_t *txBuff, size_t txSize);
uint8_t I2C_MasterWriteRegister(I2C* i2c, uint8_t addr, uint8_t reg, uint8_t *txBuff, size_t txSize);


uint8_t I2C_MasterRead(I2C* i2c, uint8_t *rxBuff, size_t rxSize);
uint8_t I2C_MasterReadRegister(I2C* i2c, uint8_t addr, uint8_t reg, uint8_t *rxBuff, size_t rxSize);
uint8_t I2C_MasterReadRegisterAsync(I2C* i2c, uint8_t addr, uint8_t reg,
		uint8_t *rx_buf, size_t rx_size, I2C_Async_Context* ctx, uint32_t timeout);
uint8_t I2C_MasterWriteRegisterAsync(I2C* i2c, uint8_t addr, uint8_t reg,
		uint8_t *tx_buf, size_t tx_size, I2C_Async_Context* ctx, uint32_t timeout);

uint8_t I2C_MasterTransferAsync(I2C* i2c, uint8_t addr, uint8_t* tx_buf, uint8_t tx_size,
		uint8_t *rx_buf, size_t rx_size, I2C_Async_Context* ctx, BOOL isRead, uint32_t timeout);


void I2C_SlaveInit(I2C *i2c, uint32_t slaveAddress);
BOOL I2C_SlaveWrite(I2C *i2c, const uint8_t *txBuff, size_t txSize);
BOOL I2C_SlaveRead(I2C *i2c, uint8_t *rxBuff, size_t rxSize);
#ifdef __cplusplus
}
#endif

#endif
