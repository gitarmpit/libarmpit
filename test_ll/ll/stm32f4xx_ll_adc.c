#if defined(USE_FULL_LL_DRIVER)

#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_bus.h"

#define assert_param(expr) ((void)0U)

#if defined (ADC1) || defined (ADC2) || defined (ADC3)
#define IS_LL_ADC_COMMON_CLOCK(__CLOCK__) \
  ( ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV2) \
   || ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV4) \
   || ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV6) \
   || ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV8) \
  )

#define IS_LL_ADC_RESOLUTION(__RESOLUTION__) \
  ( ((__RESOLUTION__) == LL_ADC_RESOLUTION_12B) \
   || ((__RESOLUTION__) == LL_ADC_RESOLUTION_10B) \
   || ((__RESOLUTION__) == LL_ADC_RESOLUTION_8B) \
   || ((__RESOLUTION__) == LL_ADC_RESOLUTION_6B) \
  )

#define IS_LL_ADC_DATA_ALIGN(__DATA_ALIGN__) \
  ( ((__DATA_ALIGN__) == LL_ADC_DATA_ALIGN_RIGHT) \
   || ((__DATA_ALIGN__) == LL_ADC_DATA_ALIGN_LEFT) \
  )

#define IS_LL_ADC_SCAN_SELECTION(__SCAN_SELECTION__) \
  ( ((__SCAN_SELECTION__) == LL_ADC_SEQ_SCAN_DISABLE) \
   || ((__SCAN_SELECTION__) == LL_ADC_SEQ_SCAN_ENABLE) \
  )

#define IS_LL_ADC_SEQ_SCAN_MODE(__SEQ_SCAN_MODE__) \
  ( ((__SCAN_MODE__) == LL_ADC_SEQ_SCAN_DISABLE) \
   || ((__SCAN_MODE__) == LL_ADC_SEQ_SCAN_ENABLE) \
  )



#define IS_LL_ADC_REG_TRIG_SOURCE(__REG_TRIG_SOURCE__) \
  ( ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_SOFTWARE) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM1_CH1) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM1_CH2) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM1_CH3) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM2_CH2) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM2_CH3) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM2_CH4) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM2_TRGO) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM3_CH1) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM3_TRGO) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM4_CH4) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM5_CH1) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM5_CH2) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM5_CH3) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM8_CH1) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM8_TRGO) \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_EXTI_LINE11) \
  )
#define IS_LL_ADC_REG_CONTINUOUS_MODE(__REG_CONTINUOUS_MODE__) \
  ( ((__REG_CONTINUOUS_MODE__) == LL_ADC_REG_CONV_SINGLE) \
   || ((__REG_CONTINUOUS_MODE__) == LL_ADC_REG_CONV_CONTINUOUS) \
  )

#define IS_LL_ADC_REG_DMA_TRANSFER(__REG_DMA_TRANSFER__) \
  ( ((__REG_DMA_TRANSFER__) == LL_ADC_REG_DMA_TRANSFER_NONE) \
   || ((__REG_DMA_TRANSFER__) == LL_ADC_REG_DMA_TRANSFER_LIMITED) \
   || ((__REG_DMA_TRANSFER__) == LL_ADC_REG_DMA_TRANSFER_UNLIMITED) \
  )

#define IS_LL_ADC_REG_FLAG_EOC_SELECTION(__REG_FLAG_EOC_SELECTION__) \
  ( ((__REG_FLAG_EOC_SELECTION__) == LL_ADC_REG_FLAG_EOC_SEQUENCE_CONV) \
   || ((__REG_FLAG_EOC_SELECTION__) == LL_ADC_REG_FLAG_EOC_UNITARY_CONV) \
  )

#define IS_LL_ADC_REG_SEQ_SCAN_LENGTH(__REG_SEQ_SCAN_LENGTH__) \
  ( ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_DISABLE) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_3RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_4RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_5RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_6RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_7RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_8RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_9RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_10RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_11RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_12RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_13RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_14RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_15RANKS) \
   || ((__REG_SEQ_SCAN_LENGTH__) == LL_ADC_REG_SEQ_SCAN_ENABLE_16RANKS) \
  )

