#ifndef TIMER_HELPER_H
#define TIMER_HELPER_H

#ifdef __cplusplus
 extern "C" {
#endif

extern void (*tim1_ptr)(void);
extern void (*tim2_ptr)(void);
extern void (*tim3_ptr)(void);


void TIM_SetUpdatePeriod_us (TIM_TypeDef *timer, uint32_t us);

void TIM_SetupCounterTIM1(uint32_t period_us, void (*tim_ptr)(void));
void TIM_SetupCounterTIM2(uint32_t period_us, void (*tim_ptr)(void));
void TIM_SetupCounterTIM3(uint32_t period_us, void (*tim_ptr)(void));

void TIM_SetupPWM_TIM2(int channel, uint32_t period_us, uint32_t ds_us);

#ifdef __cplusplus
}
#endif


#endif
