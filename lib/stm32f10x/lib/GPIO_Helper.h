#ifndef _GPIO_HELPER_H
#define _GPIO_HELPER_H

#include "spi_cpp.h"
#include "timer_cpp.h"
#include "i2c_cpp.h"
#include "usart_cpp.h"
#include "gpio_cpp.h"
#include "exti_cpp.h"

enum Timer_Pins
{
	TIM1_PA_8_9_10_11,
	TIM1_PE_9_11_13_14,
	TIM2_PA_0_1_2_3,
	TIM2_PA_15_PB_3_10_11,
	TIM3_PA6_PA7_PB0_PB1,
	TIM3_PC_6_7_8_9,
	TIM3_PB_4_5_0_1,
#if !defined(STM32F10X_LD) && !defined(STM32F10X_LD_VL)
	TIM4_PD_12_13_14_15,
	TIM4_PB_6_7_8_9,
#endif
#if defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL) || defined(STM32F10X_CL)
	TIM5_PA_0_1_2_3,
#endif
#if defined(STM32F10X_XL) || defined(STM32F10X_HD)
	TIM8_PC_6_7_8_9
#endif
};


enum I2C_Pins
{
    I2C1_PB_6_7,
    I2C1_PB_8_9,
#if !defined (STM32F10X_LD) && !defined(STM32F10X_LD_VL)
    I2C2_PB_10_11
#endif
};

enum SPI_Pins
{
    SPI1_PA_5_6_7,
    SPI1_PB_3_4_5,
    SPI2_PB_13_14_15,
#if defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL) || defined(STM32F10X_CL)
    SPI3_PB_3_4_5
#endif
};

enum USART_Pins
{
    USART1_PA_9_10,
    USART2_PA_2_3,
    USART1_PB_6_7,
    USART2_PD_5_6,
#if !defined (STM32F10X_LD) && !defined(STM32F10X_LD_VL)
	USART3_PB_10_11,
    USART3_PC_10_11,
    USART3_PD_8_9,
#endif
};


class GPIO_Helper
{
private:
    static SPI* SetupSPIx (bool isMasterMode, bool isClockPhase2, bool isClockPolarity1, SPI_BAUD_RATE baudRate,
                                GPIO_PORT* port,  GPIO_PIN_N clkPin, GPIO_PIN_N misoPin, GPIO_PIN_N mosiPin, SPI* spi);
    static void SetupTIMx(GPIO_PORT* port, GPIO_PIN_N ch0, GPIO_PIN_N ch1, GPIO_PIN_N ch2, GPIO_PIN_N ch3);
    static I2C*   SetupI2Cx (I2C* i2c, uint32_t clockSpeed,  GPIO_PORT* port,  GPIO_PIN_N sdaPin, GPIO_PIN_N sclPin);
    static USART*  SetupUSARTx(GPIO_PORT* port, GPIO_PIN_N txPin, GPIO_PIN_N rxPin, USART* usart, uint32_t clockSpeed);
public:

    static SPI* SetupSPI(SPI_Pins spi_no, bool isMasterMode, bool isClockPhase2, bool isClockPolarity1, SPI_BAUD_RATE baudRate);
    static TIMER_With_Channels* SetupTimer (Timer_Pins t);
    static I2C*  SetupI2C(I2C_Pins i2c_no, uint32_t clockSpeed);
    static USART* SetupUSART(USART_Pins usart_no, uint32_t clockSpeed);



    static GPIO_PIN* GetPin (const char* pin); //A1,B1,C15 etc

    static EXTI* SetupEXTI (const char* pin);

};

#endif
