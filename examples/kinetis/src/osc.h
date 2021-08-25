#ifndef _OSC_H
#define _OSC_H

#include "common.h"

#ifndef OSC_BASE
#define OSC_BASE     0x40065000
#endif

#define OSC_CR    (*(volatile uint8_t*)(OSC_BASE + 0x0))

#define OSC_CR_ERCLKEN   (1<<7)
#define OSC_CR_EREFSTEN  (1<<5)
#define OSC_CR_SC2P      (1<<3)
#define OSC_CR_SC4P      (1<<2)
#define OSC_CR_SC8P      (1<<1)
#define OSC_CR_SC16P     (1<<0)

#ifdef __cplusplus
 extern "C" {
#endif

void OSC_Enable_OSERCCLK(BOOL enable);
void OSC_OSERCCLK_StopEnable(BOOL enable);

#ifdef __cplusplus
}
#endif


#endif
