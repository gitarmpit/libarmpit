#ifndef _GPIO_H
#define _GPIO_H

#include "stm32f1xx_ll_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  GPIO_TypeDef *GPIOx;
  uint32_t Pin;
} GPIO_PIN;

GPIO_PIN GPIO_GetPin(const char *pin_name);

#define GPIO_Setup_OutPP(pin)                                                    \
  do {                                                                           \
    LL_GPIO_SetPinMode((pin)->GPIOx, (pin)->Pin, LL_GPIO_MODE_OUTPUT);           \
    LL_GPIO_SetPinSpeed((pin)->GPIOx, (pin)->Pin, LL_GPIO_SPEED_FREQ_HIGH);      \
    LL_GPIO_SetPinOutputType((pin)->GPIOx, (pin)->Pin, LL_GPIO_OUTPUT_PUSHPULL); \
  } while (0)

#define GPIO_Setup_OutAltPP(pin)                                                    \
  do {                                                                           \
    LL_GPIO_SetPinMode((pin)->GPIOx, (pin)->Pin, LL_GPIO_MODE_ALTERNATE);           \
    LL_GPIO_SetPinSpeed((pin)->GPIOx, (pin)->Pin, LL_GPIO_SPEED_FREQ_HIGH);      \
    LL_GPIO_SetPinOutputType((pin)->GPIOx, (pin)->Pin, LL_GPIO_OUTPUT_PUSHPULL); \
  } while (0)

#define GPIO_Setup_Floating(pin) \
  LL_GPIO_SetPinMode((pin)->GPIOx, (pin)->Pin, LL_GPIO_MODE_FLOATING)


#define GPIO_Setup_In_Pulldown(pin)                                   \
  do {                                                                \
    LL_GPIO_SetPinMode((pin)->GPIOx, (pin)->Pin, LL_GPIO_MODE_INPUT); \
    LL_GPIO_SetPinPull((pin)->GPIOx, (pin)->Pin, LL_GPIO_PULL_DOWN);  \
  } while (0)

#define GPIO_Setup_In_Pullup(pin)                                     \
  do {                                                                \
    LL_GPIO_SetPinMode((pin)->GPIOx, (pin)->Pin, LL_GPIO_MODE_INPUT); \
    LL_GPIO_SetPinPull((pin)->GPIOx, (pin)->Pin, LL_GPIO_PULL_UP);    \
  } while (0)

#define GPIO_SetPin(pin) \
  LL_GPIO_SetOutputPin((pin)->GPIOx, (pin)->Pin)

#define GPIO_ResetPin(pin) \
  LL_GPIO_ResetOutputPin((pin)->GPIOx, (pin)->Pin)

#define GPIO_TogglePin(pin) \
  LL_GPIO_TogglePin((pin)->GPIOx, (pin)->Pin)

#define GPIO_IsSet(pin) \
  LL_GPIO_IsInputPinSet((pin)->GPIOx, (pin)->Pin)

#ifdef __cplusplus
}
#endif

#endif
