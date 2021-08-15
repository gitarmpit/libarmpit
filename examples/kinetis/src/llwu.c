#include "llwu.h"
#include "sim.h"

static void LLWU_SetWakeupPin    (LLWU_PIN pin, uint8_t mask)
{
	volatile uint8_t* reg = &LLWU_PE1 + (int)pin/4;
	*reg |= mask << (((int)pin % 4)*2);
}

void LLWU_SetWakeupPin_Rising    (LLWU_PIN pin)
{
	LLWU_SetWakeupPin(pin, WUPEx_RISING);
}

void LLWU_SetWakeupPin_Falling    (LLWU_PIN pin)
{
	LLWU_SetWakeupPin(pin, WUPEx_FALLING);
}

void LLWU_SetWakeupPin_Any    (LLWU_PIN pin)
{
	LLWU_SetWakeupPin(pin, WUPEx_ANY);
}


void LLWU_SetWakeupModule (LLWU_MOD mod)
{
	LLWU_ME |= 1<<(uint8_t)mod;
}
