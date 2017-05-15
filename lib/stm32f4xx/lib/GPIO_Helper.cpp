#include "GPIO_Helper.h" 
#include <stdlib.h>
#include <string.h>
#include "rcc.h"
#include "syscfg.h"

SPI* GPIO_Helper::SetupSPIx (bool isMasterMode, bool isClockPhase2, bool isClockPolarity1, SPI_BAUD_RATE baudRate,
        GPIO_PORT* port,  GPIO_PIN_N clkPin, GPIO_PIN_N misoPin, GPIO_PIN_N mosiPin, SPI* spi, GPIO_AF af)
{
    port->EnablePeripheralClock(true);
    GPIO_PIN* CLK_pin = port->GetPin (clkPin);
    GPIO_PIN* MISO_pin = port->GetPin (misoPin);
    GPIO_PIN* MOSI_pin = port->GetPin (mosiPin);

    MOSI_pin->SetAF (af);
    CLK_pin->SetAF (af);
    MISO_pin->SetAF (af);

    MOSI_pin->SetupGPIO_OutAltPP();
    MOSI_pin->PullDown();
    MOSI_pin->SetSpeedHigh();

    CLK_pin->SetupGPIO_OutAltPP();
    CLK_pin->PullDown();
    CLK_pin->SetSpeedHigh();

    MISO_pin->SetupGPIO_OutAltPP();
    MISO_pin->PullDown();
    MISO_pin->SetSpeedHigh();

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
	    return SetupSPIx(isMasterMode, isClockPhase2, isClockPolarity1, baudRate, GPIOA::GetInstance(),
	    		GPIO_PIN5, GPIO_PIN6, GPIO_PIN7, SPI1::GetInstance(), AF5);

	case SPI1_PB_3_4_5:
	    return SetupSPIx(isMasterMode, isClockPhase2, isClockPolarity1, baudRate, GPIOB::GetInstance(),
	    		GPIO_PIN3, GPIO_PIN4, GPIO_PIN5, SPI1::GetInstance(), AF5);

	case SPI2_PB_13_14_15:
	    return SetupSPIx(isMasterMode, isClockPhase2, isClockPolarity1, baudRate, GPIOB::GetInstance(),
	    		GPIO_PIN13, GPIO_PIN14, GPIO_PIN15, SPI2::GetInstance(), AF5);

	case SPI3_PB_3_4_5:
	    return SetupSPIx(isMasterMode, isClockPhase2, isClockPolarity1, baudRate, GPIOB::GetInstance(),
	    		GPIO_PIN3, GPIO_PIN4, GPIO_PIN5, SPI3::GetInstance(), AF6);

	case SPI3_PC_10_11_12:
	    return SetupSPIx(true, isClockPhase2, isClockPolarity1, baudRate, GPIOC::GetInstance(),
	    		GPIO_PIN10, GPIO_PIN11, GPIO_PIN12, SPI3::GetInstance(), AF6);

	};
	return 0;
}

void  GPIO_Helper::SetupTIMx(GPIO_PORT* port, GPIO_PIN_N ch0, GPIO_PIN_N ch1, GPIO_PIN_N ch2, GPIO_PIN_N ch3, GPIO_AF af)
{
    port->EnablePeripheralClock(true);

    port->SetAF(af, ch0);
    port->SetAF(af, ch1);
    port->SetAF(af, ch2);
    port->SetAF(af, ch3);

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
		timer = TIM1::GetInstance();
		timer->EnablePeripheralClock(true);
		SetupTIMx(GPIOA::GetInstance(), GPIO_PIN8, GPIO_PIN9, GPIO_PIN10, GPIO_PIN11, AF1);
		return timer;
	case TIM2_PA_0_1_2_3:
	    timer = TIM2::GetInstance();
	    timer->EnablePeripheralClock(true);
	    SetupTIMx(GPIOA::GetInstance(), GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, AF1);
	    return timer;
	case TIM3_PC_6_7_8_9:
	    timer = TIM3::GetInstance();
	    timer->EnablePeripheralClock(true);
	    SetupTIMx(GPIOC::GetInstance(), GPIO_PIN6, GPIO_PIN7, GPIO_PIN8, GPIO_PIN9, AF2);
	    return timer;
	case TIM3_PB_4_5_0_1:
	    timer = TIM3::GetInstance();
	    timer->EnablePeripheralClock(true);
	    SetupTIMx(GPIOB::GetInstance(), GPIO_PIN4, GPIO_PIN5, GPIO_PIN0, GPIO_PIN1, AF2);
	    return timer;
	case TIM4_PD_12_13_14_15:
	    timer = TIM4::GetInstance();
	    timer->EnablePeripheralClock(true);
	    SetupTIMx(GPIOD::GetInstance(), GPIO_PIN12, GPIO_PIN13, GPIO_PIN14, GPIO_PIN15, AF2);
	    return timer;
	case TIM4_PB_6_7_8_9:
	    timer = TIM4::GetInstance();
	    timer->EnablePeripheralClock(true);
	    SetupTIMx(GPIOB::GetInstance(), GPIO_PIN6, GPIO_PIN7, GPIO_PIN8, GPIO_PIN9, AF2);
	    return timer;
	case TIM5_PA_0_1_2_3:
	    timer = TIM5::GetInstance();
	    timer->EnablePeripheralClock(true);
	    SetupTIMx(GPIOA::GetInstance(), GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, AF2);
	    return timer;
#if !defined(STM32F401xC)  && !defined(STM32F401xE) && !defined(STM32F401xx) && !defined(STM32F411xE)
	case TIM8_PC_6_7_8_9:
	    timer = TIM8::GetInstance();
	    timer->EnablePeripheralClock(true);
	    SetupTIMx(GPIOC::GetInstance(), GPIO_PIN6, GPIO_PIN7, GPIO_PIN8, GPIO_PIN9, AF3);
	    return timer;
#endif

	};

	return 0;
}


