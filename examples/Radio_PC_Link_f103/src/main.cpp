#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "RF22/si4432.h"
#include "GPIO_Helper.h"
#include "usart_cpp.h"
#include "fp2.h"
#include "telemetry.h"
#include "fp2.h"
#include "system_time.h"
#include "debug.h"

static void radio_rx_loop()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->EnablePeripheralClock(true);

    uint32_t clockSpeed = 400000; //400k
    USART* usart = GPIO_Helper::SetupUSART(USART1_PA_9_10, clockSpeed);

    SPI* spi = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_32);
    GPIO_PIN* ssPin = GPIOB::GetInstance()->GetPin(GPIO_PIN2);
    ssPin->SetupGPIO_OutPP();
    GPIO_PIN* sdnPin = GPIOB::GetInstance()->GetPin(GPIO_PIN10);
    sdnPin->SetupGPIO_OutPP();

    volatile uint8_t len = 0;
    uint8_t data[64]={0};

    Si4432 radio(spi, ssPin, sdnPin, 0);
    bool rc = radio.init();
    if (!rc) while(1){}
    //assert (rc);
    radio.setBaudRate(50);  //kbs
    radio.setFrequency(433); //633

    telemetry tel, tel2;
    uint32_t cnt = 0;
    len = sizeof(tel);

    radio.startListening();
    radio.clearFIFO();

    tel.timestamp_ms = 0;
    uint32_t last_recv = 0;
    uint32_t timeout = 2000;
    uint16_t timeouts = 0;
    tel.throttle = 0;
    uint16_t delay_ms = 100;
    while (1)
    {
        if (usart->IsRXNE())
        {
            delay_ms = usart->GetDR()*100;
        }

        //start of message
        usart->SendByte(0xde);
        usart->SendByte(0xad);
        usart->SendByte(0xbe);
        usart->SendByte(0xef);

        for (uint8_t i = 0; i < len; ++i)
        {
            usart->SendByte(((uint8_t*)&tel)[i]);
        }
        uint32_t now = millis();
        if (radio.isPacketReceived())
        {
            radio.getPacketReceived((uint8_t*)&len, (uint8_t*)&tel);
            tel.timestamp_ms = ++cnt;
            tel.throttle = timeouts;
            //tel.y = - 2.345;
            radio.startListening();
            radio.clearFIFO();
            last_recv = now;
        }
        else if (last_recv != 0 && (now - last_recv) > timeout)
        {
            tel.throttle = ++timeouts;
            radio.init();
            delay(10);
            radio.startListening();
            radio.clearFIFO();
            last_recv = now;
        }

        delay(delay_ms);

    }

}


int main()
{
    RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();
    systick_enable(true);
    radio_rx_loop();

}
