#ifndef _BUT_H
#define _BUT_H

#ifndef _BUTTON_H
#define _BUTTON_H

#include "main.h"
#include "gpio_helper.h"

/* The default behavior is active low: button is configured as a weak pull-up
 * So to active it, the switch needs to be connected from the input port to ground
 * Then pushing the switch will set it low
 *
 */

typedef struct {
  GPIO_PIN* pin;

  BOOL  activeHigh;
  BOOL  currentState;     // high or low
  BOOL  lastReadValue;    // high or low

  uint8_t n_same_state;

  uint8_t  buttonId;
  uint32_t lastDown;
  uint32_t durDown;
  uint32_t lastClick;

} button_ctx;

//Button_Init(GPIO_PIN* pin, uint8_t buttonId);
button_ctx Button_Init (GPIO_PIN* pin, uint8_t buttonId);

// Open: HIGH
// Closed: LOW
// This is the default
void Button_PullUp(button_ctx* ctx);

// Open: LOW
// Closed: HIGH
void Button_PullDown(button_ctx* ctx);

BOOL Button_IsDown(button_ctx* ctx);
BOOL Button_IsUp(button_ctx* ctx);

/*
GPIO_PIN* GetPin() const {
  return _pin;
}

bool IsPinSet();

uint8_t GetId() {
  return _buttonId;
}

// timestamps
uint32_t GetLastDown() {
  return _lastDown;
}
uint32_t GetDurDown() {
  return _durDown;
}
*/


#endif

#endif