#define IS_LL_ADC_REG_SEQ_SCAN_DISCONT_MODE(__REG_SEQ_DISCONT_MODE__) \
  ( ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_DISABLE) \
   || ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_1RANK) \
   || ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_2RANKS) \
   || ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_3RANKS) \
   || ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_4RANKS) \
   || ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_5RANKS) \
   || ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_6RANKS) \
   || ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_7RANKS) \
   || ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_8RANKS) \
  )



#define IS_LL_ADC_INJ_TRIG_SOURCE(__INJ_TRIG_SOURCE__) \
  ( ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_SOFTWARE) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM1_CH4) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM1_TRGO) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM2_CH1) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM2_TRGO) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM3_CH2) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM3_CH4) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM4_CH1) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM4_CH2) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM4_CH3) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM4_TRGO) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM5_CH4) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM5_TRGO) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM8_CH2) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM8_CH3) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_TIM8_CH4) \
   || ((__INJ_TRIG_SOURCE__) == LL_ADC_INJ_TRIG_EXT_EXTI_LINE15) \
  )

#define IS_LL_ADC_INJ_TRIG_EXT_EDGE(__INJ_TRIG_EXT_EDGE__) \
  ( ((__INJ_TRIG_EXT_EDGE__) == LL_ADC_INJ_TRIG_EXT_RISING) \
   || ((__INJ_TRIG_EXT_EDGE__) == LL_ADC_INJ_TRIG_EXT_FALLING) \
   || ((__INJ_TRIG_EXT_EDGE__) == LL_ADC_INJ_TRIG_EXT_RISINGFALLING) \
  )

#define IS_LL_ADC_INJ_TRIG_AUTO(__INJ_TRIG_AUTO__) \
  ( ((__INJ_TRIG_AUTO__) == LL_ADC_INJ_TRIG_INDEPENDENT) \
   || ((__INJ_TRIG_AUTO__) == LL_ADC_INJ_TRIG_FROM_GRP_REGULAR) \
  )

#define IS_LL_ADC_INJ_SEQ_SCAN_LENGTH(__INJ_SEQ_SCAN_LENGTH__) \
  ( ((__INJ_SEQ_SCAN_LENGTH__) == LL_ADC_INJ_SEQ_SCAN_DISABLE) \
   || ((__INJ_SEQ_SCAN_LENGTH__) == LL_ADC_INJ_SEQ_SCAN_ENABLE_2RANKS) \
   || ((__INJ_SEQ_SCAN_LENGTH__) == LL_ADC_INJ_SEQ_SCAN_ENABLE_3RANKS) \
   || ((__INJ_SEQ_SCAN_LENGTH__) == LL_ADC_INJ_SEQ_SCAN_ENABLE_4RANKS) \
  )

#define IS_LL_ADC_INJ_SEQ_SCAN_DISCONT_MODE(__INJ_SEQ_DISCONT_MODE__) \
  ( ((__INJ_SEQ_DISCONT_MODE__) == LL_ADC_INJ_SEQ_DISCONT_DISABLE) \
   || ((__INJ_SEQ_DISCONT_MODE__) == LL_ADC_INJ_SEQ_DISCONT_1RANK) \
  )

#if defined(ADC_MULTIMODE_SUPPORT)


#if defined(ADC3)
#define IS_LL_ADC_MULTI_MODE(__MULTI_MODE__) \
  ( ((__MULTI_MODE__) == LL_ADC_MULTI_INDEPENDENT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_REG_SIMULT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_REG_INTERL) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_INJ_SIMULT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_INJ_ALTERN) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_REG_SIM_INJ_SIM) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_REG_SIM_INJ_ALT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_REG_INT_INJ_SIM) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_TRIPLE_REG_SIM_INJ_SIM) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_TRIPLE_REG_SIM_INJ_ALT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_TRIPLE_INJ_SIMULT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_TRIPLE_REG_SIMULT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_TRIPLE_REG_INTERL) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_TRIPLE_INJ_ALTERN) \
  )
#else
#define IS_LL_ADC_MULTI_MODE(__MULTI_MODE__) \
  ( ((__MULTI_MODE__) == LL_ADC_MULTI_INDEPENDENT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_REG_SIMULT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_REG_INTERL) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_INJ_SIMULT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_INJ_ALTERN) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_REG_SIM_INJ_SIM) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_REG_SIM_INJ_ALT) \
   || ((__MULTI_MODE__) == LL_ADC_MULTI_DUAL_REG_INT_INJ_SIM) \
  )
