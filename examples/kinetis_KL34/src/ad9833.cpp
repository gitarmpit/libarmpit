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

class AD9833
{
private:
    GPIO_PIN data;
    GPIO_PIN clk;
    GPIO_PIN ss;
    uint16_t ctrl;

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
            GPIO_SetPin(&clk);
        }
        GPIO_SetPin(&ss);
    }


    void SendData14(uint16_t val)
    {
        val &= ~AD9833_D15;
        val |= AD9833_D14;
        Send16(val);
    }

    void SendData(uint32_t data)
    {
        SendData14(data & 0x3fff);
        SendData14((data>>14) & 0x3fff);
    }


public:
    AD9833()
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
        ctrl = AD9833_B28;

	}

    void SetSine()
    {
        ctrl &= ~(AD9833_D1 & AD9833_D5);
        Send16(ctrl);
    }

    void SetTriangle()
    {
    	ctrl &= ~AD9833_D5;
        ctrl |= AD9833_D1;
        Send16(ctrl);
    }

    void SetPWM()
    {
    	ctrl &= ~AD9833_D1;
        ctrl |= AD9833_D5;
        Send16(ctrl);
    }

    void Reset()
    {
        ctrl |= AD9833_RST;
        Send16(ctrl);
    }

    void Enable()
    {
        ctrl &= ~AD9833_RST;
        Send16(ctrl);
    }


    void SetFreq (uint32_t freq)
    {
        uint32_t reg = (uint32_t)((uint64_t)AD9833_2_28 * (uint64_t)freq / F_MCLK);
        SendData(reg);
    }

};

void ad9833()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();

	AD9833 ad9833;

	ad9833.Reset();
	ad9833.SetPWM();
	//ad9833.SetTriangle();
	ad9833.SetFreq(1000);
	ad9833.Enable();

	int i = 1;
    while(1)
    {
    	delay_ms(1000);
    	ad9833.SetFreq(100000*i++);
    }

}
