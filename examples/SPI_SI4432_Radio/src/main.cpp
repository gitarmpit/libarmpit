#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "RF22/si4432.h"
#include "GPIO_Helper.h"
#include "LED_Indicators.h"
#include "debug.h"
#include <stdio.h>

// Send
static void send()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->EnablePeripheralClock(true);

    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_32);
    GPIO_PIN* ssPin = GPIOA::GetInstance()->GetPin(GPIO_PIN4);
    GPIO_PIN* sdnPin = GPIOB::GetInstance()->GetPin(GPIO_PIN0);

    Si4432 radio(spi, ssPin, sdnPin, 0);
    radio.init();
    radio.setBaudRate(70);
    radio.setFrequency(433);
    radio.readAll();

    //TX
    uint8_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    while(1)
    {
        while (!radio.sendPacket(sizeof data, data))
            ;

        delay(2000);
    }
}

static void receive()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOC::GetInstance()->EnablePeripheralClock(true);
    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_32);
    GPIO_PIN* ssPin = GPIOA::GetInstance()->GetPin(GPIO_PIN4);
    GPIO_PIN* sdnPin = GPIOC::GetInstance()->GetPin(GPIO_PIN4);

    Si4432 radio(spi, ssPin, sdnPin, 0);
    bool rc = radio.init();
    if (!rc) {
    	while(1);
    }

    radio.setBaudRate(70);
    radio.setFrequency(433);
    radio.readAll();
    radio.SetGPIO();
    radio.ResetGPIO();

    //receiver

    radio.startListening();
    LED_Indicators::GetInstance()->RGB_SteadyGreen(1000);
    LED_Indicators::GetInstance()->RGB_Off();

    uint8_t data[64] = {0};
    uint8_t len = 0;

    while(1)
    {
        if (radio.isPacketReceived())
        {
            LED_Indicators::GetInstance()->RGB_SteadyRed(100);
            radio.getPacketReceived(&len, data);
#ifdef SEMIHOSTING
            printf ("%d bytes received\n", len);
            for (int i = 0; i < len; ++i) {
            	printf ("%d ", data[i]);
            }
            printf ("\n");
#endif
            LED_Indicators::GetInstance()->RGB_Off();
            radio.startListening();
        }

        delay(10);
    }


}

#ifdef SEMIHOSTING
extern "C" void initialise_monitor_handles(void);
#endif

int main()
{
#ifdef SEMIHOSTING
    initialise_monitor_handles();
 #endif

#ifdef STM32F1
	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#else
	RCC_EnableHSI_168Mhz();
#endif
	Debug_EnableCYCCNT(TRUE);
    receive();
}

