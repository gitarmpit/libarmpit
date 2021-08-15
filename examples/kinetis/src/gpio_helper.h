#ifndef _GPIO_HELPER_H
#define _GPIO_HELPER_H 

#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include "i2c.h"

typedef enum
{
    TIM1_PA_8_9_10_11,
    TIM2_PA_0_1_2_3,
    TIM3_PC_6_7_8_9,
    TIM3_PB_4_5_0_1,
    TIM4_PD_12_13_14_15,
    TIM4_PB_6_7_8_9,
    TIM5_PA_0_1_2_3,
    TIM8_PC_6_7_8_9
} Timer_Pins;

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

#ifdef __cplusplus
extern "C"
{
#endif


SPI* GPIO_Helper_SetupSPI(SPI_Pins spi_no, BOOL isMaster, uint8_t presc, SPI_BRDIV div, BOOL isPhaseStart, BOOL isPolarityLow, BOOL isLSB);
GPIO_PIN GPIO_Helper_GetPin (const char* pin);

#ifdef __cplusplus
}
#endif


#endif