#endif

#define IS_LL_ADC_MULTI_DMA_TRANSFER(__MULTI_DMA_TRANSFER__) \
  ( ((__MULTI_DMA_TRANSFER__) == LL_ADC_MULTI_REG_DMA_EACH_ADC) \
   || ((__MULTI_DMA_TRANSFER__) == LL_ADC_MULTI_REG_DMA_LIMIT_1) \
   || ((__MULTI_DMA_TRANSFER__) == LL_ADC_MULTI_REG_DMA_LIMIT_2) \
   || ((__MULTI_DMA_TRANSFER__) == LL_ADC_MULTI_REG_DMA_LIMIT_3) \
   || ((__MULTI_DMA_TRANSFER__) == LL_ADC_MULTI_REG_DMA_UNLMT_1) \
   || ((__MULTI_DMA_TRANSFER__) == LL_ADC_MULTI_REG_DMA_UNLMT_2) \
   || ((__MULTI_DMA_TRANSFER__) == LL_ADC_MULTI_REG_DMA_UNLMT_3) \
  )

#define IS_LL_ADC_MULTI_TWOSMP_DELAY(__MULTI_TWOSMP_DELAY__) \
  ( ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_5CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_6CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_7CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_8CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_9CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_10CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_11CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_12CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_13CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_14CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_15CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_16CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_17CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_18CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_19CYCLES) \
   || ((__MULTI_TWOSMP_DELAY__) == LL_ADC_MULTI_TWOSMP_DELAY_20CYCLES) \
  )

#define IS_LL_ADC_MULTI_MASTER_SLAVE(__MULTI_MASTER_SLAVE__) \
  ( ((__MULTI_MASTER_SLAVE__) == LL_ADC_MULTI_MASTER) \
   || ((__MULTI_MASTER_SLAVE__) == LL_ADC_MULTI_SLAVE) \
   || ((__MULTI_MASTER_SLAVE__) == LL_ADC_MULTI_MASTER_SLAVE) \
  )

#endif
ErrorStatus LL_ADC_CommonDeInit(ADC_Common_TypeDef *ADCxy_COMMON)
{
  assert_param(IS_ADC_COMMON_INSTANCE(ADCxy_COMMON));
  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_ADC);
  LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_ADC);
  return SUCCESS;
}

ErrorStatus LL_ADC_CommonInit(ADC_Common_TypeDef *ADCxy_COMMON, LL_ADC_CommonInitTypeDef *ADC_CommonInitStruct)
{
  ErrorStatus status = SUCCESS;
  assert_param(IS_ADC_COMMON_INSTANCE(ADCxy_COMMON));
  assert_param(IS_LL_ADC_COMMON_CLOCK(ADC_CommonInitStruct->CommonClock));

#if defined(ADC_MULTIMODE_SUPPORT)
  assert_param(IS_LL_ADC_MULTI_MODE(ADC_CommonInitStruct->Multimode));
  if(ADC_CommonInitStruct->Multimode != LL_ADC_MULTI_INDEPENDENT)
  {
    assert_param(IS_LL_ADC_MULTI_DMA_TRANSFER(ADC_CommonInitStruct->MultiDMATransfer));
    assert_param(IS_LL_ADC_MULTI_TWOSMP_DELAY(ADC_CommonInitStruct->MultiTwoSamplingDelay));
  }
#endif
  if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE(ADCxy_COMMON) == 0U)
  {
#if defined(ADC_MULTIMODE_SUPPORT)
    if(ADC_CommonInitStruct->Multimode != LL_ADC_MULTI_INDEPENDENT)
    {
      MODIFY_REG(ADCxy_COMMON->CCR,
                   ADC_CCR_ADCPRE
                 | ADC_CCR_MULTI
                 | ADC_CCR_DMA
                 | ADC_CCR_DDS
                 | ADC_CCR_DELAY
                ,
                   ADC_CommonInitStruct->CommonClock
                 | ADC_CommonInitStruct->Multimode
                 | ADC_CommonInitStruct->MultiDMATransfer
                 | ADC_CommonInitStruct->MultiTwoSamplingDelay
                );
    }
    else
    {
      MODIFY_REG(ADCxy_COMMON->CCR,
                   ADC_CCR_ADCPRE
                 | ADC_CCR_MULTI
                 | ADC_CCR_DMA
                 | ADC_CCR_DDS
                 | ADC_CCR_DELAY
                ,
                   ADC_CommonInitStruct->CommonClock
                 | LL_ADC_MULTI_INDEPENDENT
                );
    }
#else
    LL_ADC_SetCommonClock(ADCxy_COMMON, ADC_CommonInitStruct->CommonClock);
#endif
  }
  else
  {


    status = ERROR;
  }

  return status;
}

