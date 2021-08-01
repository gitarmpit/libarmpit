#include "main.h"

  
  #define ADC_CALIBRATION_TIMEOUT_MS ((uint32_t) 1)
  #define ADC_ENABLE_TIMEOUT_MS ((uint32_t) 1)
  #define ADC_DISABLE_TIMEOUT_MS ((uint32_t) 1)
  #define ADC_STOP_CONVERSION_TIMEOUT_MS ((uint32_t) 1)
  #define ADC_CONVERSION_TIMEOUT_MS ((uint32_t) 2)

  #define VDDA_APPLI ((uint32_t)3300)
  #define ADC_AWD_THRESHOLD_HIGH (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B)/2)
  #define ADC_AWD_THRESHOLD_LOW ((uint32_t) 0)

__IO uint8_t ubAnalogWatchdog1Status = 0;

void SystemClock_Config(void);
void Configure_ADC(void);
void Activate_ADC(void);
void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Blinking(uint32_t Period);
void UserButton_Init(void);
int main(void)
{
  SystemClock_Config();
  LED_Init();
  UserButton_Init();
  Configure_ADC();
  Activate_ADC();
  if (LL_ADC_IsEnabled(ADC1) == 1)
  {
    LL_ADC_REG_StartConversionSWStart(ADC1);
  }
  else
  {
    LED_Blinking(LED_BLINK_ERROR);
  }


  while (1)
  {
  }
}
void Configure_ADC(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
  NVIC_SetPriority(ADC_IRQn, 0);
  NVIC_EnableIRQ(ADC_IRQn);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
  if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
  {
    LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_SYNC_PCLK_DIV2);
  }
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
  }
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);
    LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_4);
  }
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
  }
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_56CYCLES);
  }
  LL_ADC_SetAnalogWDMonitChannels(ADC1, LL_ADC_AWD_ALL_CHANNELS_REG);
  LL_ADC_SetAnalogWDThresholds(ADC1, LL_ADC_AWD_THRESHOLD_HIGH, ADC_AWD_THRESHOLD_HIGH);
  LL_ADC_SetAnalogWDThresholds(ADC1, LL_ADC_AWD_THRESHOLD_LOW, ADC_AWD_THRESHOLD_LOW);
  LL_ADC_EnableIT_AWD1(ADC1);
}

void Activate_ADC(void)
{
  #if (USE_TIMEOUT == 1)
  uint32_t Timeout = 0;
  #endif
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    LL_ADC_Enable(ADC1);
  }
}
void LED_Init(void)
{
  LED2_GPIO_CLK_ENABLE();
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
}

void LED_On(void)
{
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

void LED_Off(void)
{
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

void LED_Blinking(uint32_t Period)
{
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    LL_mDelay(Period);
  }
}

void UserButton_Init(void)
{
  USER_BUTTON_GPIO_CLK_ENABLE();
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);
  {
    USER_BUTTON_SYSCFG_SET_EXTI();
    USER_BUTTON_EXTI_LINE_ENABLE();
    USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
    NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
    NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);
  }
}
void SystemClock_Config(void)
{
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 400, LL_RCC_PLLP_DIV_4);
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

  SysTick_Config(100000000 / 1000);
  SystemCoreClock = 100000000;
}

void UserButton_Callback(void)
{
  LED_Off();
  ubAnalogWatchdog1Status = 0;
  LL_ADC_ClearFlag_AWD1(ADC1);
  LL_ADC_EnableIT_AWD1(ADC1);
}

void AdcAnalogWatchdog1_Callback()
{
  LL_ADC_DisableIT_AWD1(ADC1);
  ubAnalogWatchdog1Status = 1;
  LED_On();
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  while (1)
  {
  }
}
#endif
