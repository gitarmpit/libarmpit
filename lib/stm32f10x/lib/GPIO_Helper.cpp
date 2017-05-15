#include "GPIO_Helper.h" 
#include "spi_cpp.h"

#include "afio.h"
#include <string.h>
#include <stdlib.h>



SPI* GPIO_Helper::SetupSPIx (bool isMasterMode, bool isClockPhase2, bool isClockPolarity1, SPI_BAUD_RATE baudRate,
        GPIO_PORT* port,  GPIO_PIN_N clkPin, GPIO_PIN_N misoPin, GPIO_PIN_N mosiPin, SPI* spi)
{
    port->EnablePeripheralClock(true);
    GPIO_PIN* CLK_pin = port->GetPin (clkPin);
    GPIO_PIN* MISO_pin = port->GetPin (misoPin);
    GPIO_PIN* MOSI_pin = port->GetPin (mosiPin);

    CLK_pin->SetupGPIO_OutAltPP();
    if (isMasterMode)
    {
        MOSI_pin->SetupGPIO_OutAltPP();
        MISO_pin->SetupGPIO_InPullDown();
    }
    else
    {
        MOSI_pin->SetupGPIO_InPullDown();
        MISO_pin->SetupGPIO_OutAltPP();
    }

    bool isFullDuplex = true;
    bool isLSBFirst = false;
    bool isSetSSM = true;
    bool is16Bit = false;

    spi->Initialize(isMasterMode, isFullDuplex, baudRate,  isClockPhase2, isClockPolarity1, isLSBFirst, isSetSSM, is16Bit);
    return spi;

}

SPI* GPIO_Helper::SetupSPI(SPI_Pins spi_no, bool isMasterMode, bool isClockPhase2, bool isClockPolarity1, SPI_BAUD_RATE baudRate)
{
    switch (spi_no)
    {
    case SPI1_PA_5_6_7:
        AFIO_Remap_SPI1(false);
        return SetupSPIx(isMasterMode, isClockPhase2, isClockPolarity1, baudRate, GPIOA::GetInstance(),
                GPIO_PIN5, GPIO_PIN6, GPIO_PIN7, SPI1::GetInstance());

    case SPI1_PB_3_4_5:
        AFIO_Remap_SPI1(true);
        return SetupSPIx(isMasterMode, isClockPhase2, isClockPolarity1, baudRate, GPIOB::GetInstance(),
                GPIO_PIN3, GPIO_PIN4, GPIO_PIN5, SPI1::GetInstance());

    case SPI2_PB_13_14_15:
        return SetupSPIx(isMasterMode, isClockPhase2, isClockPolarity1, baudRate, GPIOB::GetInstance(),
                GPIO_PIN13, GPIO_PIN14, GPIO_PIN15, SPI2::GetInstance());

#if defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL) || defined(STM32F10X_CL)
    case SPI3_PB_3_4_5:
        return SetupSPIx(isMasterMode, isClockPhase2, isClockPolarity1, baudRate, GPIOB::GetInstance(),
                GPIO_PIN3, GPIO_PIN4, GPIO_PIN5, SPI3::GetInstance());
#endif
    };
    return 0;
}


void  GPIO_Helper::SetupTIMx(GPIO_PORT* port, GPIO_PIN_N ch0, GPIO_PIN_N ch1, GPIO_PIN_N ch2, GPIO_PIN_N ch3)
{
    port->EnablePeripheralClock(true);

    port->SetupGPIO_OutAltPP(ch0);
    port->SetupGPIO_OutAltPP(ch1);
    port->SetupGPIO_OutAltPP(ch2);
    port->SetupGPIO_OutAltPP(ch3);

}

