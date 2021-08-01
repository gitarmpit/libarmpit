#include "stm32f4xx_hal.h"
#ifdef HAL_ADC_MODULE_ENABLED

static void ADC_Init(ADC_HandleTypeDef* hadc);
static void ADC_DMAConvCplt(DMA_HandleTypeDef *hdma);
static void ADC_DMAError(DMA_HandleTypeDef *hdma);
static void ADC_DMAHalfConvCplt(DMA_HandleTypeDef *hdma);

HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;

  if(hadc == NULL)
  {
    return HAL_ERROR;
  }

  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CLOCKPRESCALER(hadc->Init.ClockPrescaler));
  assert_param(IS_ADC_RESOLUTION(hadc->Init.Resolution));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ScanConvMode));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_ADC_EXT_TRIG(hadc->Init.ExternalTrigConv));
  assert_param(IS_ADC_DATA_ALIGN(hadc->Init.DataAlign));
  assert_param(IS_ADC_REGULAR_LENGTH(hadc->Init.NbrOfConversion));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.DMAContinuousRequests));
  assert_param(IS_ADC_EOCSelection(hadc->Init.EOCSelection));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.DiscontinuousConvMode));

  if(hadc->Init.ExternalTrigConv != ADC_SOFTWARE_START)
  {
    assert_param(IS_ADC_EXT_TRIG_EDGE(hadc->Init.ExternalTrigConvEdge));
  }

  if(hadc->State == HAL_ADC_STATE_RESET)
  {
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)

    hadc->ConvCpltCallback = HAL_ADC_ConvCpltCallback;
    hadc->ConvHalfCpltCallback = HAL_ADC_ConvHalfCpltCallback;
    hadc->LevelOutOfWindowCallback = HAL_ADC_LevelOutOfWindowCallback;
    hadc->ErrorCallback = HAL_ADC_ErrorCallback;
    hadc->InjectedConvCpltCallback = HAL_ADCEx_InjectedConvCpltCallback;
    if (hadc->MspInitCallback == NULL)
    {
      hadc->MspInitCallback = HAL_ADC_MspInit;
    }

    hadc->MspInitCallback(hadc);
#else

    HAL_ADC_MspInit(hadc);
#endif


    ADC_CLEAR_ERRORCODE(hadc);


    hadc->Lock = HAL_UNLOCKED;
  }

  if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL))
  {

    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                      HAL_ADC_STATE_BUSY_INTERNAL);

    ADC_Init(hadc);
    ADC_CLEAR_ERRORCODE(hadc);
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_BUSY_INTERNAL,
                      HAL_ADC_STATE_READY);
  }
  else
  {
    tmp_hal_status = HAL_ERROR;
  }

  __HAL_UNLOCK(hadc);
  return tmp_hal_status;
}
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;

  if(hadc == NULL)
  {
    return HAL_ERROR;
  }

  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  SET_BIT(hadc->State, HAL_ADC_STATE_BUSY_INTERNAL);
  __HAL_ADC_DISABLE(hadc);
  if(HAL_IS_BIT_CLR(hadc->Instance->CR2, ADC_CR2_ADON))
  {
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
  if (hadc->MspDeInitCallback == NULL)
  {
    hadc->MspDeInitCallback = HAL_ADC_MspDeInit;
  }
  hadc->MspDeInitCallback(hadc);
#else

  HAL_ADC_MspDeInit(hadc);
#endif
    ADC_CLEAR_ERRORCODE(hadc);
    hadc->State = HAL_ADC_STATE_RESET;
  }


  __HAL_UNLOCK(hadc);
  return tmp_hal_status;
}

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
HAL_StatusTypeDef HAL_ADC_RegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID, pADC_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {

    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }

  if ((hadc->State & HAL_ADC_STATE_READY) != 0UL)
  {
    switch (CallbackID)
    {
      case HAL_ADC_CONVERSION_COMPLETE_CB_ID :
        hadc->ConvCpltCallback = pCallback;
        break;

      case HAL_ADC_CONVERSION_HALF_CB_ID :
        hadc->ConvHalfCpltCallback = pCallback;
        break;

      case HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID :
        hadc->LevelOutOfWindowCallback = pCallback;
        break;

      case HAL_ADC_ERROR_CB_ID :
        hadc->ErrorCallback = pCallback;
        break;

      case HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID :
        hadc->InjectedConvCpltCallback = pCallback;
        break;

      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = pCallback;
        break;

      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = pCallback;
        break;

      default :

        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;


        status = HAL_ERROR;
        break;
    }
  }
  else if (HAL_ADC_STATE_RESET == hadc->State)
  {
    switch (CallbackID)
    {
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = pCallback;
        break;

      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = pCallback;
        break;

      default :

        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;


        status = HAL_ERROR;
        break;
    }
  }
  else
  {

    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;


    status = HAL_ERROR;
  }

  return status;
}

