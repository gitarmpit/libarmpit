#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_dac.h"
#include "stm32f4xx_ll_pwr.h"


#define LED1_PIN LL_GPIO_PIN_0
#define LED1_GPIO_PORT GPIOB
#define LED1_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)
#define LED_BLINK_FAST 200
#define LED_BLINK_SLOW 500
#define LED_BLINK_ERROR 1000
#define USER_BUTTON_PIN LL_GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC)
#define USER_BUTTON_EXTI_LINE LL_EXTI_LINE_13
#define USER_BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define USER_BUTTON_EXTI_LINE_ENABLE() LL_EXTI_EnableIT_0_31(USER_BUTTON_EXTI_LINE)
#define USER_BUTTON_EXTI_FALLING_TRIG_ENABLE() LL_EXTI_EnableFallingTrig_0_31(USER_BUTTON_EXTI_LINE)
#define USER_BUTTON_SYSCFG_SET_EXTI() do { \
                                                  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG); \
                                                  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13); \
                                                } while(0)
#define USER_BUTTON_IRQHANDLER EXTI15_10_IRQHandler

void UserButton_Callback(void);
void DacDmaTransferError_Callback(void);
void DacUnderrunError_Callback(void);

  #define VDDA_APPLI ((uint32_t)3300)
  #define DIGITAL_SCALE_12BITS (__LL_DAC_DIGITAL_SCALE(LL_DAC_RESOLUTION_12B))
  #define WAVEFORM_AMPLITUDE (VDDA_APPLI)
  #define WAVEFORM_FREQUENCY ((uint32_t)1000)
  #define WAVEFORM_SAMPLES_SIZE (sizeof (WaveformSine_12bits_32samples) / sizeof (uint16_t))
  #define WAVEFORM_TIMER_FREQUENCY (WAVEFORM_FREQUENCY * WAVEFORM_SAMPLES_SIZE)
  #define WAVEFORM_TIMER_FREQUENCY_RANGE_MIN ((uint32_t) 1)
  #define WAVEFORM_TIMER_PRESCALER_MAX_VALUE ((uint32_t)0xFFFF-1)

#define __WAVEFORM_AMPLITUDE_SCALING(__DATA_12BITS__) \
  (__DATA_12BITS__ \
   * __LL_DAC_CALC_VOLTAGE_TO_DATA(VDDA_APPLI, WAVEFORM_AMPLITUDE, LL_DAC_RESOLUTION_12B) \
   / __LL_DAC_DIGITAL_SCALE(LL_DAC_RESOLUTION_12B) \
  )


__IO uint8_t ubButtonPress = 0;

const uint16_t WaveformSine_12bits_32samples[] =
{
__WAVEFORM_AMPLITUDE_SCALING(2048),
__WAVEFORM_AMPLITUDE_SCALING(2447),
__WAVEFORM_AMPLITUDE_SCALING(2831),
__WAVEFORM_AMPLITUDE_SCALING(3185),
__WAVEFORM_AMPLITUDE_SCALING(3495),
__WAVEFORM_AMPLITUDE_SCALING(3750),
__WAVEFORM_AMPLITUDE_SCALING(3939),
__WAVEFORM_AMPLITUDE_SCALING(4056),
__WAVEFORM_AMPLITUDE_SCALING(4095),
__WAVEFORM_AMPLITUDE_SCALING(4056),
__WAVEFORM_AMPLITUDE_SCALING(3939),
__WAVEFORM_AMPLITUDE_SCALING(3750),
__WAVEFORM_AMPLITUDE_SCALING(3495),
__WAVEFORM_AMPLITUDE_SCALING(3185),
__WAVEFORM_AMPLITUDE_SCALING(2831),
__WAVEFORM_AMPLITUDE_SCALING(2447),
__WAVEFORM_AMPLITUDE_SCALING(2048),
__WAVEFORM_AMPLITUDE_SCALING(1649),
__WAVEFORM_AMPLITUDE_SCALING(1265),
__WAVEFORM_AMPLITUDE_SCALING(911),
__WAVEFORM_AMPLITUDE_SCALING(601),
__WAVEFORM_AMPLITUDE_SCALING(346),
__WAVEFORM_AMPLITUDE_SCALING(157),
__WAVEFORM_AMPLITUDE_SCALING(40),
__WAVEFORM_AMPLITUDE_SCALING(0),
__WAVEFORM_AMPLITUDE_SCALING(40),
__WAVEFORM_AMPLITUDE_SCALING(157),
__WAVEFORM_AMPLITUDE_SCALING(346),
__WAVEFORM_AMPLITUDE_SCALING(601),
__WAVEFORM_AMPLITUDE_SCALING(911),
__WAVEFORM_AMPLITUDE_SCALING(1265),
__WAVEFORM_AMPLITUDE_SCALING(1649)
};


