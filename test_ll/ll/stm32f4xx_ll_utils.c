#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_pwr.h"
#define assert_param(expr) ((void)0U)

#if defined(RCC_MAX_FREQUENCY_SCALE1)
#define UTILS_MAX_FREQUENCY_SCALE1 RCC_MAX_FREQUENCY
#endif
#define UTILS_MAX_FREQUENCY_SCALE2 RCC_MAX_FREQUENCY_SCALE2
#if defined(RCC_MAX_FREQUENCY_SCALE3)
#define UTILS_MAX_FREQUENCY_SCALE3 RCC_MAX_FREQUENCY_SCALE3
#endif


#define UTILS_PLLVCO_INPUT_MIN RCC_PLLVCO_INPUT_MIN
#define UTILS_PLLVCO_INPUT_MAX RCC_PLLVCO_INPUT_MAX
#define UTILS_PLLVCO_OUTPUT_MIN RCC_PLLVCO_OUTPUT_MIN
#define UTILS_PLLVCO_OUTPUT_MAX RCC_PLLVCO_OUTPUT_MAX


#define UTILS_HSE_FREQUENCY_MIN 4000000U
#define UTILS_HSE_FREQUENCY_MAX 26000000U


#if defined(FLASH_SCALE1_LATENCY1_FREQ)
#define UTILS_SCALE1_LATENCY1_FREQ FLASH_SCALE1_LATENCY1_FREQ
#endif
#if defined(FLASH_SCALE1_LATENCY2_FREQ)
#define UTILS_SCALE1_LATENCY2_FREQ FLASH_SCALE1_LATENCY2_FREQ
#endif
#if defined(FLASH_SCALE1_LATENCY3_FREQ)
#define UTILS_SCALE1_LATENCY3_FREQ FLASH_SCALE1_LATENCY3_FREQ
#endif
#if defined(FLASH_SCALE1_LATENCY4_FREQ)
#define UTILS_SCALE1_LATENCY4_FREQ FLASH_SCALE1_LATENCY4_FREQ
#endif
#if defined(FLASH_SCALE1_LATENCY5_FREQ)
#define UTILS_SCALE1_LATENCY5_FREQ FLASH_SCALE1_LATENCY5_FREQ
#endif
#define UTILS_SCALE2_LATENCY1_FREQ FLASH_SCALE2_LATENCY1_FREQ
#define UTILS_SCALE2_LATENCY2_FREQ FLASH_SCALE2_LATENCY2_FREQ
#if defined(FLASH_SCALE2_LATENCY3_FREQ)
#define UTILS_SCALE2_LATENCY3_FREQ FLASH_SCALE2_LATENCY3_FREQ
#endif
#if defined(FLASH_SCALE2_LATENCY4_FREQ)
#define UTILS_SCALE2_LATENCY4_FREQ FLASH_SCALE2_LATENCY4_FREQ
#endif
#if defined(FLASH_SCALE2_LATENCY5_FREQ)
#define UTILS_SCALE2_LATENCY5_FREQ FLASH_SCALE2_LATENCY5_FREQ
#endif
#if defined(FLASH_SCALE3_LATENCY1_FREQ)
#define UTILS_SCALE3_LATENCY1_FREQ FLASH_SCALE3_LATENCY1_FREQ
#endif
#if defined(FLASH_SCALE3_LATENCY2_FREQ)
#define UTILS_SCALE3_LATENCY2_FREQ FLASH_SCALE3_LATENCY2_FREQ
#endif
#if defined(FLASH_SCALE3_LATENCY3_FREQ)
#define UTILS_SCALE3_LATENCY3_FREQ FLASH_SCALE3_LATENCY3_FREQ
#endif
#if defined(FLASH_SCALE3_LATENCY4_FREQ)
#define UTILS_SCALE3_LATENCY4_FREQ FLASH_SCALE3_LATENCY4_FREQ
#endif
#if defined(FLASH_SCALE3_LATENCY5_FREQ)
#define UTILS_SCALE3_LATENCY5_FREQ FLASH_SCALE3_LATENCY5_FREQ
#endif

