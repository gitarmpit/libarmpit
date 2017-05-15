#include <common.h>
#include <exti_cpp.h>
#include <gpio.h>
#include <gpio_cpp.h>
#include <rcc.h>
#include <RCC_Helper.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <timer_cpp.h>
#include <spi_cpp.h>
#include "GPIO_Helper.h"
#include "debug.h"
#include "PWM_Capture/PWM_Capture_EXTI.h"
#include  "_pins.h"

static void run()
{
	TIMER_With_Channels* triggerTimer;
	TIMER_Channel* out_ch;

	triggerTimer = TIM2::GetInstance();
	triggerTimer->EnablePeripheralClock(true);

	//trigger
	GPIO_PIN* triggerPin = GPIO_Helper::GetPin(TRIGGER_PIN); //PWM output
	triggerPin->SetupGPIO_OutAltPP();

	out_ch = triggerTimer->GetChannel(2);
	out_ch->SetupPWM1(70000, 100);
	//out_ch->SetupPWM1(30, 10);
	out_ch->EnableChannel(true);
	//outtimer->EnableCounter(true);

	//RCC_EnableAFIO(TRUE);
	//Remap LINE5 to PE5
	//AFIO_RemapEXTI(EXTI_LINE5, PORTE);

//	GPIO_PIN* readPin = GPIO_Helper::GetPin(READ_PIN);
//	readPin->SetupGPIO_InPullDown();

	PWM_Capture_EXTI reader(TIM3::GetInstance(), READ_PIN, 0);

//    reader.Start();

	//Connector pinout
	//red Vcc, blue Vss, yellow - clock, green - MISO, white - SS, white - MOSI
	GPIOB::GetInstance()->EnablePeripheralClock(true);
	GPIO_PIN* pin = GPIOB::GetInstance()->GetPin(GPIO_PIN11);
	pin->SetupGPIO_OutPP();
//    pin->Set();
//    delay(2000);
//    pin->Reset();

	//Slave SPI
	SPI* spi_slave = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, false, false, false, SPI_BAUD_RATE_16);

	volatile uint8_t recv;
	uint16_t snd = 0;
	bool running = false;
	uint8_t div = 58;
	bool ledon = true;
	while (1)
	{
		snd = reader.GetDuty_us() / div;
		if (div == 58 && snd > 255)
		{
			snd = 0;
		}

		recv = spi_slave->TransmitByte(snd & 0xff);

		if (recv == 0x1 && !running)
		{
			triggerTimer->EnableCounter(true);
			reader.Start();
			running = true;
		}
//        else if (recv == 0x2 && running)
//        {
//            triggerTimer->EnableCounter(false);
//            reader.Stop();
//            running = false;
//        }
		else if (recv == 0x3)
		{
			div = 58;  //inches
		}
		else if (recv == 0x4)
		{
			div = 148;  //cm
		}
		else if (recv == 0x5)
		{
			ledon = false;
		}

		if (running && ledon)
		{
			pin->Toggle();
		}
	}

}

int main()
{

	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
	run();

}
