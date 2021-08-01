#ifndef __MAIN_H
#define __MAIN_H 


#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_rng.h"
#include "stm32f4xx_ll_pwr.h"
#define USE_TIMEOUT 0

#define LED1_PIN LL_GPIO_PIN_0
#define LED1_GPIO_PORT GPIOB
#define LED1_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)

#define LED_BLINK_FAST 200
#define LED_BLINK_SLOW 500
#define LED_BLINK_ERROR 1000

#define USER_BUTTON_PIN LL_GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC)
#define USER_BUTTON_EXTI_LINE LL_EXTI_LINE_13
#define USER_BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define USER_BUTTON_EXTI_LINE_ENABLE() LL_EXTI_EnableIT_0_31(USER_BUTTON_EXTI_LINE)
#define USER_BUTTON_EXTI_FALLING_TRIG_ENABLE() LL_EXTI_EnableFallingTrig_0_31(USER_BUTTON_EXTI_LINE)
#define USER_BUTTON_SYSCFG_SET_EXTI() do { \
                                                  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG); \
                                                  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13); \
                                                } while(0)
#define USER_BUTTON_IRQHANDLER EXTI15_10_IRQHandler
void UserButton_Callback(void);

#endif
