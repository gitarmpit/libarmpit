#ifndef _SYSCFG_H
#define _SYSCFG_H

#include "common.h"
#include "exti.h"
#include "gpio.h"

#define SYSCFG_BASE  0x40013800

#define SYSCFG_EXTICR1  (*(volatile uint32_t*)(SYSCFG_BASE + 0x08))

#endif

void SYSCFG_RemapEXTI(EXTI_LINE line, GPIO_PORT_N port);
