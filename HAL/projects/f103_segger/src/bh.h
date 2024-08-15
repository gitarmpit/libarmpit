#ifndef _BH_H
#define _BH_H

#include "but.h"
#include "gpio_helper.h"
#include "systick_helper.h"
#include "timer_helper.h"

/* The default behavior is active low: button is configured as a weak pull-up
 * So to active it, the switch needs to be connected from the input port to ground
 * Then pushing the switch will set it low: the IsOn method will return true
 *
 */

void BH_Init (TIM_TypeDef* tim); 
void BH_AddButton(Button* button);

Button* BH_GetButton(uint8_t ID);

void BH_SetUpdateIntervalUs(uint32_t us);
void BH_SetSettleTimeUs(uint32_t us);

void BH_SetOnStateChange(Button* button);
void BH_SetOnButtonUp(Button* button);
void BH_SetOnButtonDown(Button* button);
void BH_SetOnClick(Button* button);
void BH_SetOnDoubleClick(Button* button);
void BH_SetTimerHook(Button* button);

// void SetTimerHookInterval (uint32_t ms) { _timerHookInterval = ms; }

#endif