HAL_StatusTypeDef HAL_ADC_UnRegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  if ((hadc->State & HAL_ADC_STATE_READY) != 0UL)
  {
    switch (CallbackID)
    {
      case HAL_ADC_CONVERSION_COMPLETE_CB_ID :
        hadc->ConvCpltCallback = HAL_ADC_ConvCpltCallback;
        break;

      case HAL_ADC_CONVERSION_HALF_CB_ID :
        hadc->ConvHalfCpltCallback = HAL_ADC_ConvHalfCpltCallback;
        break;

      case HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID :
        hadc->LevelOutOfWindowCallback = HAL_ADC_LevelOutOfWindowCallback;
        break;

      case HAL_ADC_ERROR_CB_ID :
        hadc->ErrorCallback = HAL_ADC_ErrorCallback;
        break;

      case HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID :
        hadc->InjectedConvCpltCallback = HAL_ADCEx_InjectedConvCpltCallback;
        break;

      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = HAL_ADC_MspInit;
        break;

      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = HAL_ADC_MspDeInit;
        break;

      default :

        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;


        status = HAL_ERROR;
        break;
    }
  }
  else if (HAL_ADC_STATE_RESET == hadc->State)
  {
    switch (CallbackID)
    {
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = HAL_ADC_MspInit;
        break;

      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = HAL_ADC_MspDeInit;
        break;

      default :

        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;


        status = HAL_ERROR;
        break;
    }
  }
  else
  {

    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;


    status = HAL_ERROR;
  }

  return status;
}

#endif

__weak void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  UNUSED(hadc);
}

__weak void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  UNUSED(hadc);
}

HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc)
{
  __IO uint32_t counter = 0U;
  ADC_Common_TypeDef *tmpADC_Common;


  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_ADC_EXT_TRIG_EDGE(hadc->Init.ExternalTrigConvEdge));


  __HAL_LOCK(hadc);

  if((hadc->Instance->CR2 & ADC_CR2_ADON) != ADC_CR2_ADON)
  {

    __HAL_ADC_ENABLE(hadc);

    counter = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000U));
    while(counter != 0U)
    {
      counter--;
    }
  }

  if(HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_ADON))
  {
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR,
                      HAL_ADC_STATE_REG_BUSY);

    if (READ_BIT(hadc->Instance->CR1, ADC_CR1_JAUTO) != RESET)
    {
      ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
    }

    if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
    {

      CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));
    }
    else
    {

      ADC_CLEAR_ERRORCODE(hadc);
    }

    __HAL_UNLOCK(hadc);
    tmpADC_Common = ADC_COMMON_REGISTER(hadc);
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC | ADC_FLAG_OVR);

    if(HAL_IS_BIT_CLR(tmpADC_Common->CCR, ADC_CCR_MULTI))
    {
#if defined(ADC2) && defined(ADC3)
      if((hadc->Instance == ADC1) || ((hadc->Instance == ADC2) && ((ADC->CCR & ADC_CCR_MULTI_Msk) < ADC_CCR_MULTI_0)) \
                                  || ((hadc->Instance == ADC3) && ((ADC->CCR & ADC_CCR_MULTI_Msk) < ADC_CCR_MULTI_4)))
      {
#endif
        if((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET)
        {
          hadc->Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
        }
#if defined(ADC2) && defined(ADC3)
      }
#endif
    }
    else
    {
      if((hadc->Instance == ADC1) && ((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET))
      {
          hadc->Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
      }
    }
  }


  return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc)
{
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  __HAL_LOCK(hadc);
  __HAL_ADC_DISABLE(hadc);

  if(HAL_IS_BIT_CLR(hadc->Instance->CR2, ADC_CR2_ADON))
  {
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                      HAL_ADC_STATE_READY);
  }

  __HAL_UNLOCK(hadc);
  return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout)
{
  uint32_t tickstart = 0U;
  if (HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_EOCS) &&
      HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_DMA) )
  {

    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    __HAL_UNLOCK(hadc);
    return HAL_ERROR;
  }

  tickstart = HAL_GetTick();

  while(!(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC)))
  {
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((HAL_GetTick() - tickstart ) > Timeout))
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);
        __HAL_UNLOCK(hadc);
        return HAL_TIMEOUT;
      }
    }
  }

  __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_STRT | ADC_FLAG_EOC);
  SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);

  if(ADC_IS_SOFTWARE_START_REGULAR(hadc) &&
     (hadc->Init.ContinuousConvMode == DISABLE) &&
     (HAL_IS_BIT_CLR(hadc->Instance->SQR1, ADC_SQR1_L) ||
      HAL_IS_BIT_CLR(hadc->Instance->CR2, ADC_CR2_EOCS) ) )
  {

    CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);

    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
    {
      SET_BIT(hadc->State, HAL_ADC_STATE_READY);
    }
  }


  return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_PollForEvent(ADC_HandleTypeDef* hadc, uint32_t EventType, uint32_t Timeout)
{
  uint32_t tickstart = 0U;

  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_EVENT_TYPE(EventType));

  tickstart = HAL_GetTick();

  while(!(__HAL_ADC_GET_FLAG(hadc,EventType)))
  {
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((HAL_GetTick() - tickstart ) > Timeout))
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);
        __HAL_UNLOCK(hadc);
        return HAL_TIMEOUT;
      }
    }
  }

  if(EventType == ADC_AWD_EVENT)
  {
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD1);
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD);
  }
  else
  {
    SET_BIT(hadc->State, HAL_ADC_STATE_REG_OVR);
    SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_OVR);
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
  }

  return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc)
{
  __IO uint32_t counter = 0U;
  ADC_Common_TypeDef *tmpADC_Common;

  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_ADC_EXT_TRIG_EDGE(hadc->Init.ExternalTrigConvEdge));
  __HAL_LOCK(hadc);

  if((hadc->Instance->CR2 & ADC_CR2_ADON) != ADC_CR2_ADON)
  {
    __HAL_ADC_ENABLE(hadc);
    counter = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000U));
    while(counter != 0U)
    {
      counter--;
    }
  }

  if(HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_ADON))
  {
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR,
                      HAL_ADC_STATE_REG_BUSY);

    if (READ_BIT(hadc->Instance->CR1, ADC_CR1_JAUTO) != RESET)
    {
      ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
    }

    if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
    {
      CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));
    }
    else
    {
      ADC_CLEAR_ERRORCODE(hadc);
    }

    __HAL_UNLOCK(hadc);

    tmpADC_Common = ADC_COMMON_REGISTER(hadc);

    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC | ADC_FLAG_OVR);
    __HAL_ADC_ENABLE_IT(hadc, (ADC_IT_EOC | ADC_IT_OVR));
 
     if(HAL_IS_BIT_CLR(tmpADC_Common->CCR, ADC_CCR_MULTI))
    {
#if defined(ADC2) && defined(ADC3)
      if((hadc->Instance == ADC1) || ((hadc->Instance == ADC2) && ((ADC->CCR & ADC_CCR_MULTI_Msk) < ADC_CCR_MULTI_0)) \
                                  || ((hadc->Instance == ADC3) && ((ADC->CCR & ADC_CCR_MULTI_Msk) < ADC_CCR_MULTI_4)))
      {
#endif
        if((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET)
        {
          hadc->Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
        }
#if defined(ADC2) && defined(ADC3)
      }
#endif
    }
    else
    {

      if((hadc->Instance == ADC1) && ((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET))
      {

          hadc->Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
      }
    }
  }


  return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_Stop_IT(ADC_HandleTypeDef* hadc)
{
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  __HAL_LOCK(hadc);
  __HAL_ADC_DISABLE(hadc);

  if(HAL_IS_BIT_CLR(hadc->Instance->CR2, ADC_CR2_ADON))
  {
    __HAL_ADC_DISABLE_IT(hadc, (ADC_IT_EOC | ADC_IT_OVR));
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                      HAL_ADC_STATE_READY);
  }

  __HAL_UNLOCK(hadc);
  return HAL_OK;
}

void HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc)
{
  uint32_t tmp1 = 0U, tmp2 = 0U;
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_ADC_REGULAR_LENGTH(hadc->Init.NbrOfConversion));
  assert_param(IS_ADC_EOCSelection(hadc->Init.EOCSelection));

  tmp1 = __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC);
  tmp2 = __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_EOC);

  if(tmp1 && tmp2)
  {
    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL))
    {
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
    }
    if(ADC_IS_SOFTWARE_START_REGULAR(hadc) &&
       (hadc->Init.ContinuousConvMode == DISABLE) &&
       (HAL_IS_BIT_CLR(hadc->Instance->SQR1, ADC_SQR1_L) ||
        HAL_IS_BIT_CLR(hadc->Instance->CR2, ADC_CR2_EOCS) ) )
    {
      __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC);
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);

      if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_READY);
      }
    }

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->ConvCpltCallback(hadc);
#else
    HAL_ADC_ConvCpltCallback(hadc);
#endif
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_STRT | ADC_FLAG_EOC);
  }

  tmp1 = __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_JEOC);
  tmp2 = __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_JEOC);

  if(tmp1 && tmp2)
  {
    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL))
    {
      SET_BIT(hadc->State, HAL_ADC_STATE_INJ_EOC);
    }

    if(ADC_IS_SOFTWARE_START_INJECTED(hadc) &&
       (HAL_IS_BIT_CLR(hadc->Instance->JSQR, ADC_JSQR_JL) ||
        HAL_IS_BIT_CLR(hadc->Instance->CR2, ADC_CR2_EOCS) ) &&
       (HAL_IS_BIT_CLR(hadc->Instance->CR1, ADC_CR1_JAUTO) &&
        (ADC_IS_SOFTWARE_START_REGULAR(hadc) &&
        (hadc->Init.ContinuousConvMode == DISABLE) ) ) )
    {
      __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOC);
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_INJ_BUSY);

      if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_REG_BUSY))
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_READY);
      }
    }

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->InjectedConvCpltCallback(hadc);
#else
      HAL_ADCEx_InjectedConvCpltCallback(hadc);
#endif
    __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_JSTRT | ADC_FLAG_JEOC));
  }

  tmp1 = __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_AWD);
  tmp2 = __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_AWD);

  if(tmp1 && tmp2)
  {
    if(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_AWD))
    {
      SET_BIT(hadc->State, HAL_ADC_STATE_AWD1);


#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->LevelOutOfWindowCallback(hadc);
#else
      HAL_ADC_LevelOutOfWindowCallback(hadc);
#endif
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD);
    }
  }

  tmp1 = __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_OVR);
  tmp2 = __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_OVR);

  if(tmp1 && tmp2)
  {
    SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_OVR);

    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->ErrorCallback(hadc);
#else
      HAL_ADC_ErrorCallback(hadc);
#endif

    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
  }
}

HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length)
{
  __IO uint32_t counter = 0U;
  ADC_Common_TypeDef *tmpADC_Common;

  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_ADC_EXT_TRIG_EDGE(hadc->Init.ExternalTrigConvEdge));

  __HAL_LOCK(hadc);

  if((hadc->Instance->CR2 & ADC_CR2_ADON) != ADC_CR2_ADON)
  {
    __HAL_ADC_ENABLE(hadc);

    counter = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000U));
    while(counter != 0U)
    {
      counter--;
    }
  }


  if(HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_ADON))
  {
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR,
                      HAL_ADC_STATE_REG_BUSY);

    if (READ_BIT(hadc->Instance->CR1, ADC_CR1_JAUTO) != RESET)
    {
      ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
    }

    if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
    {

      CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));
    }
    else
    {
      ADC_CLEAR_ERRORCODE(hadc);
    }

    __HAL_UNLOCK(hadc);


    tmpADC_Common = ADC_COMMON_REGISTER(hadc);
    hadc->DMA_Handle->XferCpltCallback = ADC_DMAConvCplt;
    hadc->DMA_Handle->XferHalfCpltCallback = ADC_DMAHalfConvCplt;
    hadc->DMA_Handle->XferErrorCallback = ADC_DMAError;

    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC | ADC_FLAG_OVR);
    __HAL_ADC_ENABLE_IT(hadc, ADC_IT_OVR);

    hadc->Instance->CR2 |= ADC_CR2_DMA;
    HAL_DMA_Start_IT(hadc->DMA_Handle, (uint32_t)&hadc->Instance->DR, (uint32_t)pData, Length);
    if(HAL_IS_BIT_CLR(tmpADC_Common->CCR, ADC_CCR_MULTI))
    {
#if defined(ADC2) && defined(ADC3)
      if((hadc->Instance == ADC1) || ((hadc->Instance == ADC2) && ((ADC->CCR & ADC_CCR_MULTI_Msk) < ADC_CCR_MULTI_0)) \
                                  || ((hadc->Instance == ADC3) && ((ADC->CCR & ADC_CCR_MULTI_Msk) < ADC_CCR_MULTI_4)))
      {
#endif
        if((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET)
        {
          hadc->Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
        }
#if defined(ADC2) && defined(ADC3)
      }
#endif
    }
    else
    {
      if((hadc->Instance == ADC1) && ((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET))
      {
          hadc->Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
      }
    }
  }
  return HAL_OK;
}
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  __HAL_LOCK(hadc);
  __HAL_ADC_DISABLE(hadc);
  if(HAL_IS_BIT_CLR(hadc->Instance->CR2, ADC_CR2_ADON))
  {
    hadc->Instance->CR2 &= ~ADC_CR2_DMA;
    tmp_hal_status = HAL_DMA_Abort(hadc->DMA_Handle);
    __HAL_ADC_DISABLE_IT(hadc, ADC_IT_OVR);
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                      HAL_ADC_STATE_READY);
  }

  __HAL_UNLOCK(hadc);

  return tmp_hal_status;
}

uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc)
{
  return hadc->Instance->DR;
}

__weak void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  UNUSED(hadc);
}

__weak void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
  UNUSED(hadc);

}

__weak void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
  UNUSED(hadc);
}

__weak void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  UNUSED(hadc);
}

HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig)
{
  __IO uint32_t counter = 0U;
  ADC_Common_TypeDef *tmpADC_Common;

  assert_param(IS_ADC_CHANNEL(sConfig->Channel));
  assert_param(IS_ADC_REGULAR_RANK(sConfig->Rank));
  assert_param(IS_ADC_SAMPLE_TIME(sConfig->SamplingTime));

  __HAL_LOCK(hadc);

  if (sConfig->Channel > ADC_CHANNEL_9)
  {
    hadc->Instance->SMPR1 &= ~ADC_SMPR1(ADC_SMPR1_SMP10, sConfig->Channel);
    hadc->Instance->SMPR1 |= ADC_SMPR1(sConfig->SamplingTime, sConfig->Channel);
  }
  else
  {
    hadc->Instance->SMPR2 &= ~ADC_SMPR2(ADC_SMPR2_SMP0, sConfig->Channel);
    hadc->Instance->SMPR2 |= ADC_SMPR2(sConfig->SamplingTime, sConfig->Channel);
  }

  if (sConfig->Rank < 7U)
  {
    hadc->Instance->SQR3 &= ~ADC_SQR3_RK(ADC_SQR3_SQ1, sConfig->Rank);
    hadc->Instance->SQR3 |= ADC_SQR3_RK(sConfig->Channel, sConfig->Rank);
  }

  else if (sConfig->Rank < 13U)
  {
    hadc->Instance->SQR2 &= ~ADC_SQR2_RK(ADC_SQR2_SQ7, sConfig->Rank);
    hadc->Instance->SQR2 |= ADC_SQR2_RK(sConfig->Channel, sConfig->Rank);
  }

  else
  {
    hadc->Instance->SQR1 &= ~ADC_SQR1_RK(ADC_SQR1_SQ13, sConfig->Rank);
    hadc->Instance->SQR1 |= ADC_SQR1_RK(sConfig->Channel, sConfig->Rank);
  }
  
  tmpADC_Common = ADC_COMMON_REGISTER(hadc);
  if ((hadc->Instance == ADC1) && (sConfig->Channel == ADC_CHANNEL_VBAT))
  {
    if ((uint16_t)ADC_CHANNEL_TEMPSENSOR == (uint16_t)ADC_CHANNEL_VBAT)
    {
      tmpADC_Common->CCR &= ~ADC_CCR_TSVREFE;
    }

    tmpADC_Common->CCR |= ADC_CCR_VBATE;
  }

  if ((hadc->Instance == ADC1) && ((sConfig->Channel == ADC_CHANNEL_TEMPSENSOR) || (sConfig->Channel == ADC_CHANNEL_VREFINT)))
  {
    if ((uint16_t)ADC_CHANNEL_TEMPSENSOR == (uint16_t)ADC_CHANNEL_VBAT)
    {
      tmpADC_Common->CCR &= ~ADC_CCR_VBATE;
    }

    tmpADC_Common->CCR |= ADC_CCR_TSVREFE;

    if((sConfig->Channel == ADC_CHANNEL_TEMPSENSOR))
    {


      counter = (ADC_TEMPSENSOR_DELAY_US * (SystemCoreClock / 1000000U));
      while(counter != 0U)
      {
        counter--;
      }
    }
  }


  __HAL_UNLOCK(hadc);


  return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef* hadc, ADC_AnalogWDGConfTypeDef* AnalogWDGConfig)
{
#ifdef USE_FULL_ASSERT
  uint32_t tmp = 0U;
#endif


  assert_param(IS_ADC_ANALOG_WATCHDOG(AnalogWDGConfig->WatchdogMode));
  assert_param(IS_ADC_CHANNEL(AnalogWDGConfig->Channel));
  assert_param(IS_FUNCTIONAL_STATE(AnalogWDGConfig->ITMode));

#ifdef USE_FULL_ASSERT
  tmp = ADC_GET_RESOLUTION(hadc);
  assert_param(IS_ADC_RANGE(tmp, AnalogWDGConfig->HighThreshold));
  assert_param(IS_ADC_RANGE(tmp, AnalogWDGConfig->LowThreshold));
#endif


  __HAL_LOCK(hadc);

  if(AnalogWDGConfig->ITMode == ENABLE)
  {

    __HAL_ADC_ENABLE_IT(hadc, ADC_IT_AWD);
  }
  else
  {

    __HAL_ADC_DISABLE_IT(hadc, ADC_IT_AWD);
  }


  hadc->Instance->CR1 &= ~(ADC_CR1_AWDSGL | ADC_CR1_JAWDEN | ADC_CR1_AWDEN);


  hadc->Instance->CR1 |= AnalogWDGConfig->WatchdogMode;


  hadc->Instance->HTR = AnalogWDGConfig->HighThreshold;


  hadc->Instance->LTR = AnalogWDGConfig->LowThreshold;


  hadc->Instance->CR1 &= ~ADC_CR1_AWDCH;


  hadc->Instance->CR1 |= (uint32_t)((uint16_t)(AnalogWDGConfig->Channel));


  __HAL_UNLOCK(hadc);


  return HAL_OK;
}
uint32_t HAL_ADC_GetState(ADC_HandleTypeDef* hadc)
{

  return hadc->State;
}


uint32_t HAL_ADC_GetError(ADC_HandleTypeDef *hadc)
{
  return hadc->ErrorCode;
}

