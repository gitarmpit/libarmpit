#include "config.h"
#include "gpio.h"
#include "systick.h"
#include "timer.h"
#include <stdio.h>

GPIO_PIN* led;

class TestInterruptHandler : public TIMER_Interrupt_Handler {
public:
  virtual void HandleInterrupt() { GPIO_TogglePin(led); }
};

class DsInterruptHandler : public TIMER_Interrupt_Handler {

public:

  DsInterruptHandler (TIM_Channel* ch, int cnt, bool UP) : _ch (ch), _cnt(cnt), _UP (UP) {}

  virtual void HandleInterrupt() {

    if (_UP && _cnt <= 70) {
      _ch->timer->UpdateDs(_ch->channel, _cnt++);
      if (_cnt == 70) {
        _UP = FALSE;
      }
    } else if (!_UP && _cnt >= 10) {
      _ch->timer->UpdateDs(_ch->channel, _cnt--);
      if (_cnt == 10) {
        _UP = TRUE;
      }
    }
  }

private:
  TIM_Channel* _ch;
  int _cnt;
  bool _UP;
};

static GPIO_PIN SetupPWMPin(const char* pinName, uint8_t AF) {
  GPIO_PIN pin = GPIO_GetPin(pinName);
  GPIO_Setup_OutAltPP(&pin);
  GPIO_SetAF(&pin, AF);
  return pin;
}

static void testPWM1() {
  GPIO_PIN a8 = GPIO_GetPin("A8");
  GPIO_Setup_OutAltPP(&a8);
  GPIO_SetAF(&a8, 1);

  TIM* pwm = TIMER1::GetInstance();
  TIM_Channel ch = pwm->SetupPWM(1, 80, 40);


  while (1)
    ;
}

static void testDs() {
  GPIO_PIN a8 = GPIO_GetPin("A8");
  GPIO_Setup_OutAltPP(&a8);
  GPIO_SetAF(&a8, 1);

  TIM* pwm = TIMER1::GetInstance();
  TIM_Channel ch = pwm->SetupPWM(1, 80, 10);

  SysTick_Delay(2000);

  TIM* t = TIMER3::GetInstance();
  t->SetupCounter(100000);
  DsInterruptHandler handler (&ch, 10, true);
  t->SetInterruptHandler(&handler);

  while (1)
    ;
}

static void testPWM2() {

  uint8_t AF = 1;
  uint8_t channel = 2;
  SetupPWMPin("A1", AF);

  TIM* t = TIMER2::GetInstance();
  t->SetupPWM(channel, 80, 40);

  while (1)
    ;
}

static void testPWM3() {

  uint8_t AF = 2;
  uint8_t channel = 1;
  SetupPWMPin("A6", AF);

  TIM* t = TIMER3::GetInstance();
  t->SetupPWM(channel, 80, 40);

  while (1)
    ;
}

static void testPWM4() {

  uint8_t AF = 2;
  uint8_t channel = 2;
  SetupPWMPin("B7", AF);

  TIM* t = TIMER4::GetInstance();
  t->SetupPWM(channel, 80, 40);

  while (1)
    ;
}

static void testPWM5() {

  uint8_t AF = 2;
  uint8_t channel = 3;
  SetupPWMPin("A2", AF);

  TIM* t = TIMER5::GetInstance();
  t->SetupPWM(channel, 80, 40);

  while (1)
    ;
}

static void testPWM8() {

  uint8_t AF = 3;
  uint8_t channel = 3;
  SetupPWMPin("C8", AF);

  TIM* t = TIMER8::GetInstance();
  t->SetupPWM(channel, 80, 40);

  while (1)
    ;
}

static void testPWM9() {

  uint8_t AF = 3;
  uint8_t channel = 1;
  SetupPWMPin("A2", AF);

  TIM* t = TIMER9::GetInstance();
  t->SetupPWM(channel, 80, 40);

  while (1)
    ;
}

static void testPWM10() {

  uint8_t AF = 3;
  uint8_t channel = 1;
  SetupPWMPin("B8", AF);

  TIM* t = TIMER10::GetInstance();
  t->SetupPWM(channel, 80, 40);

  while (1)
    ;
}


static void testTimer() {
  GPIO_PIN a8 = GPIO_GetPin("A8");
  GPIO_Setup_OutPP(&a8);
  led = &a8;

  TestInterruptHandler handler;

  TIM* t = TIMER2::GetInstance();
  t->SetupCounter(5000000);
  t->SetInterruptHandler(&handler);
  while (1)
    ;
}

int main(void) {
  System_Config();
  BOOL enableBypass = FALSE;
  SystemClock_Config_HSE(enableBypass);
  // SystemClock_Config_HSI();

  //testPWM10();

  // testTimer();

  //testDs();

  while (1) {
  }
}
