#include "gpio_helper.h"
#include "system_time.h"
#include "mcg.h"
#include "spi.h"

#define F_MCLK 25000000
#define AD9833_B28  (1<<13)
#define AD9833_D15  (1<<15)
#define AD9833_D14  (1<<14)
#define AD9833_RST  (1<<8)
#define AD9833_D5  (1<<5)
#define AD9833_D1  (1<<1)
#define AD9833_2_28 268435456

class DAC121
{
private:
    GPIO_PIN data;
    GPIO_PIN clk;
    GPIO_PIN ss;

    static void delay(int cnt)
    {
    	while(cnt--)
    		;
    }

public:

    void Send16(uint16_t val)
    {
        GPIO_ResetPin(&ss);
        for (int i = 15; i >=0; --i)
        {
            if (val & (1<<i))
            {
                GPIO_SetPin(&data);
            }
            else
            {
                GPIO_ResetPin(&data);
            }
            GPIO_ResetPin(&clk);
            delay(10000);
            GPIO_SetPin(&clk);
        }
        GPIO_SetPin(&ss);
    }

    void Set(double v)
    {
    	double vmax = 3.25;
    	if (v > vmax)
    	{
    		v = vmax;
    	}
    	double res = vmax / 4095;
    	double val = v / res;
    	Send16 ((uint16_t) val);
    }


public:
    DAC121()
	{
        data = GPIO_Helper_GetPin("E0");
        clk = GPIO_Helper_GetPin("E1");
        ss = GPIO_Helper_GetPin("E16");

        GPIO_SetupOut(&data);
        GPIO_SetupOut(&clk);
        GPIO_SetupOut(&ss);

        GPIO_ResetPin(&data);

        GPIO_SetPin(&clk);

        GPIO_SetPin(&ss);

	}



};

void dac121()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();

	DAC121 dac121;

	dac121.Send16(0x3000);
	dac121.Send16(0x1000);
	dac121.Send16(0x2000);

	dac121.Set(0);
	dac121.Set(0.1);
	dac121.Set(0.2);
	dac121.Set(1.625);
	dac121.Set(3);
	dac121.Set(3.25);

    while(1)
    {
    	delay_ms(1000);
    }

}
