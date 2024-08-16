
#ifndef _BH_H
#define _BH_H

#include "but.h"
#include "stm32f103xb.h"
#include <stdint.h>

/* The default behavior is active low: button is configured as a weak pull-up
 * So to active it, the switch needs to be connected from the input port to ground
 * Then pushing the switch will set it low: the IsOn method will return true
 *
 */

 #ifdef __cplusplus
extern "C" {
#endif

typedef void (*button_handler)(void*);

void BH_Init (TIM_TypeDef* tim); 
void BH_AddButton(button_ctx* button);

button_ctx* BH_GetButton(uint8_t ID);

void BH_SetUpdateIntervalUs(uint32_t us);
void BH_SetSettleTimeUs(uint32_t us);

void BH_SetOnStateChangeHandler(button_handler on_state_chage);
void BH_SetOnButtonUpHandler(button_handler on_up);
void BH_SetOnButtonDownHandler(button_handler on_down);
void BH_SetOnClickHandler(button_handler on_click);
void BH_SetOnDoubleClickHandler(button_handler on_double_click);
void BH_SetTimerHookHandler(button_handler timer_hook_handler);

// void SetTimerHookInterval (uint32_t ms) { _timerHookInterval = ms; }

#ifdef __cplusplus
}
#endif

#endif