I2C* GPIO_Helper::SetupI2Cx(I2C* i2c, GPIO_PORT* port, GPIO_PIN_N sckPinNo, GPIO_PIN_N sdaPinNo, GPIO_AF afno,
        DMA_CHANNEL txChannel, DMA_CHANNEL rxChannel,
        DMA_STREAM_N txStreamNo, DMA_STREAM_N rxStreamNo,
        uint32_t clockSpeed)
{
    port->EnablePeripheralClock(true);
    GPIO_PIN* SDA_pin = port->GetPin(sdaPinNo);
    GPIO_PIN* SCK_pin = port->GetPin(sckPinNo);

    SDA_pin->SetAF(afno);
    SCK_pin->SetAF(afno);

    SDA_pin->SetupGPIO_OutAltOD();
    SDA_pin->PullUp();
    SDA_pin->SetSpeedMedium();

    SCK_pin->SetupGPIO_OutAltOD();
    SCK_pin->SetSpeedMedium();
    SCK_pin->PullUp();

    DMA1* dma1 = DMA1::GetInstance();
    dma1->EnablePeripheralClock(true);

    DMA_Stream* txStream = dma1->GetStream(txStreamNo);
    txStream->SetChannel(txChannel);
    DMA_Stream* rxStream = dma1->GetStream(rxStreamNo);
    rxStream->SetChannel(rxChannel);

    bool isDuty16_9 = false;
    bool isFmMode = false;
    i2c->Initialize(clockSpeed, isDuty16_9, isFmMode);
    i2c->SetModeI2C();
    i2c->EnableACK(false);
    i2c->SetOwnAddr7Bit(0x00);
    i2c->SetDMA_TxStream(txStream);
    i2c->SetDMA_RxStream(rxStream);

    return i2c;

}


I2C*  GPIO_Helper::SetupI2C(I2C_Pins i2c_no, uint32_t clockSpeed)
{
	switch (i2c_no)
	{
	case I2C1_PB_6_9:
	    return (I2C1*)SetupI2Cx(I2C1::GetInstance(), GPIOB::GetInstance(), GPIO_PIN6, GPIO_PIN9, AF4,
	            DMA_CHANNEL_1, DMA_CHANNEL_1, DMA_STREAM_6, DMA_STREAM_5, clockSpeed);
	case I2C1_PB_6_7:
	    return (I2C1*)SetupI2Cx(I2C1::GetInstance(), GPIOB::GetInstance(), GPIO_PIN6, GPIO_PIN7, AF4,
	            DMA_CHANNEL_1, DMA_CHANNEL_1, DMA_STREAM_6, DMA_STREAM_5, clockSpeed);

	case I2C1_PB_8_9:
	    return (I2C1*)SetupI2Cx(I2C1::GetInstance(), GPIOB::GetInstance(), GPIO_PIN8, GPIO_PIN9, AF4,
	            DMA_CHANNEL_1, DMA_CHANNEL_1, DMA_STREAM_6, DMA_STREAM_5, clockSpeed);

	case I2C2_PB_10_11:
	    return (I2C2*)SetupI2Cx(I2C2::GetInstance(), GPIOB::GetInstance(), GPIO_PIN10, GPIO_PIN11, AF4,
	            DMA_CHANNEL_7, DMA_CHANNEL_7, DMA_STREAM_7, DMA_STREAM_2, clockSpeed);

	};

	return 0;
}