void LL_ADC_CommonStructInit(LL_ADC_CommonInitTypeDef *ADC_CommonInitStruct)
{
  ADC_CommonInitStruct->CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;

#if defined(ADC_MULTIMODE_SUPPORT)
  ADC_CommonInitStruct->Multimode = LL_ADC_MULTI_INDEPENDENT;
    ADC_CommonInitStruct->MultiDMATransfer = LL_ADC_MULTI_REG_DMA_EACH_ADC;
  ADC_CommonInitStruct->MultiTwoSamplingDelay = LL_ADC_MULTI_TWOSMP_DELAY_5CYCLES;
#endif
}

ErrorStatus LL_ADC_DeInit(ADC_TypeDef *ADCx)
{
  ErrorStatus status = SUCCESS;
  assert_param(IS_ADC_ALL_INSTANCE(ADCx));
  if(LL_ADC_IsEnabled(ADCx) == 1U)
  {
    LL_ADC_REG_SetTriggerSource(ADCx, LL_ADC_REG_TRIG_SOFTWARE);
    LL_ADC_INJ_SetTriggerSource(ADCx, LL_ADC_INJ_TRIG_SOFTWARE);
    LL_ADC_Disable(ADCx);
  }

  if(READ_BIT(ADCx->CR2, ADC_CR2_ADON) == 0U)
  {
    CLEAR_BIT(ADCx->SR,
              ( LL_ADC_FLAG_STRT
               | LL_ADC_FLAG_JSTRT
               | LL_ADC_FLAG_EOCS
               | LL_ADC_FLAG_OVR
               | LL_ADC_FLAG_JEOS
               | LL_ADC_FLAG_AWD1 )
             );


    CLEAR_BIT(ADCx->CR1,
              ( ADC_CR1_OVRIE | ADC_CR1_RES | ADC_CR1_AWDEN
               | ADC_CR1_JAWDEN
               | ADC_CR1_DISCNUM | ADC_CR1_JDISCEN | ADC_CR1_DISCEN
               | ADC_CR1_JAUTO | ADC_CR1_AWDSGL | ADC_CR1_SCAN
               | ADC_CR1_JEOCIE | ADC_CR1_AWDIE | ADC_CR1_EOCIE
               | ADC_CR1_AWDCH )
             );


    CLEAR_BIT(ADCx->CR2,
              ( ADC_CR2_SWSTART | ADC_CR2_EXTEN | ADC_CR2_EXTSEL
               | ADC_CR2_JSWSTART | ADC_CR2_JEXTEN | ADC_CR2_JEXTSEL
               | ADC_CR2_ALIGN | ADC_CR2_EOCS
               | ADC_CR2_DDS | ADC_CR2_DMA
               | ADC_CR2_CONT | ADC_CR2_ADON )
             );


    CLEAR_BIT(ADCx->SMPR1,
              ( ADC_SMPR1_SMP18 | ADC_SMPR1_SMP17 | ADC_SMPR1_SMP16
               | ADC_SMPR1_SMP15 | ADC_SMPR1_SMP14 | ADC_SMPR1_SMP13
               | ADC_SMPR1_SMP12 | ADC_SMPR1_SMP11 | ADC_SMPR1_SMP10)
             );


    CLEAR_BIT(ADCx->SMPR2,
              ( ADC_SMPR2_SMP9
               | ADC_SMPR2_SMP8 | ADC_SMPR2_SMP7 | ADC_SMPR2_SMP6
               | ADC_SMPR2_SMP5 | ADC_SMPR2_SMP4 | ADC_SMPR2_SMP3
               | ADC_SMPR2_SMP2 | ADC_SMPR2_SMP1 | ADC_SMPR2_SMP0)
             );


    CLEAR_BIT(ADCx->JOFR1, ADC_JOFR1_JOFFSET1);

    CLEAR_BIT(ADCx->JOFR2, ADC_JOFR2_JOFFSET2);

    CLEAR_BIT(ADCx->JOFR3, ADC_JOFR3_JOFFSET3);

    CLEAR_BIT(ADCx->JOFR4, ADC_JOFR4_JOFFSET4);


    SET_BIT(ADCx->HTR, ADC_HTR_HT);

    CLEAR_BIT(ADCx->LTR, ADC_LTR_LT);


    CLEAR_BIT(ADCx->SQR1,
              ( ADC_SQR1_L
               | ADC_SQR1_SQ16
               | ADC_SQR1_SQ15 | ADC_SQR1_SQ14 | ADC_SQR1_SQ13)
             );


    CLEAR_BIT(ADCx->SQR2,
              ( ADC_SQR2_SQ12 | ADC_SQR2_SQ11 | ADC_SQR2_SQ10
               | ADC_SQR2_SQ9 | ADC_SQR2_SQ8 | ADC_SQR2_SQ7)
             );



    CLEAR_BIT(ADCx->JSQR,
              ( ADC_JSQR_JL
               | ADC_JSQR_JSQ4 | ADC_JSQR_JSQ3
               | ADC_JSQR_JSQ2 | ADC_JSQR_JSQ1 )
             );
    CLEAR_BIT(ADC->CCR, ADC_CCR_TSVREFE | ADC_CCR_ADCPRE);
  }

  return status;
}

