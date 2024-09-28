#include "config.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_rcc.h"

#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_system.h"
#include "systick.h"
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_exti.h"
#include "gpio.h"
#include "timer_cpp.h"
#include "buzzer.h"
#include <stdio.h>

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



static void Sleep() {
  CLEAR_BIT(SCB->SCR, (uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
  CLEAR_BIT(PWR->CR, (PWR_CR_PDDS | PWR_CR_LPSDSR_Msk));
  //CLEAR_BIT(PWR->CR, PWR_CR_PDDS);
  LL_PWR_EnableUltraLowPower();
  __WFI();
}

static void GPIO_floating() {
  LL_GPIO_InitTypeDef gpio_initstruct = {LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG, 
                                         LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_OUTPUT_PUSHPULL};

  LL_APB2_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                           LL_IOP_GRP1_PERIPH_GPIOB |
                           LL_IOP_GRP1_PERIPH_GPIOC |
                           LL_IOP_GRP1_PERIPH_GPIOD);

  LL_GPIO_Init(GPIOA, &gpio_initstruct);
  LL_GPIO_Init(GPIOB, &gpio_initstruct);
  LL_GPIO_Init(GPIOC, &gpio_initstruct);
  LL_GPIO_Init(GPIOD, &gpio_initstruct);

  LL_APB2_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                           LL_IOP_GRP1_PERIPH_GPIOB |
                           LL_IOP_GRP1_PERIPH_GPIOC |
                           LL_IOP_GRP1_PERIPH_GPIOD);
}

// 18uA
static void EnterSTOP_MAINREGUMode(void)
{
  // What's the point? Can't we just turn clock off for all ports?
  LL_GPIO_InitTypeDef gpio_initstruct = {LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG, 
                                         LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_OUTPUT_PUSHPULL};

  LL_APB2_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                           LL_IOP_GRP1_PERIPH_GPIOB |
                           LL_IOP_GRP1_PERIPH_GPIOC |
                           LL_IOP_GRP1_PERIPH_GPIOD);

  LL_GPIO_Init(GPIOA, &gpio_initstruct);
  LL_GPIO_Init(GPIOB, &gpio_initstruct);
  LL_GPIO_Init(GPIOC, &gpio_initstruct);
  LL_GPIO_Init(GPIOD, &gpio_initstruct);

  LL_APB2_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                           LL_IOP_GRP1_PERIPH_GPIOB |
                           LL_IOP_GRP1_PERIPH_GPIOC |
                           LL_IOP_GRP1_PERIPH_GPIOD);

  /** Request to enter STOP_MAINREGU mode
    * Following procedure describe in STM32F1xx Reference Manual
    * See PWR part, section Low-power modes, STOP_MAINREGU mode
    */
  /* Set STOP_MAINREGU mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STOP);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  //LL_LPM_EnableDeepSleep();  
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* Request Wait For Interrupt */
  __WFI();
}


// 3uA  (5uA without ULP)  (18uA without ULP and LP reg)
static void EnterSTOPMode()
{
  LL_PWR_ClearFlag_WU();

  uint32_t tmpreg = 0U;
  uint32_t ulpbit, vrefinbit;

  tmpreg = PWR->CR;
  
  CLEAR_BIT(tmpreg, (PWR_CR_PDDS | PWR_CR_LPSDSR));

  // Low power regulator
  SET_BIT(tmpreg, PWR_CR_LPSDSR);

  /* Enable the Ultra Low Power mode */
  SET_BIT(tmpreg, PWR_CR_ULP);

  PWR->CR = tmpreg;

  SET_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);

  __WFI();
 
  CLEAR_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);

}


// 0.3 uA with Ultra low power
// 1.8 uA without Ultra low power
static void StandBy() {

  //SET_BIT(PWR->CR, PWR_CR_PDDS);
  //CLEAR_BIT(PWR->CSR, PWR_CSR_WUF_Msk);

  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

  CLEAR_BIT(PWR->CR, PWR_CR_PVDE);  // the default

//#define LL_PWR_REGU_VOLTAGE_SCALE1         (PWR_CR_VOS_0)                   /*!< 1.8V (range 1) */
//#define LL_PWR_REGU_VOLTAGE_SCALE2         (PWR_CR_VOS_1)                   /*!< 1.5V (range 2) */
//#define LL_PWR_REGU_VOLTAGE_SCALE3         (PWR_CR_VOS_0 | PWR_CR_VOS_1)    /*!< 1.2V (range 3) */

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);

  // MODIFY_REG(PWR->CR, PWR_CR_VOS, PWR_CR_VOS_0);

  // Set SLEEPDEEP bit of Cortex System Control Register
  LL_LPM_EnableDeepSleep();
  //SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
  // LL_PWR_EnableNVMKeptOff();
  
  LL_PWR_EnableUltraLowPower();

  //SET_BIT(PWR->CSR, PWR_CSR_EWUP_Msk);
  //CLEAR_BIT(PWR->CSR, PWR_CSR_EWUP_Msk);
  
  //GPIO_floating();

  __WFI();
}

