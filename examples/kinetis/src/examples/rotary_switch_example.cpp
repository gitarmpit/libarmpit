#include "gpio.h"
#include "gpio_helper.h"
#include "sim.h"
#include "mcg.h"
#include "pit.h"
#include <stdio.h>

static GPIO_PIN g_pin;

class A
{
public:
	void f()
	{
		printf ("A::f\n");
	}
};

static void PIT_InterruptHandler2(void* ctx)
{
	//GPIO_PIN* pin = (GPIO_PIN*)ctx;
	// GPIO_TogglePin(&g_pin);
	//GPIO_TogglePin(pin);
	A* a = (A*)ctx;
	a->f();
}


void test_rotary_encoder()
{
	InitClock_FBI_Slow();
	MCG_Enable_MCGIRCLK(TRUE);

	A a;

	GPIO_PIN pin = GPIO_Helper_GetPin("D7");
	GPIO_SetupOut(&pin);

	SIM_Enable_PIT(TRUE);
	PIT_EnableClock(TRUE);
	PIT* pit = PIT_GetInstance(PIT0);
	PIT_SetPeriod_us(pit, 1000000);
	pit->interrupt_handler = PIT_InterruptHandler2;
	pit->ctx = &a;
	PIT_EnableInterrupt(pit, TRUE);
	PIT_EnableTimer(pit, TRUE);
	while(1)
		;


}
