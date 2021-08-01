#include "main.h"
#if (USE_TIMEOUT == 1)
#define RNG_GENERATION_TIMEOUT 20
#endif

#define NB_OF_GENERATED_RANDOM_NUMBERS 8


#if (USE_TIMEOUT == 1)
uint32_t Timeout = 0;
#endif
__IO uint8_t ubButtonPress = 0;
__IO uint32_t aRandom32bit[NB_OF_GENERATED_RANDOM_NUMBERS];


void SystemClock_Config(void);
void Configure_RNG(void);
void RandomNumbersGeneration(void);
void LED_Init(void);
void LED_On(void);
void LED_Blinking(uint32_t Period);
void UserButton_Init(void);
void WaitForUserButtonPress(void);
uint32_t SetSystemToHSI(void);

int main(void)
{

  SystemClock_Config();
  LED_Init();
  UserButton_Init();
  Configure_RNG();
  WaitForUserButtonPress();
  RandomNumbersGeneration();
  while (1)
  {
  }
}
void Configure_RNG(void)
{

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_RNG);

  while (SetSystemToHSI()!= 0)
  {
  };


  LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 336, LL_RCC_PLLQ_DIV_7);
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };


  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  SysTick_Config(168000000 / 1000);
  SystemCoreClock = 168000000;
}
void RandomNumbersGeneration(void)
{
  register uint8_t index = 0;
  LL_RNG_Enable(RNG);
  for(index = 0; index < NB_OF_GENERATED_RANDOM_NUMBERS; index++)
  {
#if (USE_TIMEOUT == 1)
    Timeout = RNG_GENERATION_TIMEOUT;
#endif
    while (!LL_RNG_IsActiveFlag_DRDY(RNG))
    {
#if (USE_TIMEOUT == 1)

      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if(Timeout-- == 0)
        {
          LED_Blinking(LED_BLINK_SLOW);
        }
      }
#endif
    }

    if ( (LL_RNG_IsActiveFlag_CECS(RNG) )
        || (LL_RNG_IsActiveFlag_SECS(RNG)) )
    {

      LED_Blinking(LED_BLINK_ERROR);
    }

    aRandom32bit[index] = LL_RNG_ReadRandData32(RNG);
  }


  LL_RNG_Disable(RNG);
  LED_On();
}



void LED_Init(void)
{
  LED1_GPIO_CLK_ENABLE();
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
}
void LED_On(void)
{
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}
void LED_Blinking(uint32_t Period)
{
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(Period);
  }
}
void UserButton_Init(void)
{
  USER_BUTTON_GPIO_CLK_ENABLE();
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);
  USER_BUTTON_SYSCFG_SET_EXTI();
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn, 0x03);
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
}

void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(LED_BLINK_FAST);
  }
  LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

void SystemClock_Config(void)
{
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableOverDriveMode();
  while(LL_PWR_IsActiveFlag_OD() != 1)
  {
  };


  LL_PWR_EnableOverDriveSwitching();
  while(LL_PWR_IsActiveFlag_ODSW() != 1)
  {
  };

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 360, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };


  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };


  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);


  SysTick_Config(180000000 / 1000);


  SystemCoreClock = 180000000;
}

uint32_t SetSystemToHSI(void)
{
  uint32_t timeout = 0;

  if (LL_RCC_HSI_IsReady() != 1)
  {
    LL_RCC_HSI_Enable();
    timeout = 1000;
    while ((LL_RCC_HSI_IsReady() != 1) && (timeout != 0))
    {
      if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
      {
        timeout --;
      }
      if (timeout == 0)
      {
        return 1;
      }
    }
  }


  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  SystemCoreClock = HSI_VALUE;


  if (LL_RCC_PLL_IsReady() != 0)
  {
    LL_RCC_PLL_Disable();
    timeout = 1000;
    while ((LL_RCC_PLL_IsReady() != 0) && (timeout != 0))
    {
      if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
      {
        timeout --;
      }
      if (timeout == 0)
      {
        return 1;
      }
    }
  }

  return 0;
}

void UserButton_Callback(void)
{
  ubButtonPress = 1;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  while (1)
  {
  }
}
#endif
