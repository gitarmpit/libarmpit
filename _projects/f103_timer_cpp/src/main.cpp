#include <stdio.h>
#include "config.h"
#include "gpio.h"
#include "timer_cpp.h"
#include "button.h"
#include "button_handler.h"
#include "systick.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_rcc.h"


static GPIO_PIN* led;

static void Timer_Callback(void*) {
  GPIO_TogglePin(led);
}


class TestInterruptHandler : public TIMER_Interrupt_Handler {
public:
  virtual void HandleInterrupt()  {
    GPIO_TogglePin(led);
  }
};


class MyButtonHandler : public ButtonHandler 
{
private:
    GPIO_PIN* _ledPin;

public:
    MyButtonHandler (TIMER* timer, GPIO_PIN* ledPin) : ButtonHandler (timer),
    _ledPin (ledPin) 
    {
    }

    virtual void OnStateChange(Button* button)
    {
       GPIO_TogglePin(_ledPin);
    }
    
    virtual void OnClick(Button* button)
    {
      // GPIO_TogglePin(_ledPin);
    }
};


static void testButtonHandler() {

  GPIO_PIN led = GPIO_GetPin("B8");
  GPIO_Setup_OutPP(&led);

  GPIO_PIN bpin = GPIO_GetPin("A7");
  Button b (&bpin, 1);
  MyButtonHandler bh (TIMER2::GetInstance(), &led);
  bh.AddButton(&b);
  while(1)
    ;
}

static void testPWM() {
  GPIO_PIN pin1 = GPIO_GetPin("A0");    // TIM2 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIMER* t = TIMER2::GetInstance();
  t->SetupPWM(1, 40, 20);
  t->DisableCounter();
  t->EnableCounter();

  SysTick_Delay(5000);
  t->UpdateDs(1, 30);
  SysTick_Delay(5000);
  t->UpdatePeriodDs(1, 20, 10);

  while(1)
    ;
}

static void testTimer() {

  GPIO_PIN pin1 = GPIO_GetPin("A0");  // LED: B8
  GPIO_Setup_OutPP(&pin1);
  led = &pin1;

  TestInterruptHandler handler;
  TIMER* t = TIMER4::GetInstance();
  t->SetInterruptHandler(&handler);
  //__tim2_handler = Timer_Callback;
  t->SetupCounter(40);
  while(1)
    ;
}


static void Sleep() {
  CLEAR_BIT(SCB->SCR, (uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
  CLEAR_BIT(PWR->CR, (PWR_CR_PDDS | PWR_CR_LPDS_Msk));
  __WFI();
}

// Lpreg, Main reg off, 0.8-0.9mA
static void Stop() {
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
  CLEAR_BIT(PWR->CR, PWR_CR_PDDS);
  SET_BIT(PWR->CR, PWR_CR_LPDS_Msk);

  __WFI();
}

// Main reg on, 0.9-1mA
static void Stop2() {
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
  CLEAR_BIT(PWR->CR, PWR_CR_PDDS);
  CLEAR_BIT(PWR->CR, PWR_CR_LPDS_Msk);
  __WFI();
}


// Deep sleep: standby 20uA
static void StandBy() {
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
  SET_BIT(PWR->CR, PWR_CR_PDDS);
  CLEAR_BIT(PWR->CSR, PWR_CSR_WUF_Msk);

  // Enable wakeup pin
  SET_BIT(PWR->CSR, PWR_CSR_EWUP_Msk);

  __WFI();
}


int main(void) {

  System_Config();
  SystemClock_Config_HSE(); // 24mA

  LL_GPIO_AF_DisableRemap_SWJ();


  if (LL_PWR_IsActiveFlag_SB()) {
      LL_PWR_ClearFlag_SB();
      GPIO_PIN pin1 = GPIO_GetPin("B8");  // LED: B8
      GPIO_Setup_OutPP(&pin1);
      GPIO_SetPin(&pin1);
      SysTick_Delay(4000);
  }

  // Disable wakeup pin
  CLEAR_BIT(PWR->CSR, PWR_CSR_EWUP_Msk);
  if (LL_PWR_IsActiveFlag_WU()) {
    LL_PWR_ClearFlag_WU();
  }



  //SysTick_Delay(5000);

  //LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_16);


  LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_AFIO);
  SysTick->CTRL = 0;
  StandBy();

  //System_Config();
  
  //Stop2();

  // Stop();

  //SystemClock_Config_HSE(); // 24mA

  while (1) {
    //printf("%d\n", SysTick_GetTick());
    //GPIO_TogglePin(&pin1);
    //SysTick_Delay(5000);
  }

  // testButtonHandler();

  //testPWM();
  //testTimer();

  while (1)
    ;
}