#define IS_LL_UTILS_SYSCLK_DIV(__VALUE__) (((__VALUE__) == LL_RCC_SYSCLK_DIV_1) \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_2) \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_4) \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_8) \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_16) \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_64) \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_128) \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_256) \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_512))

#define IS_LL_UTILS_APB1_DIV(__VALUE__) (((__VALUE__) == LL_RCC_APB1_DIV_1) \
                                      || ((__VALUE__) == LL_RCC_APB1_DIV_2) \
                                      || ((__VALUE__) == LL_RCC_APB1_DIV_4) \
                                      || ((__VALUE__) == LL_RCC_APB1_DIV_8) \
                                      || ((__VALUE__) == LL_RCC_APB1_DIV_16))

#define IS_LL_UTILS_APB2_DIV(__VALUE__) (((__VALUE__) == LL_RCC_APB2_DIV_1) \
                                      || ((__VALUE__) == LL_RCC_APB2_DIV_2) \
                                      || ((__VALUE__) == LL_RCC_APB2_DIV_4) \
                                      || ((__VALUE__) == LL_RCC_APB2_DIV_8) \
                                      || ((__VALUE__) == LL_RCC_APB2_DIV_16))

#define IS_LL_UTILS_PLLM_VALUE(__VALUE__) (((__VALUE__) == LL_RCC_PLLM_DIV_2) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_3) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_4) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_5) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_6) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_7) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_8) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_9) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_10) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_11) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_12) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_13) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_14) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_15) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_16) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_17) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_18) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_19) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_20) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_21) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_22) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_23) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_24) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_25) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_26) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_27) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_28) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_29) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_30) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_31) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_32) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_33) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_34) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_35) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_36) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_37) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_38) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_39) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_40) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_41) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_42) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_43) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_44) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_45) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_46) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_47) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_48) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_49) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_50) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_51) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_52) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_53) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_54) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_55) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_56) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_57) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_58) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_59) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_60) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_61) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_62) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_63))

#define IS_LL_UTILS_PLLN_VALUE(__VALUE__) ((RCC_PLLN_MIN_VALUE <= (__VALUE__)) && ((__VALUE__) <= RCC_PLLN_MAX_VALUE))

#define IS_LL_UTILS_PLLP_VALUE(__VALUE__) (((__VALUE__) == LL_RCC_PLLP_DIV_2) \
                                        || ((__VALUE__) == LL_RCC_PLLP_DIV_4) \
                                        || ((__VALUE__) == LL_RCC_PLLP_DIV_6) \
                                        || ((__VALUE__) == LL_RCC_PLLP_DIV_8))

#define IS_LL_UTILS_PLLVCO_INPUT(__VALUE__) ((UTILS_PLLVCO_INPUT_MIN <= (__VALUE__)) && ((__VALUE__) <= UTILS_PLLVCO_INPUT_MAX))

#define IS_LL_UTILS_PLLVCO_OUTPUT(__VALUE__) ((UTILS_PLLVCO_OUTPUT_MIN <= (__VALUE__)) && ((__VALUE__) <= UTILS_PLLVCO_OUTPUT_MAX))

#if !defined(RCC_MAX_FREQUENCY_SCALE1)
#define IS_LL_UTILS_PLL_FREQUENCY(__VALUE__) ((LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE2) ? ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE2) : \
                                             ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE3))

#elif defined(RCC_MAX_FREQUENCY_SCALE3)
#define IS_LL_UTILS_PLL_FREQUENCY(__VALUE__) ((LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE1) ? ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE1) : \
                                              (LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE2) ? ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE2) : \
                                              ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE3))

#else
#define IS_LL_UTILS_PLL_FREQUENCY(__VALUE__) ((LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE1) ? ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE1) : \
                                             ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE2))