TIMER_With_Channels* GPIO_Helper::SetupTimer (Timer_Pins t)
{
    TIMER_With_Channels* timer;
    switch (t)
    {
    case TIM1_PA_8_9_10_11:
        AFIO_Remap_TIM1(false, false);
        timer = TIM1::GetInstance();
        timer->EnablePeripheralClock(true);
        SetupTIMx(GPIOA::GetInstance(), GPIO_PIN8, GPIO_PIN9, GPIO_PIN10, GPIO_PIN11);
        return timer;
    case TIM1_PE_9_11_13_14:
        AFIO_Remap_TIM1(true, true);
        timer = TIM1::GetInstance();
        timer->EnablePeripheralClock(true);
        SetupTIMx(GPIOE::GetInstance(), GPIO_PIN9, GPIO_PIN11, GPIO_PIN13, GPIO_PIN14);
        return timer;
    case TIM2_PA_0_1_2_3:
        AFIO_Remap_TIM2(false, false);
        timer = TIM2::GetInstance();
        timer->EnablePeripheralClock(true);
        SetupTIMx(GPIOA::GetInstance(), GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3);
        return timer;
    case TIM2_PA_15_PB_3_10_11:
        AFIO_Remap_TIM2(true, true);
        timer = TIM2::GetInstance();
        timer->EnablePeripheralClock(true);
        SetupTIMx(GPIOA::GetInstance(), GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3);
        return timer;
    case TIM3_PA6_PA7_PB0_PB1:
    {
        AFIO_Remap_TIM3(false, false);
        timer = TIM3::GetInstance();
        timer->EnablePeripheralClock(true);
        GPIOA::GetInstance()->EnablePeripheralClock(true);
        GPIOB::GetInstance()->EnablePeripheralClock(true);
        GPIO_PIN* ch0 = GPIOA::GetInstance()->GetPin(GPIO_PIN6);
        GPIO_PIN* ch1 = GPIOA::GetInstance()->GetPin(GPIO_PIN7);
        GPIO_PIN* ch2 = GPIOB::GetInstance()->GetPin(GPIO_PIN0);
        GPIO_PIN* ch3 = GPIOB::GetInstance()->GetPin(GPIO_PIN1);
        ch0->SetupGPIO_OutAltPP();
        ch1->SetupGPIO_OutAltPP();
        ch2->SetupGPIO_OutAltPP();
        ch3->SetupGPIO_OutAltPP();
        return timer;
    }
    case TIM3_PC_6_7_8_9:
        AFIO_Remap_TIM3(true, true);
        timer = TIM3::GetInstance();
        timer->EnablePeripheralClock(true);
        SetupTIMx(GPIOC::GetInstance(), GPIO_PIN6, GPIO_PIN7, GPIO_PIN8, GPIO_PIN9);
        return timer;
    case TIM3_PB_4_5_0_1:
        AFIO_Remap_TIM3(true, false);
        timer = TIM3::GetInstance();
        timer->EnablePeripheralClock(true);
        SetupTIMx(GPIOB::GetInstance(), GPIO_PIN4, GPIO_PIN5, GPIO_PIN0, GPIO_PIN1);
        return timer;
#if !defined(STM32F10X_LD) && !defined(STM32F10X_LD_VL)
   case TIM4_PD_12_13_14_15:
	    AFIO_Remap_TIM4(true);
        timer = TIM4::GetInstance();
        timer->EnablePeripheralClock(true);
        SetupTIMx(GPIOD::GetInstance(), GPIO_PIN12, GPIO_PIN13, GPIO_PIN14, GPIO_PIN15);
        return timer;
#endif
    case TIM4_PB_6_7_8_9:
        AFIO_Remap_TIM4(false);
        timer = TIM4::GetInstance();
        timer->EnablePeripheralClock(true);
        SetupTIMx(GPIOB::GetInstance(), GPIO_PIN6, GPIO_PIN7, GPIO_PIN8, GPIO_PIN9);
        return timer;
#if defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL) || defined(STM32F10X_CL)
    case TIM5_PA_0_1_2_3:
        timer = TIM5::GetInstance();
        timer->EnablePeripheralClock(true);
        SetupTIMx(GPIOA::GetInstance(), GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3);
        return timer;
#endif
#if defined(STM32F10X_XL) || defined(STM32F10X_HD)
    case TIM8_PC_6_7_8_9:
        timer = TIM8::GetInstance();
        timer->EnablePeripheralClock(true);
        SetupTIMx(GPIOC::GetInstance(), GPIO_PIN6, GPIO_PIN7, GPIO_PIN8, GPIO_PIN9);
        return timer;
#endif

    };

    return 0;
}