// 35uA
// 62uA on L053R8T6  (64-pin)
static void lowPowerRun() {

  // POR / PVD : PVD

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
    ;

  // LL_PWR_REGU_VOLTAGE_SCALE1 : 1.8 V
  // LL_PWR_REGU_VOLTAGE_SCALE2 : 1.5 V
  // LL_PWR_REGU_VOLTAGE_SCALE3 : 1.2 V
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);

  while (LL_PWR_IsActiveFlag_VOS() != 0)
    ;

  LL_RCC_MSI_Enable();

  while(LL_RCC_MSI_IsReady() != 1)
    ;

//#define LL_RCC_MSIRANGE_0                  RCC_ICSCR_MSIRANGE_0  /*!< MSI = 65.536 KHz */
//#define LL_RCC_MSIRANGE_1                  RCC_ICSCR_MSIRANGE_1  /*!< MSI = 131.072 KHz*/
//#define LL_RCC_MSIRANGE_2                  RCC_ICSCR_MSIRANGE_2  /*!< MSI = 262.144 KHz */
//#define LL_RCC_MSIRANGE_3                  RCC_ICSCR_MSIRANGE_3  /*!< MSI = 524.288 KHz */
//#define LL_RCC_MSIRANGE_4                  RCC_ICSCR_MSIRANGE_4  /*!< MSI = 1.048 MHz */
//#define LL_RCC_MSIRANGE_5                  RCC_ICSCR_MSIRANGE_5  /*!< MSI = 2.097 MHz */
//#define LL_RCC_MSIRANGE_6                  RCC_ICSCR_MSIRANGE_6  /*!< MSI = 4.194 MHz */

  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_0);
  LL_RCC_MSI_SetCalibTrimming(0);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
    ;

  SystemCoreClockUpdate();


  /* Low-power deepsleep/Sleep/Low-power run
     When this bit is set, the regulator switches in low-power mode when the bit LPRUN is set

      The LPSDSR bit must be set before the LPRUN bit is set. 
      LPSDSR can be reset only when LPRUN bit=0.
  */
  PWR->CR |= PWR_CR_LPSDSR; 

  // Low-power run mode
  PWR->CR |= PWR_CR_LPRUN;

  // Turn PVD off (is it off by default?) 
  PWR->CR &= ~PWR_CR_PVDE;

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

  //LL_RTC_SetSynchPrescaler(RTC, 283); //36800

  LL_RTC_EnableShadowRegBypass(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  
  LL_RTC_ALMA_SetMinute (RTC, 1);
  //LL_RTC_ALMA_SetHour (RTC, 1);
  //LL_RTC_ALMA_SetSecond (RTC, __LL_RTC_CONVERT_BIN2BCD(10));

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
  
  LL_EXTI_EnableEvent_0_31 (LL_EXTI_LINE_17);
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


static void setupRTCForStandby(void) {

  LL_PWR_EnableBkUpAccess();

#if 0
  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1)
     ;
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
#else 
  LL_RCC_LSE_Enable();
  while (LL_RCC_LSE_IsReady() != 1)
     ;
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
#endif

  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();

  LL_RCC_EnableRTC();

  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_EnterInitMode(RTC);
  RTC->TR = 0;
  RTC->DR = 0;
  
  LL_RTC_SetSynchPrescaler(RTC, 310); //39810

  LL_RTC_EnableShadowRegBypass(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  while (!LL_RTC_IsActiveFlag_ALRAW(RTC))
    ;
  
  //LL_RTC_ALMA_SetMinute (RTC, 1);
  //LL_RTC_ALMA_SetHour (RTC, 1);
  LL_RTC_ALMA_SetSecond (RTC, __LL_RTC_CONVERT_BIN2BCD(5));

  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_MINUTES);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_HOURS);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_SECONDS);

  //RTC->ALRMAR = RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1;
  
  LL_RTC_ALMA_Enable(RTC);
  LL_RTC_EnableIT_ALRA(RTC);
  
  LL_RTC_ExitInitMode(RTC);
  LL_RTC_EnableWriteProtection(RTC);
}

static void run(void) {
  // Check standby flag
  if (LL_PWR_IsActiveFlag_SB()) {
    LL_PWR_ClearFlag_SB();
    if (LL_PWR_IsActiveFlag_WU()) {
      LL_PWR_ClearFlag_WU();
    }
    LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN1);
    CLEAR_BIT(PWR->CSR, PWR_CSR_EWUP1_Msk);
    LPBuzzerTimer timer;
    Buzzer* buzzer = Buzzer::GetInstance();
    buzzer->Init(&timer);
    buzzer->SetVolume(10);
    // buzzer->Stop();
    //buzzer->PlayTune("Melody:d=2,o=5,b=120:16c,16c,16c,g");
    buzzer->PlayTone(2000, 200);
    buzzer->Stop();

  }
  // setupRTCForStandby();
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);    // A0
  StandBy();
}