#endif
#define IS_LL_UTILS_HSE_BYPASS(__STATE__) (((__STATE__) == LL_UTILS_HSEBYPASS_ON) \
                                        || ((__STATE__) == LL_UTILS_HSEBYPASS_OFF))

#define IS_LL_UTILS_HSE_FREQUENCY(__FREQUENCY__) (((__FREQUENCY__) >= UTILS_HSE_FREQUENCY_MIN) && ((__FREQUENCY__) <= UTILS_HSE_FREQUENCY_MAX))



static uint32_t UTILS_GetPLLOutputFrequency(uint32_t PLL_InputFrequency,
                                               LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct);
static ErrorStatus UTILS_SetFlashLatency(uint32_t HCLK_Frequency);
static ErrorStatus UTILS_EnablePLLAndSwitchSystem(uint32_t SYSCLK_Frequency, LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct);
static ErrorStatus UTILS_PLL_IsBusy(void);
void LL_Init1msTick(uint32_t HCLKFrequency)
{
  LL_InitTick(HCLKFrequency, 1000U);
}

void LL_mDelay(uint32_t Delay)
{
  __IO uint32_t tmp = SysTick->CTRL;

  ((void)tmp);


  if(Delay < LL_MAX_DELAY)
  {
    Delay++;
  }

  while (Delay)
  {
    if((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      Delay--;
    }
  }
}

void LL_SetSystemCoreClock(uint32_t HCLKFrequency)
{

  SystemCoreClock = HCLKFrequency;
}

ErrorStatus LL_PLL_ConfigSystemClock_HSI(LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct,
                                         LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct)
{
  ErrorStatus status = SUCCESS;
  uint32_t pllfreq = 0U;


  if(UTILS_PLL_IsBusy() == SUCCESS)
  {

    pllfreq = UTILS_GetPLLOutputFrequency(HSI_VALUE, UTILS_PLLInitStruct);


    if(LL_RCC_HSI_IsReady() != 1U)
    {
      LL_RCC_HSI_Enable();
      while (LL_RCC_HSI_IsReady() != 1U)
      {

      }
    }


    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, UTILS_PLLInitStruct->PLLM, UTILS_PLLInitStruct->PLLN,
                                UTILS_PLLInitStruct->PLLP);


    status = UTILS_EnablePLLAndSwitchSystem(pllfreq, UTILS_ClkInitStruct);
  }
  else
  {

    status = ERROR;
  }

  return status;
}

ErrorStatus LL_PLL_ConfigSystemClock_HSE(uint32_t HSEFrequency, uint32_t HSEBypass,
                                         LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct, LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct)
{
  ErrorStatus status = SUCCESS;
  uint32_t pllfreq = 0U;


  assert_param(IS_LL_UTILS_HSE_FREQUENCY(HSEFrequency));
  assert_param(IS_LL_UTILS_HSE_BYPASS(HSEBypass));


  if(UTILS_PLL_IsBusy() == SUCCESS)
  {

    pllfreq = UTILS_GetPLLOutputFrequency(HSEFrequency, UTILS_PLLInitStruct);


    if(LL_RCC_HSE_IsReady() != 1U)
    {

      if(HSEBypass == LL_UTILS_HSEBYPASS_ON)
      {
        LL_RCC_HSE_EnableBypass();
      }
      else
      {
        LL_RCC_HSE_DisableBypass();
      }


      LL_RCC_HSE_Enable();
      while (LL_RCC_HSE_IsReady() != 1U)
      {

      }
    }


    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, UTILS_PLLInitStruct->PLLM, UTILS_PLLInitStruct->PLLN,
                                UTILS_PLLInitStruct->PLLP);


    status = UTILS_EnablePLLAndSwitchSystem(pllfreq, UTILS_ClkInitStruct);
  }
  else
  {

    status = ERROR;
  }

  return status;
}