I2C* GPIO_Helper::SetupI2Cx (I2C* i2c, uint32_t clockSpeed,  GPIO_PORT* port,  GPIO_PIN_N sclPin, GPIO_PIN_N sdaPin)
{
    if (!port->IsClockEnabled())
    {
        port->EnablePeripheralClock(true);
    }

    port->SetupGPIO_OutAltOD (sclPin);  //I2C_SCL
    port->SetupGPIO_OutAltOD (sdaPin);  //I2C_SDA

    bool isDuty16_9 = false;
    bool isFmMode = false;

    if (clockSpeed > 100000)
    {
        isFmMode = true;
    }
    i2c->Initialize(clockSpeed, isDuty16_9, isFmMode);
    i2c->SetModeI2C();
    i2c->EnableACK(false);
    i2c->SetOwnAddr7Bit(0x00);

    return i2c;
}

I2C*  GPIO_Helper::SetupI2C(I2C_Pins i2c_no, uint32_t clockSpeed)
{
	switch (i2c_no)
	{
	case I2C1_PB_6_7:
	    AFIO_Remap_I2C1(false);
	    return SetupI2Cx(I2C1::GetInstance(), clockSpeed, GPIOB::GetInstance(), GPIO_PIN6, GPIO_PIN7);
	case I2C1_PB_8_9:
		AFIO_Remap_I2C1(true);
		return (I2C1*)SetupI2Cx(I2C1::GetInstance(), clockSpeed, GPIOB::GetInstance(), GPIO_PIN8, GPIO_PIN9);
#if !defined (STM32F10X_LD) && !defined(STM32F10X_LD_VL)
	case I2C2_PB_10_11:
		return (I2C2*)SetupI2Cx(I2C2::GetInstance(), clockSpeed, GPIOB::GetInstance(), GPIO_PIN10, GPIO_PIN11);
#endif

	};

	return 0;
}


USART* GPIO_Helper::SetupUSARTx(GPIO_PORT* port, GPIO_PIN_N txPinNo, GPIO_PIN_N rxPinNo, USART* usart, uint32_t clockSpeed)
{
    port->EnablePeripheralClock(true);
    GPIO_PIN* txPin = port->GetPin(txPinNo);
    GPIO_PIN* rxPin = port->GetPin(rxPinNo);


    txPin->SetupGPIO_OutAltPP();
    txPin->SetSpeedHigh();

    rxPin->SetupGPIO_InFloat();

    usart->Enable(true);
    usart->SetBaudRate(clockSpeed);
    usart->EnableTransmitter(true);
    usart->EnableReceiver(true);
    return usart;

}

