#include "main.h"

#define TIM_DUTY_CYCLES_NB 11

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Duty cycles: D = T/P * 100%                                                */
/* where T is the pulse duration and P  the period of the PWM signal          */
static uint32_t aDutyCycle[TIM_DUTY_CYCLES_NB] = {
  0,    /*  0% */
  10,   /* 10% */
  20,   /* 20% */
  30,   /* 30% */
  40,   /* 40% */
  50,   /* 50% */
  60,   /* 60% */
  70,   /* 70% */
  80,   /* 80% */
  90,   /* 90% */
  100,  /* 100% */
};

/* Duty cycle index */
static uint8_t iDutyCycle = 0;

/* Measured duty cycle */
__IO uint32_t uwMeasuredDutyCycle = 0;

/* TIM2 Clock */
static uint32_t TimOutClock = 1;

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE void     SystemClock_Config(void);
__STATIC_INLINE void     Configure_TIMPWMOutput(void);
__STATIC_INLINE void     Configure_DutyCycle(uint32_t OCMode);
__STATIC_INLINE void     UserButton_Init(void);

int main(void)
{
  SystemClock_Config();

  UserButton_Init();
  
  Configure_TIMPWMOutput();

  while (1)
  {
  }
}


__STATIC_INLINE void  Configure_TIMPWMOutput(void)
{
  LL_TIM_InitTypeDef    tim_initstruct;
  LL_TIM_OC_InitTypeDef tim_oc_initstruct;
  
  /* Enable the peripheral clock of GPIOs */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  
  /* GPIO TIM2_CH1 configuration */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_0, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);
  
  NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn);
  
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2); 
  

  /* Set fields of initialization structure */
  /* - Set the pre-scaler value to have TIM2 counter clock equal to 10 kHz  */
  /* - Set the auto-reload value to have a counter frequency of 100 Hz        */
  /* TIM2CLK = SystemCoreClock / (APB prescaler & multiplier)               */
  TimOutClock = SystemCoreClock/1;
  
  tim_initstruct.Prescaler         = __LL_TIM_CALC_PSC(SystemCoreClock, 10000);
  tim_initstruct.CounterMode       = LL_TIM_COUNTERMODE_UP;
  tim_initstruct.Autoreload        = __LL_TIM_CALC_ARR(TimOutClock, tim_initstruct.Prescaler, 100);
  tim_initstruct.ClockDivision     = LL_TIM_CLOCKDIVISION_DIV1;
  tim_initstruct.RepetitionCounter = (uint8_t)0x00;
  
  /* Initialize TIM instance according to parameters defined in               */
  /* initialization structure.                                                */
  LL_TIM_Init(TIM2, &tim_initstruct);
  
  /* Enable TIM2_ARR register preload. Writing to or reading from the         */
  /* auto-reload register accesses the preload register. The content of the   */
  /* preload register are transferred into the shadow register at each update */
  /* event (UEV).                                                             */  
  LL_TIM_EnableARRPreload(TIM2);
  
  /*********************************/
  /* Output waveform configuration */
  /*********************************/
  /* Set fields of initialization structure */
  /*  - Set compare value to half of the counter period (50% duty cycle ) */
  tim_oc_initstruct.OCMode       = LL_TIM_OCMODE_PWM1;
  tim_oc_initstruct.OCState      = LL_TIM_OCSTATE_DISABLE;
  tim_oc_initstruct.OCNState     = LL_TIM_OCSTATE_DISABLE;
  tim_oc_initstruct.CompareValue = ( (LL_TIM_GetAutoReload(TIM2) + 1 ) / 2);
  tim_oc_initstruct.OCPolarity   = LL_TIM_OCPOLARITY_HIGH;
  tim_oc_initstruct.OCNPolarity  = LL_TIM_OCPOLARITY_HIGH;
  tim_oc_initstruct.OCIdleState  = LL_TIM_OCIDLESTATE_LOW;
  tim_oc_initstruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
  
  /* Initialize TIM instance according to parameters defined in               */
  /* initialization structure.                                                */
  LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH1, &tim_oc_initstruct);
  
  /* Enable TIM2_CCR1 register preload. Read/Write operations access the      */
  /* preload register. TIM2_CCR1 preload value is loaded in the active        */
  /* at each update event.                                                    */
  LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH1);
  
  /**************************/
  /* TIM2 interrupts set-up */
  /**************************/
  /* Enable the capture/compare interrupt for channel 1*/
  LL_TIM_EnableIT_CC1(TIM2);
  
  /**********************************/
  /* Start output signal generation */
  /**********************************/
  /* Enable output channel 1 */
  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
  
  /* Enable counter */
  LL_TIM_EnableCounter(TIM2);
  
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM2);
}

/**
  * @brief  Changes the duty cycle of the PWM signal.
  *         D = (T/P)*100
  *           where T is the pulse duration and P is the PWM signal period
  * @param  D Duty cycle
  * @retval None
  */
__STATIC_INLINE void Configure_DutyCycle(uint32_t D)
{
  uint32_t P;    /* Pulse duration */
  uint32_t T;    /* PWM signal period */
  
  /* PWM signal period is determined by the value of the auto-reload register */
  T = LL_TIM_GetAutoReload(TIM2) + 1;
  
  /* Pulse duration is determined by the value of the compare register.       */
  /* Its value is calculated in order to match the requested duty cycle.      */
  P = (D*T)/100;
  LL_TIM_OC_SetCompareCH1(TIM2, P);
}

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None  
  * @retval None
  */
__STATIC_INLINE void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_DOWN);

  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();
    
  /* Enable a rising trigger EXTI line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
    
  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
}

void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}

void UserButton_Callback(void)
{
  /* Set new duty cycle */
  iDutyCycle = (iDutyCycle + 1) % TIM_DUTY_CYCLES_NB;

  /* Change PWM signal duty cycle */
  Configure_DutyCycle(aDutyCycle[iDutyCycle]);
}

void TimerCaptureCompare_Callback(void)
{
  uwMeasuredDutyCycle = (LL_TIM_OC_GetCompareCH1(TIM2) * 100) / ( LL_TIM_GetAutoReload(TIM2) + 1 );
}