static ErrorStatus UTILS_SetFlashLatency(uint32_t HCLK_Frequency)
{
  ErrorStatus status = SUCCESS;

  uint32_t latency = LL_FLASH_LATENCY_0;


  if(HCLK_Frequency == 0U)
  {
    status = ERROR;
  }
  else
  {
    if(LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE1)
    {
#if defined (UTILS_SCALE1_LATENCY5_FREQ)
      if((HCLK_Frequency > UTILS_SCALE1_LATENCY5_FREQ)&&(latency == LL_FLASH_LATENCY_0))
      {
        latency = LL_FLASH_LATENCY_5;
      }
#endif
#if defined (UTILS_SCALE1_LATENCY4_FREQ)
      if((HCLK_Frequency > UTILS_SCALE1_LATENCY4_FREQ)&&(latency == LL_FLASH_LATENCY_0))
      {
        latency = LL_FLASH_LATENCY_4;
      }
#endif
#if defined (UTILS_SCALE1_LATENCY3_FREQ)
      if((HCLK_Frequency > UTILS_SCALE1_LATENCY3_FREQ)&&(latency == LL_FLASH_LATENCY_0))
      {
        latency = LL_FLASH_LATENCY_3;
      }
#endif
#if defined (UTILS_SCALE1_LATENCY2_FREQ)
      if((HCLK_Frequency > UTILS_SCALE1_LATENCY2_FREQ)&&(latency == LL_FLASH_LATENCY_0))
      {
        latency = LL_FLASH_LATENCY_2;
      }
      else
      {
        if((HCLK_Frequency > UTILS_SCALE1_LATENCY1_FREQ)&&(latency == LL_FLASH_LATENCY_0))
        {
          latency = LL_FLASH_LATENCY_1;
        }
      }
#endif
    }
    if(LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE2)
    {
#if defined (UTILS_SCALE2_LATENCY5_FREQ)
      if((HCLK_Frequency > UTILS_SCALE2_LATENCY5_FREQ)&&(latency == LL_FLASH_LATENCY_0))
      {
        latency = LL_FLASH_LATENCY_5;
      }
#endif
#if defined (UTILS_SCALE2_LATENCY4_FREQ)
      if((HCLK_Frequency > UTILS_SCALE2_LATENCY4_FREQ)&&(latency == LL_FLASH_LATENCY_0))
      {
        latency = LL_FLASH_LATENCY_4;
      }
#endif
#if defined (UTILS_SCALE2_LATENCY3_FREQ)
      if((HCLK_Frequency > UTILS_SCALE2_LATENCY3_FREQ)&&(latency == LL_FLASH_LATENCY_0))
      {
        latency = LL_FLASH_LATENCY_3;
      }
#endif
      if((HCLK_Frequency > UTILS_SCALE2_LATENCY2_FREQ)&&(latency == LL_FLASH_LATENCY_0))
      {
        latency = LL_FLASH_LATENCY_2;
      }
      else
      {
        if((HCLK_Frequency > UTILS_SCALE2_LATENCY1_FREQ)&&(latency == LL_FLASH_LATENCY_0))
        {
          latency = LL_FLASH_LATENCY_1;
        }
      }
    }
#if defined (LL_PWR_REGU_VOLTAGE_SCALE3)
    if(LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE3)
    {
#if defined (UTILS_SCALE3_LATENCY3_FREQ)
      if((HCLK_Frequency > UTILS_SCALE3_LATENCY3_FREQ)&&(latency == LL_FLASH_LATENCY_0))
      {
        latency = LL_FLASH_LATENCY_3;
      }
#endif
#if defined (UTILS_SCALE3_LATENCY2_FREQ)
      if((HCLK_Frequency > UTILS_SCALE3_LATENCY2_FREQ)&&(latency == LL_FLASH_LATENCY_0))
      {
        latency = LL_FLASH_LATENCY_2;
      }
      else
      {
        if((HCLK_Frequency > UTILS_SCALE3_LATENCY1_FREQ)&&(latency == LL_FLASH_LATENCY_0))
        {
          latency = LL_FLASH_LATENCY_1;
        }
      }
    }
#endif
#endif

    LL_FLASH_SetLatency(latency);



    if(LL_FLASH_GetLatency() != latency)
    {
      status = ERROR;
    }
  }
  return status;
}

