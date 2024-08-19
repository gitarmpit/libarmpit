#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_tim.h"

extern "C" {
typedef void (*tim_handler)(void*);

extern tim_handler __tim1_handler;
extern tim_handler __tim2_handler;
extern tim_handler __tim3_handler;
extern tim_handler __tim4_handler;
}

class TIM;

typedef struct {
  TIM* timer;
  uint8_t channel;
} TIM_Channel;

class TIMER_Interrupt_Handler {
public:
  virtual void HandleInterrupt() = 0;
};

class TIM {
private:
  uint32_t GetTIMx_CLK();
  void CalculateTimerValues(uint32_t timClk, uint32_t us, uint16_t* presc, uint16_t* arr);
  void CalculateTimerValues_ns(uint32_t timClk, uint32_t ns, uint16_t* presc, uint16_t* arr);

public:
  void SetUpdatePeriod_us(uint32_t us);
  void SetupCounter(uint32_t period_us);
  TIM_Channel SetupPWM(uint8_t channel, uint32_t period_us, uint32_t ds_us);
  void EnableCounter();
  void DisableCounter();
  void UpdatePeriodDs(uint8_t channel, uint32_t period_us, uint32_t ds_us);
  void UpdateDs(uint8_t channel, uint32_t ds_us);
  void UpdatePeriodDs_ns(uint8_t channel, uint32_t period_ns, uint32_t ds_ns);
  void UpdateDs_ns(uint8_t channel, uint32_t ds_us);
  void SetInterruptHandler(TIMER_Interrupt_Handler* handler) { _irqHandler = handler; }
  void HandleInterrupt() {
    if (_irqHandler) {
      _irqHandler->HandleInterrupt();
    }
  }
  void SetIrqPriority(uint32_t IRQ_priority) { _IRQ_priority = IRQ_priority; }

protected:
  TIM(TIM_TypeDef* tim, bool isAPB1, IRQn_Type IRQn, uint32_t periph)
      : _tim(tim),
        _isAPB1(isAPB1),
        _IRQn(IRQn),
        _IRQ_priority(0),
        _periph(periph) {}

private:
  TIM_TypeDef* _tim;
  TIMER_Interrupt_Handler* _irqHandler;
  IRQn_Type _IRQn;
  bool _isAPB1;
  uint32_t _IRQ_priority;
  uint32_t _periph;
};

class TIMER1 : public TIM {
protected:
  TIMER1() : TIM(TIM1, false, TIM1_UP_TIM10_IRQn, LL_APB2_GRP1_PERIPH_TIM1) {}

public:
  static TIMER1* GetInstance() {
    static TIMER1 instance;
    return &instance;
  }
};

class TIMER2 : public TIM {
public:
  TIMER2() : TIM(TIM2, true, TIM2_IRQn, LL_APB1_GRP1_PERIPH_TIM2) {}

public:
  static TIMER2* GetInstance() {
    static TIMER2 instance;
    return &instance;
  }
};

class TIMER3 : public TIM {
public:
  TIMER3() : TIM(TIM3, true, TIM3_IRQn, LL_APB1_GRP1_PERIPH_TIM3) {}

public:
  static TIMER3* GetInstance() {
    static TIMER3 instance;
    return &instance;
  }
};

class TIMER4 : public TIM {
public:
  TIMER4() : TIM(TIM4, true, TIM4_IRQn, LL_APB1_GRP1_PERIPH_TIM4) {}

public:
  static TIMER4* GetInstance() {
    static TIMER4 instance;
    return &instance;
  }
};

class TIMER5 : public TIM {
public:
  TIMER5() : TIM(TIM5, true, TIM5_IRQn, LL_APB1_GRP1_PERIPH_TIM5) {}

public:
  static TIMER5* GetInstance() {
    static TIMER5 instance;
    return &instance;
  }
};

class TIMER6 : public TIM {
public:
  TIMER6() : TIM(TIM6, true, TIM6_DAC_IRQn, LL_APB1_GRP1_PERIPH_TIM6) {}

public:
  static TIMER6* GetInstance() {
    static TIMER6 instance;
    return &instance;
  }
};


class TIMER7 : public TIM {
public:
  TIMER7() : TIM(TIM7, true, TIM7_IRQn, LL_APB1_GRP1_PERIPH_TIM7) {}

public:
  static TIMER7* GetInstance() {
    static TIMER7 instance;
    return &instance;
  }
};

class TIMER8 : public TIM {
public:
  TIMER8() : TIM(TIM8, false, TIM8_UP_TIM13_IRQn, LL_APB2_GRP1_PERIPH_TIM8) {}

public:
  static TIMER8* GetInstance() {
    static TIMER8 instance;
    return &instance;
  }
};


class TIMER9 : public TIM {
public:
  TIMER9() : TIM(TIM9, false, TIM1_BRK_TIM9_IRQn, LL_APB2_GRP1_PERIPH_TIM9) {}

public:
  static TIMER9* GetInstance() {
    static TIMER9 instance;
    return &instance;
  }
};

class TIMER10 : public TIM {
public:
  TIMER10() : TIM(TIM10, false, TIM1_UP_TIM10_IRQn, LL_APB2_GRP1_PERIPH_TIM10) {}

public:
  static TIMER10* GetInstance() {
    static TIMER10 instance;
    return &instance;
  }
};

class TIMER11 : public TIM {
public:
  TIMER11() : TIM(TIM11, false, TIM1_TRG_COM_TIM11_IRQn, LL_APB2_GRP1_PERIPH_TIM11) {}

public:
  static TIMER11* GetInstance() {
    static TIMER11 instance;
    return &instance;
  }
};

class TIMER12 : public TIM {
public:
  TIMER12() : TIM(TIM12, true, TIM8_BRK_TIM12_IRQn, LL_APB1_GRP1_PERIPH_TIM12) {}

public:
  static TIMER12* GetInstance() {
    static TIMER12 instance;
    return &instance;
  }
};

class TIMER13 : public TIM {
public:
  TIMER13() : TIM(TIM13, true, TIM8_UP_TIM13_IRQn, LL_APB1_GRP1_PERIPH_TIM13) {}

public:
  static TIMER13* GetInstance() {
    static TIMER13 instance;
    return &instance;
  }
};

class TIMER14 : public TIM {
public:
  TIMER14() : TIM(TIM14, true, TIM8_TRG_COM_TIM14_IRQn, LL_APB1_GRP1_PERIPH_TIM14) {}

public:
  static TIMER14* GetInstance() {
    static TIMER14 instance;
    return &instance;
  }
};



#endif