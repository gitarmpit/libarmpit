#include "gpio.h"
#include <string.h>
#include <stdlib.h>
#include "stm32l0xx_ll_bus.h"

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))

void GPIO_SetAF(GPIO_PIN* pin, uint32_t af) {

  if (POSITION_VAL(pin->Pin) < 0x00000008U) {
    LL_GPIO_SetAFPin_0_7(pin->GPIOx, pin->Pin, af);
  } else {
    LL_GPIO_SetAFPin_8_15(pin->GPIOx, pin->Pin, af);
  }
}


GPIO_PIN GPIO_GetPin(const char *pin_name)
{
    GPIO_PIN pin = { NULL, 0 };

    static uint32_t pin_array[] = { LL_GPIO_PIN_0, LL_GPIO_PIN_1, LL_GPIO_PIN_2,
            LL_GPIO_PIN_3, LL_GPIO_PIN_4, LL_GPIO_PIN_5, LL_GPIO_PIN_6,
            LL_GPIO_PIN_7, LL_GPIO_PIN_8, LL_GPIO_PIN_9, LL_GPIO_PIN_10,
            LL_GPIO_PIN_11, LL_GPIO_PIN_12, LL_GPIO_PIN_13, LL_GPIO_PIN_14,
            LL_GPIO_PIN_15, };

    if (pin_name && strlen(pin_name) >= 2)
    {
        uint32_t port = 0;
        switch (pin_name[0])
        {
        case 'a':
        case 'A':
            pin.GPIOx = GPIOA;
            port = LL_IOP_GRP1_PERIPH_GPIOA;
            break;
        case 'b':
        case 'B':
            pin.GPIOx = GPIOB;
            port = LL_IOP_GRP1_PERIPH_GPIOB;
            break;
        case 'c':
        case 'C':
            pin.GPIOx = GPIOC;
            port = LL_IOP_GRP1_PERIPH_GPIOC;
            break;
        case 'd':
        case 'D':
            pin.GPIOx = GPIOD;
            port = LL_IOP_GRP1_PERIPH_GPIOD;
            break;
        default:
            break;
        };

        if (pin.GPIOx != NULL)
        {
            int pin_no = atoi(&pin_name[1]);
            if (pin_no < 0 || pin_no > 15)
            {
                pin.GPIOx = NULL;
            }
            else
            {
                pin.Pin = pin_array[pin_no];
                if (!LL_IOP_GRP1_IsEnabledClock(port))
                {
                    LL_IOP_GRP1_EnableClock(port);
                }
            }
        }
    }
    return pin;
}


