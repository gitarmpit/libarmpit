#include "config.h"

#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_rtc.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_system.h"

#include "systick.h"

//#include "stm32f0xx.h"
#include <stdio.h>
#include "gpio.h"
#include "timer_cpp.h"


#define OB_VDDA_ANALOG_ON          ((uint8_t)0x20U) /*!< Analog monitoring on VDDA Power source ON */
#define OB_VDDA_ANALOG_OFF         ((uint8_t)0x00U) /*!< Analog monitoring on VDDA Power source OFF */

uint8_t FLASH_OB_GetUser(void);
void FLASH_OB_UserConfig(uint8_t UserConfig);
void HAL_FLASH_Unlock(void);
void HAL_FLASH_OB_Unlock(void);
void HAL_FLASH_OB_Launch(void);


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);


static void GPIO_floating(void) {
  LL_GPIO_InitTypeDef gpio_initstruct = {LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG, 
                                         LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_OUTPUT_PUSHPULL};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA |
                           LL_AHB1_GRP1_PERIPH_GPIOB);

  LL_GPIO_Init(GPIOA, &gpio_initstruct);
  LL_GPIO_Init(GPIOB, &gpio_initstruct);
  //LL_GPIO_Init(GPIOC, &gpio_initstruct);
  //LL_GPIO_Init(GPIOD, &gpio_initstruct);

  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOA |
                           LL_AHB1_GRP1_PERIPH_GPIOB);
}


// 1.9 uA:  VDDA_MONITOR is off in option bytes: AA 55 DF 20   (DF)
// 3.0 uA:  VDDA_MONITOR on  (FF)
static void StandBy(void) {

  LL_PWR_ClearFlag_WU();

  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  GPIO_floating();  // doesn't make any difference


  __WFI();
}

static void testStandBy(void) {
  // Check standby flag
  if (LL_PWR_IsActiveFlag_SB()) {
      LL_PWR_ClearFlag_SB();
      CLEAR_BIT(PWR->CSR, PWR_CSR_EWUP1_Msk);
      GPIO_PIN pin = GPIO_GetPin("B5");
      GPIO_Setup_OutPP (&pin);
      GPIO_SetPin(&pin);
      SysTick_Delay(1000);
  }
  //else {
    // Enable wakeup pin
  SET_BIT(PWR->CSR, PWR_CSR_EWUP1_Msk);
  StandBy();
  //}
}


static void setOptionBytes(void) {
  //uint8_t userConfig = FLASH_OB_GetUser();
  //printf ("%d\n", userConfig & OB_VDDA_ANALOG_ON);
  //userConfig &= ~OB_VDDA_ANALOG_ON;

  //HAL_FLASH_Unlock();
  //HAL_FLASH_OB_Unlock();
  //FLASH_OB_UserConfig(userConfig);
  //HAL_FLASH_OB_Launch();
}


extern "C" void RTC_IRQHandler(void) {
  static int i; 
  ++i;
  printf ("%d: alarm\n", SysTick_GetTick());
  if (RTC->ISR & RTC_ISR_ALRAF) {
    RTC->ISR &= ~RTC_ISR_ALRAF;
  }
  if (EXTI->PR & EXTI_PR_PR17) {
    EXTI->PR &= EXTI_PR_PR17;
  }
}

static void setupRTCForStandby(void) {

  LL_PWR_EnableBkUpAccess();
  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1)
     ;

  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);

  LL_RCC_EnableRTC();

  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_EnterInitMode(RTC);
  RTC->TR = 0;
  RTC->DR = 0;
  
  //LL_RTC_SetAsynchPrescaler(RTC, 6);
  //LL_RTC_SetSynchPrescaler(RTC, 5687); //39810


  LL_RTC_SetSynchPrescaler(RTC, 310); //39810

  LL_RTC_EnableShadowRegBypass(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  while (!LL_RTC_IsActiveFlag_ALRAW(RTC))
    ;
  
  //LL_RTC_ALMA_SetMinute (RTC, 1);
  //LL_RTC_ALMA_SetHour (RTC, 1);
  LL_RTC_ALMA_SetSecond (RTC, __LL_RTC_CONVERT_BIN2BCD(10));

  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_MINUTES);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_HOURS);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_SECONDS);

  //RTC->ALRMAR = RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1;

  //LL_RTC_SetAlarmOutEvent(RTC, LL_RTC_ALARMOUT_ALMA);
  
  LL_RTC_ALMA_Enable(RTC);
  LL_RTC_EnableIT_ALRA(RTC);

  
  // Calibration out
  //RTC->CR |= RTC_CR_COSEL | RTC_CR_COE;
  
  
  LL_RTC_ExitInitMode(RTC);


  LL_RTC_EnableWriteProtection(RTC);
}

static void testStandByAlarm(void) {
  // Check standby flag
  if (LL_PWR_IsActiveFlag_SB()) {
      LL_PWR_ClearFlag_SB();
      CLEAR_BIT(PWR->CSR, PWR_CSR_EWUP1_Msk);
      GPIO_PIN pin = GPIO_GetPin("B5");
      GPIO_Setup_OutPP (&pin);
      GPIO_SetPin(&pin);
      SysTick_Delay(10000);
  }
  setupRTCForStandby();
  StandBy();
}


