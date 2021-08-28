#ifndef _GPIO_HELPER_H
#define _GPIO_HELPER_H 

#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include "i2c.h"
#include "tpm.h"

#ifdef __cplusplus
#include "tpm_cpp.h"
#endif

typedef enum
{
    I2C1_PB_6_9,
    I2C1_PB_6_7,
    I2C1_PB_8_9,
    I2C2_PB_10_11
} I2C_Pins;

typedef enum
{
    // CK/MO/MI
    SPI0_PE_17_18_19,
    SPI0_PC_5_6_7,
    SPI0_PD_1_2_3,
    SPI1_PD_5_6_7,
} SPI_Pins;

typedef enum
{
    USART1_PA_9_10,
    USART1_PB_6_7,
    USART2_PA_2_3,
    USART2_PD_5_6,
} UART_Pins;


typedef enum
{
	TPM1_CH0_E20,
	TPM1_CH1_E21,

	TPM2_CH0_E22,
	TPM2_CH1_E23,

	TPM0_CH2_E29,
	TPM0_CH3_E30,
	TPM0_CH4_E31,

	TPM0_CH0_E24,
	TPM0_CH1_E25,
	TPM0_CH5_A0, // SWD_CLK !!!

	TPM2_CH0_A1,
	TPM2_CH1_A2,

	TPM0_CH0_A3, // SWD_IO !!!
	TPM0_CH1_A4, // NMI
	TPM0_CH2_A5,

	TPM1_CH0_A12,
	TPM1_CH1_A13,

	TPM1_CH0_B0,
	TPM1_CH1_B1,

	TPM2_CH0_B2,
	TPM2_CH1_B3,

	TPM2_CH0_B18,
	TPM2_CH1_B19,

	TPM0_CH0_C1,
	TPM0_CH1_C2,
	TPM0_CH2_C3,
	TPM0_CH3_C4,

	TPM0_CH0_D0,
	TPM0_CH1_D1,
	TPM0_CH2_D2,
	TPM0_CH3_D3,
	TPM0_CH4_D4,
	TPM0_CH5_D5,

} TPM_Pins;

#ifdef __cplusplus
extern "C"
{
#endif

SPI* GPIO_Helper_SetupSPI(SPI_Pins spi_no, BOOL isMaster, uint8_t presc, SPI_BRDIV div, BOOL isPhaseStart, BOOL isPolarityLow, BOOL isLSB);
GPIO_PIN GPIO_Helper_GetPin (const char* pin);

TPM_Channel_t* GPIO_Helper_SetupTPM_Channel_t(TPM_Pins pin);

#ifdef __cplusplus
TPM_Channel* GPIO_Helper_SetupTPM_Channel(TPM_Pins pin);
#endif

#ifdef __cplusplus
}
#endif


#endif
