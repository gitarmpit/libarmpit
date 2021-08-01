#ifndef __MAIN_H
#define __MAIN_H 

#include "stm32f4xx_hal.h"
#include "stm32446e_eval.h"

#define ADCx ADC1
#define ADCx_CLK_ENABLE() __HAL_RCC_ADC1_CLK_ENABLE()
#define DMAx_CLK_ENABLE() __HAL_RCC_DMA2_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define ADCx_FORCE_RESET() __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET() __HAL_RCC_ADC_RELEASE_RESET()

#define ADCx_CHANNEL_PIN GPIO_PIN_4
#define ADCx_CHANNEL_GPIO_PORT GPIOA

#define ADCx_CHANNEL ADC_CHANNEL_4

#define ADCx_DMA_CHANNEL DMA_CHANNEL_0
#define ADCx_DMA_STREAM DMA2_Stream0

#define ADCx_DMA_IRQn DMA2_Stream0_IRQn
#define ADCx_DMA_IRQHandler DMA2_Stream0_IRQHandler


#endif