static void testRTC(void) {

  //GPIO_PIN c13 = GPIO_GetPin ("c13");
  //GPIO_Setup_OutPP(&c13);

  LL_PWR_EnableBkUpAccess();
  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1)
     ;

  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);

  LL_RCC_EnableRTC();

  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_EnterInitMode(RTC);
  RTC->TR = 0;
  RTC->DR = 0;
  
  //LL_RTC_SetAsynchPrescaler(RTC, 6);
  //LL_RTC_SetSynchPrescaler(RTC, 5687); //39810


  LL_RTC_SetSynchPrescaler(RTC, 310); //39810

  LL_RTC_EnableShadowRegBypass(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  
  //LL_RTC_ALMA_SetMinute (RTC, 1);
  //LL_RTC_ALMA_SetHour (RTC, 1);
  LL_RTC_ALMA_SetSecond (RTC, __LL_RTC_CONVERT_BIN2BCD(10));

  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_MINUTES);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_HOURS);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_SECONDS);

  //RTC->ALRMAR = RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1;

  //LL_RTC_SetAlarmOutEvent(RTC, LL_RTC_ALARMOUT_ALMA);
  
  LL_RTC_ALMA_Enable(RTC);
  LL_RTC_EnableIT_ALRA(RTC);

  
  // Calibration out
  //RTC->CR |= RTC_CR_COSEL | RTC_CR_COE;
  
  
  LL_RTC_ExitInitMode(RTC);


  LL_RTC_EnableWriteProtection(RTC);

  LL_EXTI_EnableIT_0_31 (LL_EXTI_LINE_17);
  
  //LL_EXTI_EnableEvent_0_31 (LL_EXTI_LINE_17);
  LL_EXTI_EnableRisingTrig_0_31 (LL_EXTI_LINE_17);

  NVIC_SetPriority(RTC_IRQn, 0x0F);
  NVIC_EnableIRQ(RTC_IRQn);


  while(1) {
    //printf ("%d, %lu\n", RTC->TR, RTC->ISR & RTC_ISR_ALRAF);
    //if (RTC->ISR & RTC_ISR_ALRAF) {
    //  printf ("%d: alarm\n", SysTick_GetTick());
    //  while(1)
    //    ; 
    //}
    SysTick_Delay(10);
  }
    
}

static void SystemClock_Config_HSI(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(16);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_16);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }

}

void th (void* arg) {
  GPIO_PIN* pin = (GPIO_PIN*)arg;
  GPIO_TogglePin(pin);
}

class MyHandler : public TIMER_Interrupt_Handler {
public:
  MyHandler (GPIO_PIN* pin) : _pin (pin) {}
  virtual void HandleInterrupt() {
    GPIO_TogglePin(_pin);
  }
private:
  GPIO_PIN* _pin;
};


static void testTimer(void) {
  GPIO_PIN pin = GPIO_GetPin("A8");
  GPIO_Setup_OutPP(&pin);
  TIMER* t = TIMER1::GetInstance();
  //TIM_SetupCounter(TIM1, 600, th, (void*)&pin);
  MyHandler handler (&pin);
  t->SetupCounter(600);
  t->SetInterruptHandler(&handler);
  while(1) 
    ;
}

static void testPWM(void) {
  GPIO_PIN pin = GPIO_GetPin("A8");
  GPIO_Setup_OutAltPP(&pin);
  GPIO_SetAF(&pin, 2);
  //TIM_Channel ch = TIM_SetupPWM(TIM1, 1, 800, 600);
  TIMER* t = TIMER1::GetInstance();
  TIM_Channel ch = t->SetupPWM(1, 800, 100);
  while(1)
    ;
}

static void testPWM2(void) {
  GPIO_PIN pin = GPIO_GetPin("A6");
  GPIO_Setup_OutAltPP(&pin);
  GPIO_SetAF(&pin, 1);
  //TIM_Channel ch = TIM_SetupPWM(TIM1, 1, 800, 600);
  TIMER* t = TIMER3::GetInstance();
  TIM_Channel ch = t->SetupPWM(1, 3000, 100);
  SysTick_Delay(5000);
  t->DisableCounter();
  while(1)
    ;
  
}


static void MCO(void) {
  GPIO_PIN pin = GPIO_GetPin("A8");
  GPIO_Setup_OutAltPP(&pin);

  // LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_LSI, LL_RCC_MCO1_DIV_1);
  // LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_SYSCLK, LL_RCC_MCO1_DIV_1);
  // LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_HSI, LL_RCC_MCO1_DIV_1);
}

int main(void)
{
  System_Config();

  //SystemClock_Config();
  SystemClock_Config_HSI();

  // MCO();

  SystemCoreClockUpdate();
  SysTick_Init();



  //GPIO_PIN pin = GPIO_GetPin("B5");
  //GPIO_Setup_OutPP(&pin);
  //GPIO_SetPin(&pin);
  //GPIO_ResetPin(&pin);


  //testRTC();
  //testStandByAlarm();
  //StandBy();

  //testTimer();

  testPWM2();

  while (1)
    ;
}

#if 0
static void MX_TIM3_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM3, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM3);
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM3, LL_TIM_CHANNEL_CH1);
  LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM3);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**TIM3 GPIO Configuration
  PA6   ------> TIM3_CH1
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_9);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  //LL_SYSCFG_EnableFastModePlus(SYSCFG_CFGR1_I2C_FMP_PA9);

}

#endif
