#include "system_control.h"
#include "sim.h"
#include "gpio.h"
#include "systick.h"
#include "mcg.h"
#include "rtc.h"
#include "osc.h"
#include "smc.h"
#include <time.h>

void HardFault_Handler()
{
	while(1)
		;
}

void SVC_Handler()
{
	while(1)
		;

}


GPIO_PIN* g_pin;

// static const uint8_t array[60000] = {0xdd, 0xcc, 0xaa};


static void test1()
{
	GPIO_PORT* portD = GPIO_GetInstance(PORTD);
	GPIO_PORT* portE = GPIO_GetInstance(PORTE);

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

void test_nokia5110();

extern "C" void test_exti();
extern "C" void test_lm74_2();
extern "C" void test_tpm_capture();
extern "C" void test_tpm_capture2();
extern "C" void test_gpio_helper_pwm();
extern "C" void test_gpio_helper_pwm2();
extern "C" void test_gpio_helper_pwm3();
extern "C" void test_gpio_helper_pwm4();

extern "C" void test_pwm3();

#ifdef SEMIHOSTING
extern "C" void initialise_monitor_handles(void);
#include <stdio.h>  //for printf
#endif

void test_tpm_cpp();
void test_tpm_capture_cpp1();
void test_button_handler();
extern "C" void test_systick_millis();
void test_rotary_handler();
extern "C" void test_adc();

int main(void)
{
#ifdef SEMIHOSTING
    initialise_monitor_handles();
#endif

	SIM_DisableWatchDog();
	//get_reset_source();
	//test_lptmr();
	//test_llwu();
	//test_spi();

	//test_exti();
	//test_tpm_pwm4();

	//test_tpm_capture();
	//test_tpm_capture_cpp1();

	//test_button_handler();
	test_adc();
	//test_rotary_handler();

	//test_systick_millis();

	//test_rotary_encoder();

	//test_lm74_2();
	//test_nokia5110();

	//test_uart6();
	//test_clockout_RTC_LPO_1hz();
	// test_clockout_OSCCLOCK32();
	//TestClockout3();
	//test_RTCIN();
	//pit_test1();
	//test_pwm2();
	//test_delay2();
	// delay_ms(2000);
	//test_lptmr3();
	//test_tpm_cpp();
	//test_pwm3();


	while(1)
		;
}
