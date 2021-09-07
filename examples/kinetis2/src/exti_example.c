#include "gpio.h"
#include "tpm.h"
#include "sim.h"
#include "mcg.h"

int cnt = 0;
GPIO_PIN* exti_pin;
GPIO_PIN  led_pin;

#pragma GCC diagnostic ignored "-Wunused-function"


static void handler(GPIO_PORT_N port, GPIO_PIN_N pin)
{
	if (port == PORTA && pin == GPIO_PIN13 && GPIO_IsSet(exti_pin))
	{
		++cnt;
		GPIO_TogglePin(&led_pin);
	}
}

static void generatePWM()
{
	InitClock_FBI_Slow();
	MCG_Enable_MCGIRCLK(TRUE);
	SIM_Select_TPMSRC_MCGIRCLK();

	//InitClock_FEI_24Mhz_Bus_24Mhz();
	//SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;

	SIM_Select_FLL();

    GPIO_PORT* portA = GPIO_GetInstance(PORTA);
    GPIO_EnableClock(portA, TRUE);
    GPIO_PIN a5 = GPIO_GetPin(portA, GPIO_PIN5);
    GPIO_SetAF(&a5, 3);

	TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
	TPM_EnableClock(tpm, TRUE);
	int period_ms = 2000;
	int duty_ms = 1000;
	TPM_Channel_t* ch = TPM_GetChannel(tpm, 2);
	TPM_Channel_SetupPWM(ch, period_ms * 1000, duty_ms * 1000);
	TPM_EnableCounter(tpm, TRUE);
}

void test_exti()
{
	generatePWM();
	GPIO_PORT* portA = GPIO_GetInstance(PORTA);
	GPIO_EnableClock(portA, TRUE);
	GPIO_PIN pin = GPIO_GetPin(portA, GPIO_PIN13);
	GPIO_SetupIn(&pin);
	GPIO_PullUp(&pin);
	exti_pin = &pin;
	led_pin = GPIO_GetPin(portA, GPIO_PIN12);
	GPIO_SetupOut(&led_pin);

	GPIO_SetInterruptHandler(portA, handler);
	GPIO_EnableInterruptEitherEdge(&pin);
	while(1)
		;
}


#if 0


class EXTI15_Handler2: public EXTI_IRQ_Handler
{
public:
    //toggles LED on pin25 every second
    void HandleInterrupt(EXTI* exti)
    {
        GPIOB::GetInstance()->TogglePin(GPIO_PIN12);
    }
};

static void exti_input_pwm_test()
{
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    //LED on pin25
    GPIOB::GetInstance()->SetupGPIO_OutPP(GPIO_PIN12);

    //Output PWM signal with 1 sec period on pin10 
    //Connect pin10 to the EXTI input on pin28
    generatePWM();

    RCC::GetInstance()->EnableAFIO(true);
    
    //Remap LINE15 to PB15 : pin28
    AFIO_RemapEXTI(EXTI_LINE15, PORTB);

    
    EXTI15::GetInstance()->EnableInterruptRisingEdge(true);

    EXTI15_Handler2 handler;
    EXTI15::GetInstance()->SetIrqHandler(&handler);
    while (1)
    {
    }
}

#endif
