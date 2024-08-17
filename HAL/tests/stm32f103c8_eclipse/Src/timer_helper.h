#ifndef TIMER_HELPER_H
#define TIMER_HELPER_H

void TIM_SetUpdatePeriod_us (TIM_TypeDef *timer, uint32_t us);

void TIM_SetupCounterTIM1(uint32_t period_us);
void TIM_SetupCounterTIM2(uint32_t period_us);
void TIM_SetupCounterTIM3(uint32_t period_us);

#endif
