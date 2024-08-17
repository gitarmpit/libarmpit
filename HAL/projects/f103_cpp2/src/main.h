#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint8_t BOOL; 

#define TRUE 1 
#define FALSE 0

void System_Config(void);
void SystemClock_Config_HSE(void);
void SystemClock_Config_HSI(void);


#define SW1_Pin        LL_GPIO_PIN_9
#define SW1_GPIO_Port  GPIOB

#define LED1_Pin LL_GPIO_PIN_8
#define LED1_GPIO_Port GPIOB


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
