#include "GPIO_Helper.h"
#include "RCC_Helper.h"
#include "debug.h"

#define AD9850_CLOCK 125000000

void send_byte(uint8_t data, GPIO_PIN* clock_pin, GPIO_PIN* data_pin)
{
    for (int i = 0; i < 8; i++, data >>= 1)
    {
        (data & 0x01) ? data_pin->Set() : data_pin->Reset();
        clock_pin->Set();
        delay_us(40);
        clock_pin->Reset();
    }
}

void sendFrequency(double frequency, GPIO_PIN* clock_pin, GPIO_PIN* data_pin)
{
    volatile double f = frequency;
    volatile int32_t freq1 = f * (double)4294967295 / (double)AD9850_CLOCK;  // note 125 MHz clock on 9850
    for (int b = 0; b < 4; b++, freq1 >>= 8)
    {
        send_byte(freq1 & 0xFF, clock_pin, data_pin);
    }

    send_byte(0x0, clock_pin, data_pin);
}

void sendFrequency(SPI* spi, double frequency)
{
    volatile double f = frequency;
    volatile uint32_t freq1 = f * (double)4294967295 / (double)AD9850_CLOCK;  // note 125 MHz clock on 9850
    for (int b = 0; b < 4; b++, freq1 >>= 8)
    {
        spi->TransmitByte(freq1 & 0xFF);
    }

//    spi->TransmitByte(0x5b);                     // Final control byte, phase and settings
//    spi->TransmitByte(0xd);                     // Final control byte, phase and settings
//    spi->TransmitByte(0x0);                     // Final control byte, phase and settings
//    spi->TransmitByte(0x0);                     // Final control byte, phase and settings


//  for (int b = 0; b < 4; ++b)
//  {
//      volatile uint8_t tmp = (freq1 >> (24-b*8)) & 0xFF;
//      spi->TransmitByte(tmp);
//  }

    spi->TransmitByte(0x00);                     // Final control byte, phase and settings
}

void run2()
{
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    GPIO_PIN* SS_pin = portA->GetPin(GPIO_PIN4);
    SS_pin->SetupGPIO_OutPP();
    SS_pin->SetSpeedHigh();

    GPIO_PIN* res_pin = portB->GetPin(GPIO_PIN1);
    res_pin->SetupGPIO_OutPP();
    res_pin->SetSpeedHigh();

    GPIO_PIN* clock_pin = portA->GetPin(GPIO_PIN5);
    clock_pin->SetupGPIO_OutPP();
    clock_pin->SetSpeedHigh();

    GPIO_PIN* data_pin = portA->GetPin(GPIO_PIN7);
    data_pin->SetupGPIO_OutPP();
    data_pin->SetSpeedHigh();

//    res_pin->Set();
//    res_pin->Reset();

//    clock_pin->Set();
//    clock_pin->Reset();

    //SS_pin->Set();

    for (int i = 0; i < 100; ++i)
    {
        SS_pin->Reset();
        sendFrequency(100 + i, clock_pin, data_pin);
        SS_pin->Set();
        SS_pin->Reset();
        SS_pin->Set();
        delay(1000);
    }

    while(1)
        ;
}


void run()
{
    //RESET  should be high
    //DATA : MOSI
    //CLK
    //FQ_UD: SS

    //GPIO_PIN* SS_pin = GPIO_Helper::GetPin(

    delay(100);
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    GPIO_PIN* clock_pin = portA->GetPin(GPIO_PIN5);
    clock_pin->SetupGPIO_OutPP();
    clock_pin->SetSpeedHigh();


    GPIO_PIN* SS_pin = portA->GetPin(GPIO_PIN4);
    SS_pin->SetupGPIO_OutPP();
    SS_pin->SetSpeedHigh();

    GPIO_PIN* res_pin = portB->GetPin(GPIO_PIN1);
    res_pin->SetupGPIO_OutPP();
    res_pin->SetSpeedHigh();

    res_pin->Set();
    res_pin->Reset();

    clock_pin->Set();
    clock_pin->Reset();

    SS_pin->Set();
    SS_pin->Reset();

    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_64);
    spi->SetLSBFirst();

    sendFrequency(spi, 100.);

    SS_pin->Set();
    SS_pin->Reset();

    while(1)
        ;
}

int main()
{
	RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();

	GPIOA::GetInstance()->EnablePeripheralClock(true);
	GPIO_PIN* p = GPIOA::GetInstance()->GetPin(GPIO_PIN0);
	p->SetupGPIO_OutPP();
	p->Set();
	while(1)
		;


    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
    run2();
}
