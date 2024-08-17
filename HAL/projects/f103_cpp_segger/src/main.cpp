#include <stdio.h>
#include "main.h"
#include "gpio_helper.h"
#include "timer_cpp.h"
#include "button.h"
#include "button_handler.h"

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


int main(void) {
  
  System_Config();
  SystemClock_Config_HSE();

  /*
  TestInterruptHandler handler;
  TIMER* t = TIMER2::GetInstance();
  t->SetInterruptHandler(&handler);
  __tim2_handler = Timer_Callback;
  t->SetupCounter(1);
  */

  /*
  while (1) {
    printf("%d\n", SysTick_GetTick());
    GPIO_TogglePin(&pin1);
    SysTick_Delay(1000);
  }
  */

  testButtonHandler();

  while (1)
    ;
}