static uint32_t UTILS_GetPLLOutputFrequency(uint32_t PLL_InputFrequency, LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct)
{
  uint32_t pllfreq = 0U;


  assert_param(IS_LL_UTILS_PLLM_VALUE(UTILS_PLLInitStruct->PLLM));
  assert_param(IS_LL_UTILS_PLLN_VALUE(UTILS_PLLInitStruct->PLLN));
  assert_param(IS_LL_UTILS_PLLP_VALUE(UTILS_PLLInitStruct->PLLP));

  pllfreq = PLL_InputFrequency / (UTILS_PLLInitStruct->PLLM & (RCC_PLLCFGR_PLLM >> RCC_PLLCFGR_PLLM_Pos));
  assert_param(IS_LL_UTILS_PLLVCO_INPUT(pllfreq));

  pllfreq = pllfreq * (UTILS_PLLInitStruct->PLLN & (RCC_PLLCFGR_PLLN >> RCC_PLLCFGR_PLLN_Pos));
  assert_param(IS_LL_UTILS_PLLVCO_OUTPUT(pllfreq));


  pllfreq = pllfreq / (((UTILS_PLLInitStruct->PLLP >> RCC_PLLCFGR_PLLP_Pos) + 1) * 2);
  assert_param(IS_LL_UTILS_PLL_FREQUENCY(pllfreq));

  return pllfreq;
}


static ErrorStatus UTILS_PLL_IsBusy(void)
{
  ErrorStatus status = SUCCESS;


  if(LL_RCC_PLL_IsReady() != 0U)
  {

    status = ERROR;
  }

#if defined(RCC_PLLSAI_SUPPORT)

  if(LL_RCC_PLLSAI_IsReady() != 0U)
  {

    status = ERROR;
  }
#endif
#if defined(RCC_PLLI2S_SUPPORT)

  if(LL_RCC_PLLI2S_IsReady() != 0U)
  {

    status = ERROR;
  }
#endif
  return status;
}

static ErrorStatus UTILS_EnablePLLAndSwitchSystem(uint32_t SYSCLK_Frequency, LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct)
{
  ErrorStatus status = SUCCESS;
  uint32_t hclk_frequency = 0U;

  assert_param(IS_LL_UTILS_SYSCLK_DIV(UTILS_ClkInitStruct->AHBCLKDivider));
  assert_param(IS_LL_UTILS_APB1_DIV(UTILS_ClkInitStruct->APB1CLKDivider));
  assert_param(IS_LL_UTILS_APB2_DIV(UTILS_ClkInitStruct->APB2CLKDivider));

  hclk_frequency = __LL_RCC_CALC_HCLK_FREQ(SYSCLK_Frequency, UTILS_ClkInitStruct->AHBCLKDivider);

  if(SystemCoreClock < hclk_frequency)
  {
    status = UTILS_SetFlashLatency(hclk_frequency);
  }

  if(status == SUCCESS)
  {
    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1U)
    {
    }

    LL_RCC_SetAHBPrescaler(UTILS_ClkInitStruct->AHBCLKDivider);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }


    LL_RCC_SetAPB1Prescaler(UTILS_ClkInitStruct->APB1CLKDivider);
    LL_RCC_SetAPB2Prescaler(UTILS_ClkInitStruct->APB2CLKDivider);
  }

  if(SystemCoreClock > hclk_frequency)
  {
    status = UTILS_SetFlashLatency(hclk_frequency);
  }


  if(status == SUCCESS)
  {
    LL_SetSystemCoreClock(hclk_frequency);
  }

  return status;
}
