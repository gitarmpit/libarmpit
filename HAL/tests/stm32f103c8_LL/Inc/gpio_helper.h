#ifndef GPIO_HELPER_H
#define GPIO_HELPER_H

#include "stm32f1xx_ll_gpio.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint32_t Pin;
} GPIO_PIN;


GPIO_PIN GPIO_GetPin (const char* pin_name);

// void GPIO_Setup_OutPP (GPIO_PIN* pin);

#define GPIO_Setup_OutPP(pin) \
    do { \
        LL_GPIO_SetPinMode((pin)->GPIOx, (pin)->Pin, LL_GPIO_MODE_OUTPUT); \
        LL_GPIO_SetPinSpeed((pin)->GPIOx, (pin)->Pin, LL_GPIO_SPEED_FREQ_HIGH); \
        LL_GPIO_SetPinOutputType((pin)->GPIOx, (pin)->Pin, LL_GPIO_OUTPUT_PUSHPULL); \
    } while (0)


#ifdef __cplusplus
}
#endif


#endif
