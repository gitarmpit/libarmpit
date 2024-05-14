#include "I2C_Nau7802.h"
#include "RCC_Helper.h"
#include "debug.h"
#include "GPIO_Helper.h"

void i2c_test_24bit_adc()
{
	I2C* i2c = GPIO_Helper::SetupI2C(I2C1_PB_6_7, 100000);
    I2C_Nau7802 nau7802(i2c);
    nau7802.InitializeSensor();
    static volatile uint32_t result;

    while (1)
    {
        result = nau7802.Convert();
        delay(400);
    }
}



int main()
{
#ifdef STM32F4
	RCC_EnableHSI_168Mhz();
#else
	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#endif
	Debug_EnableCYCCNT(TRUE);
    i2c_test_24bit_adc();
}
