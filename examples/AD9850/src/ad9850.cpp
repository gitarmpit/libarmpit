#include "GPIO_Helper.h"
#include "RCC_Helper.h"
#include "debug.h"
#include "5110/Adafruit_Nokia5110.h"
#include "afio.h"
#include "adc_cpp.h"
#include "gpio_cpp.h"

#define AD9850_CLOCK 125000000

void send_byte(uint8_t data, GPIO_PIN* clock_pin, GPIO_PIN* data_pin)
{
    for (int i = 0; i < 8; i++, data >>= 1)
    {
        (data & 0x01) ? data_pin->Set() : data_pin->Reset();
        clock_pin->Set();
        delay_us(10); //40us worked
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
    //delay(1000);
    delay(100);
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

    res_pin->Set();
    res_pin->Reset();

    clock_pin->Set();
    clock_pin->Reset();
    SS_pin->Set();
    SS_pin->Reset();

    //delay(100);

    //SS_pin->Set();

    /*
    for (int i = 0; i < 10000; ++i)
    {
        SS_pin->Reset();
        sendFrequency(100 * i, clock_pin, data_pin);
        SS_pin->Set();
        SS_pin->Reset();
        SS_pin->Set();
        delay(100);
    }
    */
    sendFrequency(20000000, clock_pin, data_pin);
    SS_pin->Set();
    SS_pin->Reset();

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

static volatile uint32_t adc_val;


class ADC1_IRQ_Handler: public ADC_IRQ_Handler
{
private:

public:

    ADC1_IRQ_Handler()
    {
    }

    virtual void Handle(ADC* adc)
    {
        adc_val = adc->GetData();
    }
};

/*
extern "C" void ADC1_2_IRQHandler()
{
    static ADC1* adc1 = ADC1::GetInstance();

    if (ADC1_SR & ADC_SR_EOC)
    {
        adc_val = adc1->GetData();
        adc1->ClearInterrupt();
    }
}
*/

static void setup_ADC(ADC_IRQ_Handler* adcHandler)
{
    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN0);
    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->AddConversion(ADC_IN0, ADC_SMP_SLOWEST);
    adc1->EnableInterrupt(true);
    adc1->SetInterruptHandler(adcHandler);
    adc1->StartContinuousConversion();
//    while (1)
//    {
//        delay(10);
//    }
}


static void test_lcd()
{
    RCC_EnableAFIO(TRUE);
    AFIO_Remap_SWJ_CFG(false, true, true);

    //Too fast won't work, check clock and baud rate
    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PB_3_4_5, true, false, false, SPI_BAUD_RATE_16);

    GPIO_PIN* rstPin = GPIO_Helper::GetPin("B9");
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = GPIO_Helper::GetPin("B7");
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = GPIO_Helper::GetPin("B6");
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();


    Adafruit_Nokia5110 display(spi, dcPin, rstPin, ssPin);
    display.Init(0xbc);
    display.setRotation(2);

    ADC1_IRQ_Handler adcHandler;
    setup_ADC(&adcHandler);

    //display.setFont(&dos12);
    //double f = 4.12345;
    //display.printf(0, 1, 2, "%8.5f", f);
    while(1)
    {
        display.clearDisplay();
        display.printf (0, 0, "%4d", adc_val);
        display.display();
        delay(100);
    }

}

static void set (uint8_t val)
{

}

static void test_shift_reg()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);

    GPIO_PIN* sr_clk = GPIOA::GetInstance()->GetPin(GPIO_PIN1);
    sr_clk->SetupGPIO_OutPP();
    sr_clk->Reset();

    GPIO_PIN* ser = GPIOA::GetInstance()->GetPin(GPIO_PIN0);
    ser->SetupGPIO_OutPP();
    ser->Reset();

    GPIO_PIN* rclk = GPIOA::GetInstance()->GetPin(GPIO_PIN2);
    rclk->SetupGPIO_OutPP();
    rclk->Reset();

    //delay(100);

    for (int i = 0; i < 16; ++i)
    {
        sr_clk->Set();
        rclk->Set();
        //delay(1);
        sr_clk->Reset();
        rclk->Reset();
        //delay(1);
    }

    ser->Set();
    sr_clk->Set();
    rclk->Set();
    sr_clk->Reset();
    rclk->Reset();
    ser->Reset();
    delay(100);

    for (int i = 0; i < 16; ++i)
    {
        sr_clk->Set();
        rclk->Set();
        //delay(1);
        sr_clk->Reset();
        rclk->Reset();
        delay(100);
    }

    while(1)
        ;

}
int main()
{
	//RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();

	//GPIOA::GetInstance()->EnablePeripheralClock(true);
	//GPIO_PIN* p = GPIOA::GetInstance()->GetPin(GPIO_PIN0);
	//p->SetupGPIO_OutPP();
	//p->Set();
	//while(1)
	//	;

    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
    Debug_EnableCYCCNT(TRUE);
    //test_lcd();
    run2();
    //test_shift_reg();
}