USART* GPIO_Helper::SetupUSARTx(GPIO_PORT* port, GPIO_PIN_N txPinNo, GPIO_PIN_N rxPinNo, GPIO_AF af, USART* usart, uint32_t clockSpeed)
{
    port->EnablePeripheralClock(true);
    GPIO_PIN* txPin = port->GetPin(txPinNo);
    GPIO_PIN* rxPin = port->GetPin(rxPinNo);

    txPin->SetAF(af);
    rxPin->SetAF(af);

    txPin->SetupGPIO_OutAltPP();
    txPin->SetSpeedMedium();

    rxPin->SetupGPIO_OutAltPP();
    rxPin->SetSpeedMedium();

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
	    return SetupUSARTx(GPIOA::GetInstance(), GPIO_PIN9, GPIO_PIN10, AF7, USART1::GetInstance(), clockSpeed);
	case USART1_PB_6_7:
	    return SetupUSARTx(GPIOB::GetInstance(), GPIO_PIN6, GPIO_PIN7, AF7, USART1::GetInstance(), clockSpeed);
	case USART2_PA_2_3:
	    return SetupUSARTx(GPIOA::GetInstance(), GPIO_PIN2, GPIO_PIN3, AF7, USART2::GetInstance(), clockSpeed);
	case USART2_PD_5_6:
	    return SetupUSARTx(GPIOD::GetInstance(), GPIO_PIN5, GPIO_PIN6, AF7, USART2::GetInstance(), clockSpeed);
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) || defined(STM32F427xx) || defined(STM32F437xx) \
|| defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F446xx)
	case USART3_PB_10_11:
	    return SetupUSARTx(GPIOB::GetInstance(), GPIO_PIN10, GPIO_PIN11, AF7, USART3::GetInstance(), clockSpeed);
	case USART3_PC_10_11:
	    return SetupUSARTx(GPIOC::GetInstance(), GPIO_PIN10, GPIO_PIN11, AF7, USART3::GetInstance(), clockSpeed);
	case USART3_PD_8_9:
	    return SetupUSARTx(GPIOD::GetInstance(), GPIO_PIN8, GPIO_PIN9, AF7, USART3::GetInstance(), clockSpeed);
#endif
	case USART6_PC_6_7:
	    return SetupUSARTx(GPIOC::GetInstance(), GPIO_PIN6, GPIO_PIN7, AF7, USART6::GetInstance(), clockSpeed);
	case USART6_PG_14_9:
	    return SetupUSARTx(GPIOG::GetInstance(), GPIO_PIN14, GPIO_PIN9, AF7, USART6::GetInstance(), clockSpeed);
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
	case 'h':
	case 'H':
		port = GPIOH::GetInstance();
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

    if (!RCC_IsEnabledSYSCFG())
    {
        RCC_EnableSYSCFG(TRUE);
    }

    int pin_no = atoi(&pin[1]);

    switch (pin[0])
    {
    case 'a':
    case 'A':
        SYSCFG_RemapEXTI((EXTI_LINE)pin_no, PORTA);
        break;
    case 'b':
    case 'B':
        SYSCFG_RemapEXTI((EXTI_LINE)pin_no, PORTB);
        break;
    case 'c':
    case 'C':
        SYSCFG_RemapEXTI((EXTI_LINE)pin_no, PORTC);
        break;
    case 'd':
    case 'D':
        SYSCFG_RemapEXTI((EXTI_LINE)pin_no, PORTD);
        break;
    case 'e':
    case 'E':
        SYSCFG_RemapEXTI((EXTI_LINE)pin_no, PORTE);
        break;
    case 'f':
    case 'F':
        SYSCFG_RemapEXTI((EXTI_LINE)pin_no, PORTF);
        break;
    case 'g':
    case 'G':
        SYSCFG_RemapEXTI((EXTI_LINE)pin_no, PORTG);
        break;
    case 'h':
    case 'H':
        SYSCFG_RemapEXTI((EXTI_LINE)pin_no, PORTH);
        break;
    default:
        return 0;

    };

    return  EXTI::GetInstance((EXTI_LINE)pin_no);

}




