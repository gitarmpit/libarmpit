#ifndef TIMER_HELPER_H
#define TIMER_HELPER_H

#include "stm32f7xx_ll_tim.h"
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef void (*tim_handler)(void*);

extern tim_handler  tim1_handler;
extern tim_handler  tim2_handler;
extern tim_handler  tim3_handler;
extern tim_handler  tim4_handler;
extern tim_handler  tim5_handler;
extern tim_handler  tim6_handler;
extern tim_handler  tim7_handler;
extern tim_handler  tim8_handler;
extern tim_handler  tim9_handler;
extern tim_handler  tim10_handler;
extern tim_handler  tim11_handler;
extern tim_handler  tim12_handler;
extern tim_handler  tim13_handler;
extern tim_handler  tim14_handler;

typedef struct {
  TIM_TypeDef *tim;
  uint8_t channel;
} TIM_Channel;

#define IS_APB1(tim)    ((uint32_t)tim < APB2PERIPH_BASE)

#define TIM_DisableTimer(tim) LL_TIM_DisableCounter(tim)
#define TIM_EnableTimer(tim)  LL_TIM_EnableCounter(tim)

void TIM_SetUpdatePeriod_us (TIM_TypeDef *timer, uint32_t us);
void TIM_SetUpdatePeriod_ns (TIM_TypeDef *timer, uint32_t ns);

void TIM_SetupCounter(TIM_TypeDef *timer, uint32_t period_us, tim_handler th, void* ctx);
void TIM_SetHandler(TIM_TypeDef *timer, tim_handler th, void* ctx);


////////////////////////////////////////////////////////////////////////////////
// PWM

TIM_Channel TIM_SetupPWM(TIM_TypeDef* tim, uint8_t channel, uint32_t period_us, uint32_t ds_us);

void TIM_UpdateDs (TIM_Channel* ch, uint32_t ds_us);
void TIM_UpdatePeriodDs(TIM_Channel* ch, uint32_t period_us, uint32_t ds_us);

/*
void TIM_SetupPWM_TIM1_A8(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM1_A9(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM1_A10(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM1_A11(uint32_t period_us, uint32_t ds_us);

void TIM_SetupPWM_TIM2_A0(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM2_A1(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM2_A2(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM2_A3(uint32_t period_us, uint32_t ds_us);

void TIM_SetupPWM_TIM2_A15(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM2_B3(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM2_B10(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM2_B11(uint32_t period_us, uint32_t ds_us);

void TIM_SetupPWM_TIM3_A6(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM3_A7(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM3_B0(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM3_B1(uint32_t period_us, uint32_t ds_us);

void TIM_SetupPWM_TIM3_B4(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM3_B5(uint32_t period_us, uint32_t ds_us);

void TIM_SetupPWM_TIM3_C6(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM3_C7(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM3_C8(uint32_t period_us, uint32_t ds_us);
void TIM_SetupPWM_TIM3_C9(uint32_t period_us, uint32_t ds_us);


void TIM_SetupPWM_OnPin(const char* pin_name, uint32_t period_us, uint32_t ds_us);
*/


#ifdef __cplusplus
}
#endif


#endif
