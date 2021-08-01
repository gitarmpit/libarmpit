#include "main.h"
 __IO uint32_t uwStep = 0;

TIM_HandleTypeDef TimHandle;
TIM_OC_InitTypeDef sPWMConfig1, sPWMConfig2, sPWMConfig3;
TIM_BreakDeadTimeConfigTypeDef sBreakConfig;
__IO uint32_t uwAuthorizeTimComEvent = 0;
static void SystemClock_Config(void);
static void Error_Handler(void);
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  BSP_LED_Init(LED3);
  TimHandle.Instance = TIM1;
  TimHandle.Init.Period = 4095;
  TimHandle.Init.Prescaler = 0;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if(HAL_TIM_OC_Init(&TimHandle) != HAL_OK)
  {
    Error_Handler();
  }

  sPWMConfig1.OCMode = TIM_OCMODE_TIMING;
  sPWMConfig1.OCPolarity = TIM_OCPOLARITY_HIGH;
  sPWMConfig1.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sPWMConfig1.OCIdleState = TIM_OCIDLESTATE_SET;
  sPWMConfig1.OCNIdleState = TIM_OCNIDLESTATE_SET;
  sPWMConfig1.OCFastMode = TIM_OCFAST_DISABLE;

  sPWMConfig1.Pulse = 2047;
  if(HAL_TIM_OC_ConfigChannel(&TimHandle, &sPWMConfig1, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  sPWMConfig2 = sPWMConfig1;
  sPWMConfig2.Pulse = 1023;
  if(HAL_TIM_OC_ConfigChannel(&TimHandle, &sPWMConfig2, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

  sPWMConfig3 = sPWMConfig1;
  sPWMConfig3.Pulse = 511;
  if(HAL_TIM_OC_ConfigChannel(&TimHandle, &sPWMConfig3, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  sBreakConfig.OffStateRunMode = TIM_OSSR_ENABLE;
  sBreakConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakConfig.BreakState = TIM_BREAK_ENABLE;
  sBreakConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
  sBreakConfig.DeadTime = 1;

  if(HAL_TIMEx_ConfigBreakDeadTime(&TimHandle, &sBreakConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIMEx_ConfigCommutationEvent_IT(&TimHandle, TIM_TS_NONE, TIM_COMMUTATION_SOFTWARE);

  if(HAL_TIM_OC_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_TIMEx_OCN_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_TIM_OC_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_TIMEx_OCN_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_TIM_OC_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_TIMEx_OCN_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  uwAuthorizeTimComEvent = 1;

  while (1)
  {
  }
}



void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim)
{
  if (uwStep == 0)
  {
    sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
    HAL_TIMEx_OCN_Stop(&TimHandle, TIM_CHANNEL_1);

    sPWMConfig3.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig3, TIM_CHANNEL_3);
    HAL_TIMEx_OCN_Start(&TimHandle, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_3);

    HAL_TIM_OC_Stop(&TimHandle, TIM_CHANNEL_2);
    HAL_TIMEx_OCN_Stop(&TimHandle, TIM_CHANNEL_2);
    uwStep = 1;
  }

  if (uwStep == 1)
  {
    sPWMConfig2.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig2, TIM_CHANNEL_2);
    HAL_TIMEx_OCN_Start(&TimHandle, TIM_CHANNEL_2);
    HAL_TIMEx_OCN_Stop(&TimHandle, TIM_CHANNEL_3);
    uwStep++;
  }
  else if (uwStep == 2)
  {
    sPWMConfig3.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3);
    HAL_TIM_OC_Stop(&TimHandle, TIM_CHANNEL_1);
    uwStep++;
  }

  else if (uwStep == 3)
  {
    HAL_TIMEx_OCN_Stop(&TimHandle, TIM_CHANNEL_2);
    sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig1, TIM_CHANNEL_1);
    HAL_TIMEx_OCN_Start(&TimHandle, TIM_CHANNEL_1);
    uwStep++;
  }
  else if (uwStep == 4)
  {
    HAL_TIM_OC_Stop(&TimHandle, TIM_CHANNEL_3);
    sPWMConfig2.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2);
    uwStep++;
  }
  else if (uwStep == 5)
  {
    sPWMConfig3.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig3, TIM_CHANNEL_3);
    HAL_TIMEx_OCN_Start(&TimHandle, TIM_CHANNEL_3);
    HAL_TIMEx_OCN_Stop(&TimHandle, TIM_CHANNEL_1);
    uwStep++;
  }
  else
  {
    sPWMConfig1.OCMode = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
    HAL_TIM_OC_Stop(&TimHandle, TIM_CHANNEL_2);

    uwStep = 1;
  }
}
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

static void Error_Handler(void)
{
  BSP_LED_On(LED3);
  while (1)
  {
  }
}