static void testStandBy() {
  // Check standby flag
  if (LL_PWR_IsActiveFlag_WU()) {
    LL_PWR_ClearFlag_WU();
  }
  else {
  }

  setupRTCForStandby();

  if (LL_PWR_IsActiveFlag_SB()) {
    LL_PWR_ClearFlag_SB();
    // CLEAR_BIT(PWR->CSR, PWR_CSR_EWUP1);
    LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN1);

    LPBuzzerTimer timer;
    Buzzer* buzzer = Buzzer::GetInstance();
    buzzer->Init(&timer);
    buzzer->SetVolume(10);
    //buzzer->PlayTune(barbie);
    buzzer->PlayTone(2000, 100);
    buzzer->Stop();
    for (int i = 0; i < 3000; ++i) {
      
    }
  }
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);    // A0
//  setupRTCForStandby();
  StandBy();
}


static void testGPIO() {

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /**/

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3);


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
  GPIO_PIN pin = GPIO_GetPin("A3");
  GPIO_Setup_OutPP(&pin);
  TIMER* t = TIMER6::GetInstance();
  //TIM_SetupCounter(TIM1, 600, th, (void*)&pin);
  MyHandler handler (&pin);
  t->SetupCounter(200000);
  t->SetInterruptHandler(&handler);
  while(1) 
    ;
}

static void testPWM(void) {
  GPIO_PIN pin = GPIO_GetPin("A3");
  GPIO_Setup_OutAltPP(&pin);
  GPIO_SetAF(&pin, 0);
  TIMER* t = TIMER21::GetInstance();
  TIM_Channel ch = t->SetupPWM(2, 400000, 200000);
  while(1)
    ;
}


GPIO_PIN* g_pin;

void th (void* ctx) {
  //GPIO_PIN* pin = (GPIO_PIN*) ctx;
  GPIO_TogglePin(g_pin);
}

static void testLPTIM(void) {
  GPIO_PIN pin = GPIO_GetPin("A8");
  GPIO_Setup_OutPP(&pin);

  LPTIMER* t = LPTIMER::GetInstance();
  GPIO_Setup_OutPP(&pin);
  MyHandler handler (&pin);
  t->SetupCounter(80000);
  t->SetInterruptHandler(&handler);
  while(1)
    ;
}

static void testLPTIM_PWM(void) {
  GPIO_PIN pin = GPIO_GetPin("B2");
  GPIO_Setup_OutAltPP(&pin);
  GPIO_SetAF(&pin, 2);

  LPTIMER* t = LPTIMER::GetInstance();
  t->SetupPWM(2000, 1500);
  while(1)
    ;

}

static void testBuzzer(void) {
  LPBuzzerTimer timer;
  Buzzer* buzzer = Buzzer::GetInstance();
  buzzer->Init(&timer);
  buzzer->SetVolume(20);
  // buzzer->Stop();

  //buzzer->PlayTune("Melody:d=2,o=5,b=120:16c,16c,16c,g");

  buzzer->PlayTone(1000, 200);
  buzzer->Stop();
  buzzer->PlayTune(barbie);
  buzzer->Stop();
  while(1)
  ;
}

static void testMCO(void) {

  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1)
     ;

  GPIO_PIN mco = GPIO_GetPin("A8");
  GPIO_Setup_OutAltPP (&mco);
  GPIO_SetAF (&mco, 0);

  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_LSI, LL_RCC_MCO1_DIV_1);
}

// MSI 2MHz : 260uA
// MSI 2Mhz + SysTick: 300uA
int main(void) {

  if (LL_PWR_IsActiveFlag_SB()) {
    LL_PWR_ClearFlag_SB();
  }

  if (LL_PWR_IsActiveFlag_WU()) {
    LL_PWR_ClearFlag_WU();
  }

  System_Config();
  //GPIO_floating();
  //GPIO_PIN pa14 = GPIO_GetPin("A14");
  //GPIO_Setup_OutAltPP(&pa14);
  //GPIO_SetAF(&pa14, 4);

  //testTimer();
  //testPWM();
  //testLPTIM2();

  lowPowerRun();
  //testBuzzer();
  //testLPTIM_PWM();
  //GPIO_floating();
  //SystemCoreClockUpdate();
  //SysTick_Init();


  //testMCO();

  //testRTC();


  testStandBy();
  //setupRTCForStandby();

  // run();

  //StandBy();

  while(1)
    ;

  if (LL_PWR_IsActiveFlag_WU()) {
    LL_PWR_ClearFlag_WU();
  }

  //testGPIO();


  // EnterSTOPMode();

  //SystemClock_Config_MSI();

  //Sleep();

  /* Set SLEEPONEXIT bit of Cortex System Control Register */
  // SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));

  // EnterSTOP_MAINREGUMode();

  /*
  LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_PCLK1);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1);
  LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_PCLK1);
  */
  

  while(1)
    ;

}