ErrorStatus LL_ADC_Init(ADC_TypeDef *ADCx, LL_ADC_InitTypeDef *ADC_InitStruct)
{
  ErrorStatus status = SUCCESS;
  assert_param(IS_ADC_ALL_INSTANCE(ADCx));
  assert_param(IS_LL_ADC_RESOLUTION(ADC_InitStruct->Resolution));
  assert_param(IS_LL_ADC_DATA_ALIGN(ADC_InitStruct->DataAlignment));
  assert_param(IS_LL_ADC_SCAN_SELECTION(ADC_InitStruct->SequencersScanMode));

  if(LL_ADC_IsEnabled(ADCx) == 0U)
  {
    MODIFY_REG(ADCx->CR1,
                 ADC_CR1_RES
               | ADC_CR1_SCAN
              ,
                 ADC_InitStruct->Resolution
               | ADC_InitStruct->SequencersScanMode
              );

    MODIFY_REG(ADCx->CR2,
                 ADC_CR2_ALIGN
              ,
                 ADC_InitStruct->DataAlignment
              );

  }
  else
  {
    status = ERROR;
  }
  return status;
}

void LL_ADC_StructInit(LL_ADC_InitTypeDef *ADC_InitStruct)
{
  ADC_InitStruct->Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct->DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct->SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;

}

ErrorStatus LL_ADC_REG_Init(ADC_TypeDef *ADCx, LL_ADC_REG_InitTypeDef *ADC_REG_InitStruct)
{
  ErrorStatus status = SUCCESS;
  assert_param(IS_ADC_ALL_INSTANCE(ADCx));
  assert_param(IS_LL_ADC_REG_TRIG_SOURCE(ADC_REG_InitStruct->TriggerSource));
  assert_param(IS_LL_ADC_REG_SEQ_SCAN_LENGTH(ADC_REG_InitStruct->SequencerLength));
  if(ADC_REG_InitStruct->SequencerLength != LL_ADC_REG_SEQ_SCAN_DISABLE)
  {
    assert_param(IS_LL_ADC_REG_SEQ_SCAN_DISCONT_MODE(ADC_REG_InitStruct->SequencerDiscont));
  }
  assert_param(IS_LL_ADC_REG_CONTINUOUS_MODE(ADC_REG_InitStruct->ContinuousMode));
  assert_param(IS_LL_ADC_REG_DMA_TRANSFER(ADC_REG_InitStruct->DMATransfer));

  if(LL_ADC_IsEnabled(ADCx) == 0U)
  {
    if(ADC_REG_InitStruct->SequencerLength != LL_ADC_REG_SEQ_SCAN_DISABLE)
    {
      MODIFY_REG(ADCx->CR1,
                   ADC_CR1_DISCEN
                 | ADC_CR1_DISCNUM
                ,
                   ADC_REG_InitStruct->SequencerLength
                 | ADC_REG_InitStruct->SequencerDiscont
                );
    }
    else
    {
      MODIFY_REG(ADCx->CR1,
                   ADC_CR1_DISCEN
                 | ADC_CR1_DISCNUM
                ,
                   ADC_REG_InitStruct->SequencerLength
                 | LL_ADC_REG_SEQ_DISCONT_DISABLE
                );
    }

    MODIFY_REG(ADCx->CR2,
                 ADC_CR2_EXTSEL
               | ADC_CR2_EXTEN
               | ADC_CR2_CONT
               | ADC_CR2_DMA
               | ADC_CR2_DDS
              ,
                (ADC_REG_InitStruct->TriggerSource & ADC_CR2_EXTSEL)
               | ADC_REG_InitStruct->ContinuousMode
               | ADC_REG_InitStruct->DMATransfer
              );
    LL_ADC_REG_SetSequencerLength(ADCx, ADC_REG_InitStruct->SequencerLength);
  }
  else
  {
    status = ERROR;
  }
  return status;
}