static void ADC_Init(ADC_HandleTypeDef* hadc)
{
  ADC_Common_TypeDef *tmpADC_Common;

  tmpADC_Common = ADC_COMMON_REGISTER(hadc);

  tmpADC_Common->CCR &= ~(ADC_CCR_ADCPRE);
  tmpADC_Common->CCR |= hadc->Init.ClockPrescaler;

  hadc->Instance->CR1 &= ~(ADC_CR1_SCAN);
  hadc->Instance->CR1 |= ADC_CR1_SCANCONV(hadc->Init.ScanConvMode);

  hadc->Instance->CR1 &= ~(ADC_CR1_RES);
  hadc->Instance->CR1 |= hadc->Init.Resolution;

  hadc->Instance->CR2 &= ~(ADC_CR2_ALIGN);
  hadc->Instance->CR2 |= hadc->Init.DataAlign;

  if(hadc->Init.ExternalTrigConv != ADC_SOFTWARE_START)
  {

    hadc->Instance->CR2 &= ~(ADC_CR2_EXTSEL);
    hadc->Instance->CR2 |= hadc->Init.ExternalTrigConv;


    hadc->Instance->CR2 &= ~(ADC_CR2_EXTEN);
    hadc->Instance->CR2 |= hadc->Init.ExternalTrigConvEdge;
  }
  else
  {

    hadc->Instance->CR2 &= ~(ADC_CR2_EXTSEL);
    hadc->Instance->CR2 &= ~(ADC_CR2_EXTEN);
  }


  hadc->Instance->CR2 &= ~(ADC_CR2_CONT);
  hadc->Instance->CR2 |= ADC_CR2_CONTINUOUS((uint32_t)hadc->Init.ContinuousConvMode);

  if(hadc->Init.DiscontinuousConvMode != DISABLE)
  {
    assert_param(IS_ADC_REGULAR_DISC_NUMBER(hadc->Init.NbrOfDiscConversion));
    hadc->Instance->CR1 |= (uint32_t)ADC_CR1_DISCEN;
    hadc->Instance->CR1 &= ~(ADC_CR1_DISCNUM);
    hadc->Instance->CR1 |= ADC_CR1_DISCONTINUOUS(hadc->Init.NbrOfDiscConversion);
  }
  else
  {
    hadc->Instance->CR1 &= ~(ADC_CR1_DISCEN);
  }

  hadc->Instance->SQR1 &= ~(ADC_SQR1_L);
  hadc->Instance->SQR1 |= ADC_SQR1(hadc->Init.NbrOfConversion);
  hadc->Instance->CR2 &= ~(ADC_CR2_DDS);
  hadc->Instance->CR2 |= ADC_CR2_DMAContReq((uint32_t)hadc->Init.DMAContinuousRequests);
  hadc->Instance->CR2 &= ~(ADC_CR2_EOCS);
  hadc->Instance->CR2 |= ADC_CR2_EOCSelection(hadc->Init.EOCSelection);
}

static void ADC_DMAConvCplt(DMA_HandleTypeDef *hdma)
{
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL | HAL_ADC_STATE_ERROR_DMA))
  {
    SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
    if(ADC_IS_SOFTWARE_START_REGULAR(hadc) &&
       (hadc->Init.ContinuousConvMode == DISABLE) &&
       (HAL_IS_BIT_CLR(hadc->Instance->SQR1, ADC_SQR1_L) ||
        HAL_IS_BIT_CLR(hadc->Instance->CR2, ADC_CR2_EOCS) ) )
    {
      __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC);
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);
      if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_READY);
      }
    }

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->ConvCpltCallback(hadc);
#else
    HAL_ADC_ConvCpltCallback(hadc);
#endif
  }
  else
  {
    if ((hadc->State & HAL_ADC_STATE_ERROR_INTERNAL) != 0UL)
    {

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->ErrorCallback(hadc);
#else
      HAL_ADC_ErrorCallback(hadc);
#endif
    }
 else
 {

      hadc->DMA_Handle->XferErrorCallback(hdma);
    }
  }
}

static void ADC_DMAHalfConvCplt(DMA_HandleTypeDef *hdma)
{
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
  hadc->ConvHalfCpltCallback(hadc);
#else
  HAL_ADC_ConvHalfCpltCallback(hadc);
#endif
}

static void ADC_DMAError(DMA_HandleTypeDef *hdma)
{
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  hadc->State= HAL_ADC_STATE_ERROR_DMA;

  hadc->ErrorCode |= HAL_ADC_ERROR_DMA;

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
  hadc->ErrorCallback(hadc);
#else
  HAL_ADC_ErrorCallback(hadc);
#endif
}
#endif
