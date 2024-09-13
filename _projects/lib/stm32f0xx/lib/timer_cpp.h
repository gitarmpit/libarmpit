#ifndef _TIMER_CPP_H
#define _TIMER_CPP_H

#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_tim.h"

extern "C" {
typedef void (*tim_handler)(void*);

extern tim_handler __tim1_handler;
extern tim_handler __tim2_handler;
extern tim_handler __tim3_handler;
extern tim_handler __tim4_handler;
}

class TIMER;

typedef struct {
  TIMER* timer;
  uint8_t channel;
} TIM_Channel;

class TIMER_Interrupt_Handler {
public:
  virtual void HandleInterrupt() = 0;
};

class TIMER {
private:
  uint32_t GetTIMx_CLK();
  void CalculateTimerValues(uint32_t timClk, uint32_t us, uint16_t* presc, uint16_t* arr);

public:
  void SetUpdatePeriod_us(uint32_t us);
  void SetupCounter(uint32_t period_us);
  TIM_Channel SetupPWM(uint8_t channel, uint32_t period_us, uint32_t ds_us);
  void EnableCounter();
  void DisableCounter();
  void UpdatePeriodDs(uint8_t channel, uint32_t period_us, uint32_t ds_us);
  void UpdateDs(uint8_t channel, uint32_t ds_us);
  void SetInterruptHandler(TIMER_Interrupt_Handler* handler) { _irqHandler = handler; }
  void HandleInterrupt() {
    if (_irqHandler) {
      _irqHandler->HandleInterrupt();
    }
  }
  void SetIrqPriority(uint32_t IRQ_priority) { _IRQ_priority = IRQ_priority; }

protected:
  TIMER(TIM_TypeDef* tim, bool isAPB1, IRQn_Type IRQn, uint32_t periph)
      : _tim(tim),
        _isAPBGroup1(isAPB1),
        _IRQn(IRQn),
        _IRQ_priority(0),
        _periph(periph) {}

private:
  TIM_TypeDef* _tim;
  TIMER_Interrupt_Handler* _irqHandler;
  IRQn_Type _IRQn;
  bool _isAPBGroup1;
  uint32_t _IRQ_priority;
  uint32_t _periph;
};

class TIMER1 : public TIMER {
protected:
  TIMER1() : TIMER(TIM1, false, TIM1_BRK_UP_TRG_COM_IRQn, LL_APB1_GRP2_PERIPH_TIM1) {}

public:
  static TIMER1* GetInstance() {
    static TIMER1 instance;
    return &instance;
  }
};


class TIMER3 : public TIMER {
public:
  TIMER3() : TIMER(TIM3, true, TIM3_IRQn, LL_APB1_GRP1_PERIPH_TIM3) {}

public:
  static TIMER3* GetInstance() {
    static TIMER3 instance;
    return &instance;
  }
};


#endif