void LL_ADC_REG_StructInit(LL_ADC_REG_InitTypeDef *ADC_REG_InitStruct)
{
  ADC_REG_InitStruct->TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct->SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  ADC_REG_InitStruct->SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct->ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct->DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
}

ErrorStatus LL_ADC_INJ_Init(ADC_TypeDef *ADCx, LL_ADC_INJ_InitTypeDef *ADC_INJ_InitStruct)
{
  ErrorStatus status = SUCCESS;
  assert_param(IS_ADC_ALL_INSTANCE(ADCx));
  assert_param(IS_LL_ADC_INJ_TRIG_SOURCE(ADC_INJ_InitStruct->TriggerSource));
  assert_param(IS_LL_ADC_INJ_SEQ_SCAN_LENGTH(ADC_INJ_InitStruct->SequencerLength));
  if(ADC_INJ_InitStruct->SequencerLength != LL_ADC_INJ_SEQ_SCAN_DISABLE)
  {
    assert_param(IS_LL_ADC_INJ_SEQ_SCAN_DISCONT_MODE(ADC_INJ_InitStruct->SequencerDiscont));
  }
  assert_param(IS_LL_ADC_INJ_TRIG_AUTO(ADC_INJ_InitStruct->TrigAuto));

  if(LL_ADC_IsEnabled(ADCx) == 0U)
  {
    if(ADC_INJ_InitStruct->SequencerLength != LL_ADC_REG_SEQ_SCAN_DISABLE)
    {
      MODIFY_REG(ADCx->CR1,
                   ADC_CR1_JDISCEN
                 | ADC_CR1_JAUTO
                ,
                   ADC_INJ_InitStruct->SequencerDiscont
                 | ADC_INJ_InitStruct->TrigAuto
                );
    }
    else
    {
      MODIFY_REG(ADCx->CR1,
                   ADC_CR1_JDISCEN
                 | ADC_CR1_JAUTO
                ,
                   LL_ADC_REG_SEQ_DISCONT_DISABLE
                 | ADC_INJ_InitStruct->TrigAuto
                );
    }

    MODIFY_REG(ADCx->CR2,
                 ADC_CR2_JEXTSEL
               | ADC_CR2_JEXTEN
              ,
                (ADC_INJ_InitStruct->TriggerSource & ADC_CR2_JEXTSEL)
              );
    LL_ADC_INJ_SetSequencerLength(ADCx, ADC_INJ_InitStruct->SequencerLength);
  }
  else
  {

    status = ERROR;
  }
  return status;
}

void LL_ADC_INJ_StructInit(LL_ADC_INJ_InitTypeDef *ADC_INJ_InitStruct)
{
  ADC_INJ_InitStruct->TriggerSource = LL_ADC_INJ_TRIG_SOFTWARE;
  ADC_INJ_InitStruct->SequencerLength = LL_ADC_INJ_SEQ_SCAN_DISABLE;
  ADC_INJ_InitStruct->SequencerDiscont = LL_ADC_INJ_SEQ_DISCONT_DISABLE;
  ADC_INJ_InitStruct->TrigAuto = LL_ADC_INJ_TRIG_INDEPENDENT;
}
#endif

#endif
