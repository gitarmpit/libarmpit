
#include "gpio_helper.h"
#include "main.h"
#include "timer_helper.h"
#include <stdio.h>

// #define __HAL_RCC_DMA1_CLK_DISABLE()      (RCC->AHBENR &= ~(RCC_AHBENR_DMA1EN))

 
static GPIO_PIN* led;

static int cnt = 0;
static BOOL UP = FALSE;
static void Timer_Callback(void*) {
  GPIO_TogglePin(led);
}

/*
static void Timer_Callback2(void* ctx) {
  TIM_Channel* ch = (TIM_Channel*)ctx;
  if (UP && cnt <= 70) {
    TIM_UpdateDs(ch, cnt++);
    if (cnt == 70) {
      UP = FALSE;
    }
  }
  else if (!UP && cnt >= 10) {
    TIM_UpdateDs(ch, cnt--);
    if (cnt == 10) {
      UP = TRUE;
    }
  }
}


static void testPWM3() {
  GPIO_PIN pin1 = GPIO_GetPin("B0");    // TIM3 channel 3
  GPIO_Setup_OutAltPP(&pin1);
  TIM_Channel ch = TIM_SetupPWM_TIM3(3, 80, 20);

  TIM_UpdateDs(&ch, 40);

  //TIM_SetupPWM_OnPin("B5", 400, 200);
  while (1)
    ;
}

static void testUpdateDs() {
  GPIO_PIN pin1 = GPIO_GetPin("B0");    // TIM3 channel 3
  GPIO_Setup_OutAltPP(&pin1);
  TIM_Channel ch = TIM_SetupPWM_TIM3(3, 80, 10);
  SysTick_Delay(2000);
  UP = TRUE;
  cnt = 10;
  TIM_SetupCounterTIM1(100000, Timer_Callback2, &ch);

  while (1)
    ;
}


static void testPWM1() {
  // GPIO_PIN pin1 = GPIO_GetPin("A11");    // TIM1 channel 4
  // GPIO_Setup_OutAltPP(&pin1);

  GPIO_PIN pin1 = GPIO_GetPin("A10");
  GPIO_Setup_OutPP(&pin1);
  GPIO_SetPin(&pin1);
  GPIO_ResetPin(&pin1);
    
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM1(3, 80, 20);

  // TIM_SetupPWM_TIM1(4, 80, 60);
  //TIM_SetupPWM_OnPin("A10", 8, 2);
  while (1)
    ;
}

static void testButton() {
  GPIO_PIN sw = GPIO_GetPin("A7");
  // GPIO_Setup_In_Pullup(&sw);
  Button b(&sw, 1);
  while (1) {
    SysTick_Delay(100);
    // printf ("%d\n", GPIO_IsSet(&sw));
    printf("%d\n", b.IsPinSet());
  }
}

static void testButton2() {
  GPIO_PIN sw = GPIO_GetPin("A7");
  // GPIO_Setup_In_Pullup(&sw);
  button_ctx button = Button_Init(&sw, 1);

  while (1) {
    SysTick_Delay(100);
    // printf ("%d\n", GPIO_IsSet(&sw));
    printf("%d\n", Button_IsPinSet(&button));
  }
}

void on_click(void* ctx) {
  button_ctx* b = (button_ctx*)ctx;
  printf ("on click, b: %d\n", b->buttonId);
}

void on_dclick(void* ctx) {
  button_ctx* b = (button_ctx*)ctx;
  printf ("on dclick, b: %d\n", b->buttonId);
}

static void testButtonHandler() {
  BH_Init(TIM2);
  GPIO_PIN bpin = GPIO_GetPin("A7");
  button_ctx b1 = Button_Init(&bpin, 1);
  BH_AddButton(&b1);
  BH_SetOnClickHandler(on_click);
  BH_SetOnDoubleClickHandler(on_dclick);
  while(1)
    ;
}


static void testBlinky() {
  GPIO_PIN pin1 = GPIO_GetPin("B8");
  GPIO_Setup_OutPP(&pin1);
  while (1) {
    printf("%d\n", SysTick_GetTick());
    GPIO_TogglePin(&pin1);
    SysTick_Delay(1000);
  }
}
*/

static void testTimer() {
  GPIO_PIN pin1 = GPIO_GetPin("A0");  // LED: B8
  GPIO_Setup_OutPP(&pin1);
  led = &pin1;

  GPIO_SetPin(&pin1);
  TIM_SetupCounterTIM4(10, Timer_Callback, NULL);
  while (1)
    ;
}

static void testPWM2() {
  GPIO_PIN pin1 = GPIO_GetPin("A0");    // TIM2 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM2(1, 180, 40);
  //TIM_SetupPWM_OnPin("B11", 800, 600);
  while (1)
    ;
}


 
int main(void) {
  System_Config();
  SystemClock_Config_HSE();

  //testBlinky();
  //testPWM2();
  testTimer();

  //testUpdateDs();
  //testButton2();

  //testButtonHandler();

  while (1)
    ;
}

