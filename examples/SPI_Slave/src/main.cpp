#include "spi_cpp.h"
#include "GPIO_Helper.h"
#include "RCC_Helper.h"
#include "debug.h"
#include "timer_cpp.h"

SPI* spi_master;



class Update_Handler: public TIMER_Interrupt_Handler
{
public:

    virtual void HandleInterrupt(bool isUpdate, bool isTrigger)
    {
        static volatile uint8_t  recv = 0;
        static volatile uint8_t snd = 0;
    	spi_master->TransmitByte(snd++);
    }
};


static void setup_timer()
{
    Update_Handler handler;
    TIMER* t = TIM3::GetInstance();
    t->EnablePeripheralClock(true);
    t->EnableNVICInterrupt(true);     //enable global system interrupts (core)
    t->AddInterruptHandler(&handler);
    t->EnableUpdateInterrupt(true);   //enable timer update interrupt
    t->SetCounterValue(0);            //optional
    t->SetUpdatePeriod_us(1000000);   //1 second
    t->EnableCounter(true);           //start counting
}


static void test_spi_slave()
{
	//Master
    spi_master = GPIO_Helper::SetupSPI(SPI1_PB_3_4_5, true, false, false, SPI_BAUD_RATE_16);

    setup_timer();

    //Connector pinout
    //red Vcc, blue Vss, yellow - clock, green - MISO, white - SS, white - MOSI
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    GPIO_PIN* pin = GPIOB::GetInstance()->GetPin(GPIO_PIN11);
    pin->SetupGPIO_OutPP();
    pin->Set();
    delay(2000);
    pin->Reset();

    SPI* spi_slave = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, false, false, false, SPI_BAUD_RATE_16);

    volatile uint8_t  recv = 0;
    volatile uint8_t snd = 0;
    while(1)
    {
        recv = spi_slave->TransmitByte(snd++);
        pin->Toggle();
    }
}


int main()
{
#ifdef STM32F1
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#else
    RCC_EnableHSI_100Mhz();
#endif

    test_spi_slave();
}
