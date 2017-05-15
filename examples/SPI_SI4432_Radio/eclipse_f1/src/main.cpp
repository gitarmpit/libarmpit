#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "RF22/si4432.h"
#include "GPIO_Helper.h"
#include "debug.h"


static void main_loop ()
{

}

static void test()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->EnablePeripheralClock(true);

//  SPI* spi = GPIO_Helper::SetupSPI2_MasterMode_PB_13_14_15(false, false, SPI_BAUD_RATE_32);
//  GPIO_PIN* ssPin = GPIOB::GetInstance()->GetPin(GPIO_PIN12);
//  GPIO_PIN* sdnPin = GPIOB::GetInstance()->GetPin(GPIO_PIN11);

    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_32);
    GPIO_PIN* ssPin = GPIOA::GetInstance()->GetPin(GPIO_PIN4);
    GPIO_PIN* sdnPin = GPIOB::GetInstance()->GetPin(GPIO_PIN0);


    Si4432 radio(spi, ssPin, sdnPin, 0);
    radio.init();
    radio.setBaudRate(70);
    radio.setFrequency(433);
    radio.readAll();

    //TX
    uint8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    while(1)
    {
        while (!radio.sendPacket(sizeof data, data))
            ;

        delay(2000);
    }


}

static void test2()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->EnablePeripheralClock(true);

    SPI* spi = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_32);
    GPIO_PIN* ssPin = GPIOB::GetInstance()->GetPin(GPIO_PIN2);
    ssPin->SetupGPIO_OutPP();
    GPIO_PIN* sdnPin = GPIOB::GetInstance()->GetPin(GPIO_PIN10);
    sdnPin->SetupGPIO_OutPP();



    Si4432 radio(spi, ssPin, sdnPin, 0);
    radio.init();
    radio.setBaudRate(30);  //kbs
    radio.setFrequency(433);
    radio.readAll();

    //TX
    uint8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    while(1)
    {
        while (!radio.sendPacket(sizeof data, data))
            ;

        delay(1000);
    }


}


int main()
{
    RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();
    test2();

}