USART* GPIO_Helper::SetupUSART(USART_Pins usart_no, uint32_t clockSpeed)
{
	switch (usart_no)
	{
	case USART1_PA_9_10:
	    AFIO_Remap_USART1(false);
	    return (USART1*)SetupUSARTx(GPIOA::GetInstance(), GPIO_PIN9, GPIO_PIN10, USART1::GetInstance(), clockSpeed);
	case USART1_PB_6_7:
	    AFIO_Remap_USART1(true);
	    return (USART1*)SetupUSARTx(GPIOB::GetInstance(), GPIO_PIN6, GPIO_PIN7, USART1::GetInstance(), clockSpeed);
	case USART2_PA_2_3:
	    AFIO_Remap_USART2(false);
	    return (USART2*)SetupUSARTx(GPIOA::GetInstance(), GPIO_PIN2, GPIO_PIN3, USART2::GetInstance(), clockSpeed);
	case USART2_PD_5_6:
	    AFIO_Remap_USART2(true);
	    return (USART2*)SetupUSARTx(GPIOD::GetInstance(), GPIO_PIN5, GPIO_PIN6, USART2::GetInstance(), clockSpeed);
#if !defined (STM32F10X_LD) && !defined(STM32F10X_LD_VL)
	case USART3_PB_10_11:
	    AFIO_Remap_USART3(false, false);
	    return (USART3*)SetupUSARTx(GPIOB::GetInstance(), GPIO_PIN10, GPIO_PIN11, USART3::GetInstance(), clockSpeed);
	case USART3_PC_10_11:
	    AFIO_Remap_USART3(true, false);
	    return (USART3*)SetupUSARTx(GPIOC::GetInstance(), GPIO_PIN10, GPIO_PIN11, USART3::GetInstance(), clockSpeed);
	case USART3_PD_8_9:
	    AFIO_Remap_USART3(true, true);
	    return (USART3*)SetupUSARTx(GPIOD::GetInstance(), GPIO_PIN8, GPIO_PIN9, USART3::GetInstance(), clockSpeed);
#endif
	};

	return 0;
}


GPIO_PIN* GPIO_Helper::GetPin (const char* pin) //A1,B1,C15 etc
{
    if (!pin || strlen(pin) < 2)
    {
        return 0;
    }

    GPIO_PORT* port;
    switch (pin[0])
    {
    case 'a':
    case 'A':
        port = GPIOA::GetInstance();
        break;
    case 'b':
    case 'B':
        port = GPIOB::GetInstance();
        break;
    case 'c':
    case 'C':
        port = GPIOC::GetInstance();
        break;
    case 'd':
    case 'D':
        port = GPIOD::GetInstance();
        break;
    case 'e':
    case 'E':
        port = GPIOE::GetInstance();
        break;
    case 'f':
    case 'F':
        port = GPIOF::GetInstance();
        break;
    case 'g':
    case 'G':
        port = GPIOG::GetInstance();
        break;
    default:
        return 0;

    };

    if (!port->IsClockEnabled())
    {
        port->EnablePeripheralClock(true);
    }

    int pin_no = atoi(&pin[1]);
    return port->GetPin((GPIO_PIN_N)pin_no);

}


EXTI* GPIO_Helper::SetupEXTI (const char* pin)
{
    if (!pin || strlen(pin) < 2)
    {
        return 0;
    }

    if (!RCC_IsEnabledAFIO())
    {
        RCC_EnableAFIO(TRUE);
    }

    int pin_no = atoi(&pin[1]);

    switch (pin[0])
    {
    case 'a':
    case 'A':
        AFIO_RemapEXTI((EXTI_LINE)pin_no, PORTA);
        break;
    case 'b':
    case 'B':
        AFIO_RemapEXTI((EXTI_LINE)pin_no, PORTB);
        break;
    case 'c':
    case 'C':
        AFIO_RemapEXTI((EXTI_LINE)pin_no, PORTC);
        break;
    case 'd':
    case 'D':
        AFIO_RemapEXTI((EXTI_LINE)pin_no, PORTD);
        break;
    case 'e':
    case 'E':
        AFIO_RemapEXTI((EXTI_LINE)pin_no, PORTE);
        break;
    case 'f':
    case 'F':
        AFIO_RemapEXTI((EXTI_LINE)pin_no, PORTF);
        break;
    case 'g':
    case 'G':
        AFIO_RemapEXTI((EXTI_LINE)pin_no, PORTG);
        break;
    default:
        return 0;

    };

    return  EXTI::GetInstance((EXTI_LINE)pin_no);


}