void SystemClock_Config(void);
void Configure_DMA(void);
void Configure_TIM_TimeBase_DAC_trigger(void);
void Configure_DAC(void);
void Activate_DAC(void);
void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Blinking(uint32_t Period);
void UserButton_Init(void);
void WaitForUserButtonPress(void);
int main(void)
{

  SystemClock_Config();
  LED_Init();
  UserButton_Init();
  WaitForUserButtonPress();
  LED_Off();
  Configure_DMA();
  Configure_TIM_TimeBase_DAC_trigger();
  Configure_DAC();
  Activate_DAC();
  LED_On();
  while (1)
  {
  }
}

void Configure_DMA(void)
{
  NVIC_SetPriority(DMA1_Stream5_IRQn, 1);
  NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_5, LL_DMA_CHANNEL_7);
  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_STREAM_5,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
                        LL_DMA_MODE_CIRCULAR |
                        LL_DMA_PERIPH_NOINCREMENT |
                        LL_DMA_MEMORY_INCREMENT |
                        LL_DMA_PDATAALIGN_HALFWORD |
                        LL_DMA_MDATAALIGN_HALFWORD |
                        LL_DMA_PRIORITY_HIGH );

  LL_DMA_ConfigAddresses(DMA1,
                         LL_DMA_STREAM_5,
                         (uint32_t)&WaveformSine_12bits_32samples,
                         LL_DAC_DMA_GetRegAddr(DAC1, LL_DAC_CHANNEL_1, LL_DAC_DMA_REG_DATA_12BITS_RIGHT_ALIGNED),
                         LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetDataLength(DMA1,
                       LL_DMA_STREAM_5,
                       WAVEFORM_SAMPLES_SIZE);


  LL_DMA_EnableIT_TE(DMA1,
                     LL_DMA_STREAM_5);
  LL_DMA_EnableStream(DMA1,
                       LL_DMA_STREAM_5);
}

void Configure_TIM_TimeBase_DAC_trigger(void)
{
  uint32_t timer_clock_frequency = 0;
  uint32_t timer_prescaler = 0;
  uint32_t timer_reload = 0;
  if (LL_RCC_GetAPB1Prescaler() == LL_RCC_APB1_DIV_1)
  {
    timer_clock_frequency = __LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock, LL_RCC_GetAPB1Prescaler());
  }
  else
  {
    timer_clock_frequency = (__LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock, LL_RCC_GetAPB1Prescaler()) * 2);
  }

  timer_prescaler = ((timer_clock_frequency / (WAVEFORM_TIMER_PRESCALER_MAX_VALUE * WAVEFORM_TIMER_FREQUENCY_RANGE_MIN)) +1);
  timer_reload = (timer_clock_frequency / (timer_prescaler * WAVEFORM_TIMER_FREQUENCY));
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);
  LL_TIM_SetPrescaler(TIM6, (timer_prescaler - 1));
  LL_TIM_SetAutoReload(TIM6, (timer_reload - 1));
  LL_TIM_SetCounterMode(TIM6, LL_TIM_COUNTERMODE_UP);
  LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_UPDATE);
  LL_TIM_EnableCounter(TIM6);
}

void Configure_DAC(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
  NVIC_SetPriority(TIM6_DAC_IRQn, 0);
  NVIC_EnableIRQ(TIM6_DAC_IRQn);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);
  LL_DAC_SetTriggerSource(DAC1, LL_DAC_CHANNEL_1, LL_DAC_TRIG_EXT_TIM6_TRGO);
  LL_DAC_EnableDMAReq(DAC1, LL_DAC_CHANNEL_1);
  LL_DAC_EnableIT_DMAUDR1(DAC1);
}

void Activate_DAC(void)
{
  __IO uint32_t wait_loop_index = 0;
  LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);

  wait_loop_index = ((LL_DAC_DELAY_STARTUP_VOLTAGE_SETTLING_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
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

void LED_Off(void)
{
  LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

void LED_Blinking(uint32_t Period)
{
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
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
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);
}

void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(LED_BLINK_FAST);
  }
  ubButtonPress = 0;
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


  SysTick_Config(180000000 / 1000);
  SystemCoreClock = 180000000;
}

void UserButton_Callback(void)
{
  if(ubButtonPress == 0)
  {
    ubButtonPress = 1;
  }
}

void DacDmaTransferError_Callback()
{
  LED_Blinking(LED_BLINK_ERROR);
}

void DacUnderrunError_Callback(void)
{
  LL_DAC_DisableIT_DMAUDR1(DAC1);
  LED_Blinking(LED_BLINK_ERROR);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  while (1)
  {
  }
}
#endif
