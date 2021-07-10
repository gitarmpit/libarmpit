#include "gpio.h"
#include <stdlib.h>
#include "sim.h"
#define PORT_PCR_OFFSET(port, pin) ((volatile uint32_t*)(port->PORT_PCR_BASE + (uint8_t)pin*4))

static GPIO_PORT GPIO_PORT_list[] =
{
	{ &SIM_SCGC5, SIM_SCGC5_PORTA, &GPIOA_PDOR, &GPIOA_PSOR, &GPIOA_PCOR, &GPIOA_PTOR, &GPIOA_PDIR, &GPIOA_PDDR, &PORTA_ISFR, PORTA_BASE },
	{ &SIM_SCGC5, SIM_SCGC5_PORTB, &GPIOB_PDOR, &GPIOB_PSOR, &GPIOB_PCOR, &GPIOB_PTOR, &GPIOB_PDIR, &GPIOB_PDDR, &PORTB_ISFR, PORTB_BASE },
	{ &SIM_SCGC5, SIM_SCGC5_PORTC, &GPIOC_PDOR, &GPIOC_PSOR, &GPIOC_PCOR, &GPIOC_PTOR, &GPIOC_PDIR, &GPIOC_PDDR, &PORTC_ISFR, PORTC_BASE },
	{ &SIM_SCGC5, SIM_SCGC5_PORTD, &GPIOD_PDOR, &GPIOD_PSOR, &GPIOD_PCOR, &GPIOD_PTOR, &GPIOD_PDIR, &GPIOD_PDDR, &PORTD_ISFR, PORTD_BASE },
	{ &SIM_SCGC5, SIM_SCGC5_PORTE, &GPIOE_PDOR, &GPIOE_PSOR, &GPIOE_PCOR, &GPIOE_PTOR, &GPIOE_PDIR, &GPIOE_PDDR, &PORTE_ISFR, PORTE_BASE },
};


GPIO_PORT* GPIO_GetInstance(GPIO_PORT_N port)
{
	return &GPIO_PORT_list[(uint8_t)port];
}

GPIO_PIN  GPIO_GetPin(GPIO_PORT* port, GPIO_PIN_N pinN)
{
	GPIO_PIN pin;
	pin.GPIO_port = port;
	pin.pinN = pinN;
	pin.pinMask = 1<<(uint8_t)pinN;
	pin.PORT_PCR = PORT_PCR_OFFSET(port, pinN);
	return pin;
}


void GPIO_EnableClock(GPIO_PORT* port, BOOL enable)
{
	SIM_Enable (port->SIM_enableReg, port->SIM_enableMask, enable);
}

BOOL GPIO_IsInterrupt(GPIO_PIN* pin)
{
	return *pin->PORT_PCR & PORT_PCR_ISF;
}

void GPIO_ClearInterrupt(GPIO_PIN* pin)
{
	*pin->PORT_PCR |= PORT_PCR_ISF;
}

void GPIO_SetSlewRateFast(GPIO_PIN* pin)
{
	*pin->PORT_PCR &= ~PORT_PCR_SRE;
}

void GPIO_SetSlewRateSlow(GPIO_PIN* pin)
{
	*pin->PORT_PCR |= PORT_PCR_SRE;
}

void GPIO_SetupOut(GPIO_PIN* pin)
{
	GPIO_SetAF(pin, 1);
	*pin->GPIO_port->GPIO_PDDR |= pin->pinMask;
}

void GPIO_SetupIn(GPIO_PIN* pin)
{
	GPIO_SetAF(pin, 1);
	*pin->GPIO_port->GPIO_PDDR &= ~pin->pinMask;
}

void GPIO_SetupAnalog(GPIO_PIN* pin)
{
	GPIO_SetAF(pin, 0);
}

void GPIO_SetPin(GPIO_PIN* pin)
{
	*pin->GPIO_port->GPIO_PSOR |= pin->pinMask;
}

void GPIO_ResetPin(GPIO_PIN* pin)
{
	*pin->GPIO_port->GPIO_PCOR |= pin->pinMask;
}

void GPIO_TogglePin(GPIO_PIN* pin)
{
	*pin->GPIO_port->GPIO_PTOR |= pin->pinMask;
}

BOOL GPIO_IsSet(GPIO_PIN* pin)
{
	return *pin->GPIO_port->GPIO_PDIR & pin->pinMask;
}

void GPIO_SetOutput(GPIO_PORT* port, uint32_t value)
{
	*port->GPIO_PSOR = value;
}

uint32_t GPIO_GetInput(GPIO_PORT* port)
{
	return *port->GPIO_PDIR;
}

void GPIO_PullDown(GPIO_PIN* pin)
{
	*pin->PORT_PCR |= PORT_PCR_PE;
	*pin->PORT_PCR &= ~PORT_PCR_PS;
}

void GPIO_PullUp(GPIO_PIN* pin)
{
	*pin->PORT_PCR |= PORT_PCR_PE;
	*pin->PORT_PCR |= PORT_PCR_PS;
}

void GPIO_SetAF(GPIO_PIN* pin, uint8_t alt)
{
	if (alt > 7)
	{
		while(1)
			;
	}
	*pin->PORT_PCR &= ~PORT_PCR_MUX_CLEARMASK;
	*pin->PORT_PCR |= (alt & 0x7)<<8;
}
