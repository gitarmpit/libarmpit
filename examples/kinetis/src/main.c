#include "system_control.h"
#include "sim.h"
#include "gpio.h"
#include "systick.h"
#include "mcg.h"
#include "rtc.h"
#include "osc.h"
#include <time.h>

void HardFault_Handler()
{
	while(1)
		;
}

// #define NOP() __asm volatile("mov r0, r0")
#define NOP() __asm volatile("nop")

__attribute__( ( always_inline ) ) static inline void __nop()
{
  __asm volatile ("nop");
}

// extern "C" void SystemInit()
void SystemInit()
{
	// __asm volatile("mov r0, r0");
}


#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif
static void delay_ms (int ms) {
	//ms = 7200000;
	//ms =  400000;
	ms = CORE_FREQ/10;
  while (ms--) { __nop(); __nop(); __nop(); __nop(); __nop(); NOP(); }
}

#ifdef __GNUC__
#pragma GCC pop_options
#endif


GPIO_PIN* g_pin;

// static const uint8_t array[60000] = {0xdd, 0xcc, 0xaa};



#ifdef SEMIHOSTING
void initialise_monitor_handles(void);
#include <stdio.h>  //for printf
#endif

static void test1()
{
	GPIO_PORT* portD = GPIO_GetInstance(PORTD);
	GPIO_PORT* portE = GPIO_GetInstance(PORTE);
	int sz = sizeof(GPIO_PORT);

	MCG_SetFLL_Freq_72(TRUE);
	CORE_FREQ = 72000000;

	// SIM_Select_UART0SRC_MCGIRCLK();
	GPIO_EnableClock(portE, TRUE);
	GPIO_EnableClock(portD, TRUE);

	GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
	GPIO_SetupOut(&e0);
	GPIO_SetSlewRateFast(&e0);
	GPIO_SetPin(&e0);
	//GPIO_ResetPin(&e0);
	//GPIO_TogglePin(&e0);

	GPIO_PIN d7 = GPIO_GetPin(portD, GPIO_PIN7);
	GPIO_SetupIn(&d7);

	BOOL isSet = GPIO_IsSet(&d7);
	GPIO_PullDown(&d7);
	isSet = GPIO_IsSet(&d7);
	GPIO_PullUp(&d7);
	isSet = GPIO_IsSet(&d7);
	isSet = GPIO_IsSet(&d7);
	isSet = GPIO_IsSet(&d7);
	

	//SIM_SetOUTDIV1(4);
	//CORE_FREQ /= 4;

	g_pin = &e0;
	//SystickEnable((uint64_t)10 * CORE_FREQ/16/1000000, TRUE, TRUE, MySysTickHandler);
	//SystickEnable(CORE_FREQ/16/100, TRUE, TRUE, MySysTickHandler);
	//SystickEnable(1 * CORE_FREQ/1000000, TRUE, FALSE, MySysTickHandler);
	//SystickEnable(18, TRUE, FALSE, MySysTickHandler);
	while(1)
		;

}

static void test2()
{
	uint16_t i_id = 0, v_id = 0, a_id = 0, part_id = 0, rev_id = 0;
	BOOL be = FALSE;
	SCB_ReadCPUID(&i_id, &v_id, &a_id, &part_id, &rev_id, &be);
	uint8_t famID, subfamID, seriesID, sramSize, revID, dieID, pinID;
	SIM_Get_SDID(&famID, &subfamID, &seriesID, &sramSize, &revID, &dieID, &pinID);
	uint32_t low, mid, high;
	SIM_GetUID(&low, &mid, &high);

}


void test_clockout_RTC()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);
	//MCG_SetFLL_Freq_48(TRUE);

	GPIO_PORT* portE = GPIO_GetInstance(PORTE);
	GPIO_EnableClock(portE, TRUE);
	GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
	GPIO_SetAF(&e0, 4);
	OSC_Enable_OSERCCLK(TRUE);
	SIM_Select_RTCClock_OSC32KCLK();
	// SIM_Select_RTCClock_LPO();
	SIM_Enable_RTC(TRUE);
	RTC_CR |= RTC_CR_SC16P;
	RTC_EnableOSC(TRUE);
	RTC_IER = 0;
	RTC_EnableCounter(TRUE);

	// 1Hz
	SIM_Select_RTC_CLOCKOUT_RTC();

	// 32Khz
	// SIM_Select_RTC_CLOCKOUT_OSCERCLK();

	while(1)
		;

}

int main(void)
{
//#ifdef SEMIHOSTING
 //   initialise_monitor_handles();
//#endif

	SIM_DisableWatchDog();
	test_systick1();

	// delay_ms(2000);
	test_clockout_FLL_OSC32();


	// SIM_SetOUTDIV4(2);

	// 4 Mhz
	//MCG_Set_FCRDIV1();
	//CORE_FREQ = 4000000;

	// 32Khz (slow clock is the default IRC
	MCG_Set_IRC_Slow();
	CORE_FREQ = 32768;

	MCG_Select_MCGOUTCLK_IRC();

	// The default
	MCG_Set_PLLS_FLL();
	//MCG_Set_PLLS_PLL();

	// Clock option for some peripherals
	// MCG_Set_MCGIRCLK();

	// The default
	//MCG_Select_MCGOUTCLK_FLL_PLL();

	// The default
	//SIM_Select_FLL();
	//SIM_Select_PLL();

	int i = 0;
	for(int i = 0; i < 30; ++i)
	{
	    delay_ms(1000);
	    //printf ("test semihosting: %d\n", ++i);
	}


	SystickEnable(1000, TRUE, TRUE, NULL);

	while(1)
		;
}
