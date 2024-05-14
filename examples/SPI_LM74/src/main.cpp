#include "gpio_cpp.h"
#include "LM74.h"
#include "GPIO_Helper.h"
#include "RCC_Helper.h"
#include "debug.h"
#include <stdio.h>

static void temp_lm74_test() {

	SPI* spi1 = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_128);

	GPIO_PIN* ssPin = GPIO_Helper::GetPin("A4");
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

	LM74 lm74(spi1, ssPin);
	lm74.Init();

	uint16_t code = lm74.GetProductCode();
#ifdef SEMIHOSTING
	printf ("product code: %d\n", code);
#endif

	//lm74.Shutdown();

	volatile float t;

	while (true) {
		t = lm74.GetTemp();
		delay(1000);
#ifdef SEMIHOSTING
		printf ("%f\n", t);
#endif
	}

}

#ifdef SEMIHOSTING

extern "C" void initialise_monitor_handles(void);

#endif

int main() {
#ifdef SEMIHOSTING
    initialise_monitor_handles();
 #endif

#ifdef STM32F1
	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#else
	RCC_EnableHSI_168Mhz();
#endif

	Debug_EnableCYCCNT(TRUE);
	temp_lm74_test();
}
