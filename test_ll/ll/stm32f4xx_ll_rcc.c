#if defined(USE_FULL_LL_DRIVER)

#include "stm32f4xx_ll_rcc.h"
#define assert_param(expr) ((void)0U)


#if defined(RCC)

#if defined(FMPI2C1)
#define IS_LL_RCC_FMPI2C_CLKSOURCE(__VALUE__) ((__VALUE__) == LL_RCC_FMPI2C1_CLKSOURCE)
#endif

#if defined(LPTIM1)
#define IS_LL_RCC_LPTIM_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_LPTIM1_CLKSOURCE))
#endif

#if defined(SAI1)
#if defined(RCC_DCKCFGR_SAI1SRC)
#define IS_LL_RCC_SAI_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_SAI1_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_SAI2_CLKSOURCE))
#elif defined(RCC_DCKCFGR_SAI1ASRC)
#define IS_LL_RCC_SAI_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_SAI1_A_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_SAI1_B_CLKSOURCE))
#endif
#endif

#if defined(SDIO)
#define IS_LL_RCC_SDIO_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_SDIO_CLKSOURCE))
#endif

#if defined(RNG)
#define IS_LL_RCC_RNG_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_RNG_CLKSOURCE))
#endif

#if defined(USB_OTG_FS) || defined(USB_OTG_HS)
#define IS_LL_RCC_USB_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_USB_CLKSOURCE))
#endif

#if defined(DFSDM2_Channel0)
#define IS_LL_RCC_DFSDM_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_DFSDM1_CLKSOURCE))

#define IS_LL_RCC_DFSDM_AUDIO_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_DFSDM1_AUDIO_CLKSOURCE) \
                                                    || ((__VALUE__) == LL_RCC_DFSDM2_AUDIO_CLKSOURCE))
#elif defined(DFSDM1_Channel0)
#define IS_LL_RCC_DFSDM_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_DFSDM1_CLKSOURCE))

#define IS_LL_RCC_DFSDM_AUDIO_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_DFSDM1_AUDIO_CLKSOURCE))
#endif

#if defined(RCC_DCKCFGR_I2S2SRC)
#define IS_LL_RCC_I2S_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_I2S1_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_I2S2_CLKSOURCE))
#else
#define IS_LL_RCC_I2S_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_I2S1_CLKSOURCE))
#endif

#if defined(CEC)
#define IS_LL_RCC_CEC_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_CEC_CLKSOURCE))
#endif

#if defined(DSI)
#define IS_LL_RCC_DSI_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_DSI_CLKSOURCE))
#endif

#if defined(LTDC)
#define IS_LL_RCC_LTDC_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_LTDC_CLKSOURCE))
#endif

#if defined(SPDIFRX)
#define IS_LL_RCC_SPDIFRX_CLKSOURCE(__VALUE__) (((__VALUE__) == LL_RCC_SPDIFRX1_CLKSOURCE))
#endif
uint32_t RCC_GetSystemClockFreq(void);
uint32_t RCC_GetHCLKClockFreq(uint32_t SYSCLK_Frequency);
uint32_t RCC_GetPCLK1ClockFreq(uint32_t HCLK_Frequency);
uint32_t RCC_GetPCLK2ClockFreq(uint32_t HCLK_Frequency);
uint32_t RCC_PLL_GetFreqDomain_SYS(uint32_t SYSCLK_Source);
uint32_t RCC_PLL_GetFreqDomain_48M(void);
#if defined(RCC_DCKCFGR_I2SSRC) || defined(RCC_DCKCFGR_I2S1SRC)
uint32_t RCC_PLL_GetFreqDomain_I2S(void);
#endif
#if defined(SPDIFRX)
uint32_t RCC_PLL_GetFreqDomain_SPDIFRX(void);
#endif
#if defined(RCC_PLLCFGR_PLLR)
#if defined(SAI1)
uint32_t RCC_PLL_GetFreqDomain_SAI(void);
#endif
#endif
#if defined(DSI)
uint32_t RCC_PLL_GetFreqDomain_DSI(void);
#endif
#if defined(RCC_PLLSAI_SUPPORT)
uint32_t RCC_PLLSAI_GetFreqDomain_SAI(void);
#if defined(RCC_PLLSAICFGR_PLLSAIP)
uint32_t RCC_PLLSAI_GetFreqDomain_48M(void);
#endif
#if defined(LTDC)
uint32_t RCC_PLLSAI_GetFreqDomain_LTDC(void);
#endif
#endif
#if defined(RCC_PLLI2S_SUPPORT)
uint32_t RCC_PLLI2S_GetFreqDomain_I2S(void);
#if defined(RCC_PLLI2SCFGR_PLLI2SQ) && !defined(RCC_DCKCFGR_PLLI2SDIVQ)
uint32_t RCC_PLLI2S_GetFreqDomain_48M(void);
#endif
#if defined(SAI1)
uint32_t RCC_PLLI2S_GetFreqDomain_SAI(void);
#endif
#if defined(SPDIFRX)
uint32_t RCC_PLLI2S_GetFreqDomain_SPDIFRX(void);
#endif
#endif
ErrorStatus LL_RCC_DeInit(void)
{
  uint32_t vl_mask = 0U;
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1U)
  {}

  LL_RCC_WriteReg(CFGR, 0x00000000U);

  vl_mask = 0xFFFFFFFFU;
  CLEAR_BIT(vl_mask, (RCC_CR_HSEON | RCC_CR_HSEBYP | RCC_CR_PLLON | RCC_CR_CSSON));
#if defined(RCC_PLLSAI_SUPPORT)
  CLEAR_BIT(vl_mask, RCC_CR_PLLSAION);
#endif

#if defined(RCC_PLLI2S_SUPPORT)
  CLEAR_BIT(vl_mask, RCC_CR_PLLI2SON);
#endif

  LL_RCC_WriteReg(CR, vl_mask);
  LL_RCC_HSI_SetCalibTrimming(0x10U);

  while(LL_RCC_PLL_IsReady() != 0U)
  {}


  LL_RCC_WriteReg(PLLCFGR, RCC_PLLCFGR_RST_VALUE);

#if defined(RCC_PLLI2S_SUPPORT)

  LL_RCC_WriteReg(PLLI2SCFGR, RCC_PLLI2SCFGR_RST_VALUE);
#endif

#if defined(RCC_PLLSAI_SUPPORT)

  LL_RCC_WriteReg(PLLSAICFGR, RCC_PLLSAICFGR_RST_VALUE);
#endif


  CLEAR_BIT(RCC->CIR, RCC_CIR_LSIRDYIE | RCC_CIR_LSERDYIE | RCC_CIR_HSIRDYIE | RCC_CIR_HSERDYIE | RCC_CIR_PLLRDYIE);

#if defined(RCC_CIR_PLLI2SRDYIE)
  CLEAR_BIT(RCC->CIR, RCC_CIR_PLLI2SRDYIE);
#endif

#if defined(RCC_CIR_PLLSAIRDYIE)
  CLEAR_BIT(RCC->CIR, RCC_CIR_PLLSAIRDYIE);
#endif


  SET_BIT(RCC->CIR, RCC_CIR_LSIRDYC | RCC_CIR_LSERDYC | RCC_CIR_HSIRDYC | RCC_CIR_HSERDYC | RCC_CIR_PLLRDYC | RCC_CIR_CSSC);

#if defined(RCC_CIR_PLLI2SRDYC)
  SET_BIT(RCC->CIR, RCC_CIR_PLLI2SRDYC);
#endif

#if defined(RCC_CIR_PLLSAIRDYC)
  SET_BIT(RCC->CIR, RCC_CIR_PLLSAIRDYC);
#endif


  CLEAR_BIT(RCC->CSR, RCC_CSR_LSION);
  SET_BIT(RCC->CSR, RCC_CSR_RMVF);

  return SUCCESS;
}

void LL_RCC_GetSystemClocksFreq(LL_RCC_ClocksTypeDef *RCC_Clocks)
{
  RCC_Clocks->SYSCLK_Frequency = RCC_GetSystemClockFreq();
  RCC_Clocks->HCLK_Frequency = RCC_GetHCLKClockFreq(RCC_Clocks->SYSCLK_Frequency);
  RCC_Clocks->PCLK1_Frequency = RCC_GetPCLK1ClockFreq(RCC_Clocks->HCLK_Frequency);
  RCC_Clocks->PCLK2_Frequency = RCC_GetPCLK2ClockFreq(RCC_Clocks->HCLK_Frequency);
}

#if defined(FMPI2C1)

uint32_t LL_RCC_GetFMPI2CClockFreq(uint32_t FMPI2CxSource)
{
  uint32_t FMPI2C_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  assert_param(IS_LL_RCC_FMPI2C_CLKSOURCE(FMPI2CxSource));

  if (FMPI2CxSource == LL_RCC_FMPI2C1_CLKSOURCE)
  {

    switch (LL_RCC_GetFMPI2CClockSource(FMPI2CxSource))
    {
      case LL_RCC_FMPI2C1_CLKSOURCE_SYSCLK:
        FMPI2C_frequency = RCC_GetSystemClockFreq();
        break;

      case LL_RCC_FMPI2C1_CLKSOURCE_HSI:
        if (LL_RCC_HSI_IsReady())
        {
          FMPI2C_frequency = HSI_VALUE;
        }
        break;

      case LL_RCC_FMPI2C1_CLKSOURCE_PCLK1:
      default:
        FMPI2C_frequency = RCC_GetPCLK1ClockFreq(RCC_GetHCLKClockFreq(RCC_GetSystemClockFreq()));
        break;
    }
  }

  return FMPI2C_frequency;
}
#endif

uint32_t LL_RCC_GetI2SClockFreq(uint32_t I2SxSource)
{
  uint32_t i2s_frequency = LL_RCC_PERIPH_FREQUENCY_NO;

  assert_param(IS_LL_RCC_I2S_CLKSOURCE(I2SxSource));

  if (I2SxSource == LL_RCC_I2S1_CLKSOURCE)
  {

    switch (LL_RCC_GetI2SClockSource(I2SxSource))
    {
#if defined(RCC_PLLI2S_SUPPORT)
      case LL_RCC_I2S1_CLKSOURCE_PLLI2S:
        if (LL_RCC_PLLI2S_IsReady())
        {
          i2s_frequency = RCC_PLLI2S_GetFreqDomain_I2S();
        }
        break;
#endif

#if defined(RCC_DCKCFGR_I2SSRC) || defined(RCC_DCKCFGR_I2S1SRC)
      case LL_RCC_I2S1_CLKSOURCE_PLL:
        if (LL_RCC_PLL_IsReady())
        {
          i2s_frequency = RCC_PLL_GetFreqDomain_I2S();
        }
        break;

      case LL_RCC_I2S1_CLKSOURCE_PLLSRC:
        switch (LL_RCC_PLL_GetMainSource())
        {
           case LL_RCC_PLLSOURCE_HSE:
             if (LL_RCC_HSE_IsReady())
             {
               i2s_frequency = HSE_VALUE;
             }
             break;

           case LL_RCC_PLLSOURCE_HSI:
           default:
             if (LL_RCC_HSI_IsReady())
             {
               i2s_frequency = HSI_VALUE;
             }
             break;
        }
        break;
#endif

      case LL_RCC_I2S1_CLKSOURCE_PIN:
      default:
        i2s_frequency = EXTERNAL_CLOCK_VALUE;
        break;
    }
  }
#if defined(RCC_DCKCFGR_I2S2SRC)
  else
  {

    switch (LL_RCC_GetI2SClockSource(I2SxSource))
    {
      case LL_RCC_I2S2_CLKSOURCE_PLLI2S:
        if (LL_RCC_PLLI2S_IsReady())
        {
          i2s_frequency = RCC_PLLI2S_GetFreqDomain_I2S();
        }
        break;

      case LL_RCC_I2S2_CLKSOURCE_PLL:
        if (LL_RCC_PLL_IsReady())
        {
          i2s_frequency = RCC_PLL_GetFreqDomain_I2S();
        }
        break;

      case LL_RCC_I2S2_CLKSOURCE_PLLSRC:
        switch (LL_RCC_PLL_GetMainSource())
        {
           case LL_RCC_PLLSOURCE_HSE:
             if (LL_RCC_HSE_IsReady())
             {
               i2s_frequency = HSE_VALUE;
             }
             break;

           case LL_RCC_PLLSOURCE_HSI:
           default:
             if (LL_RCC_HSI_IsReady())
             {
               i2s_frequency = HSI_VALUE;
             }
             break;
        }
        break;

      case LL_RCC_I2S2_CLKSOURCE_PIN:
      default:
        i2s_frequency = EXTERNAL_CLOCK_VALUE;
        break;
    }
  }
#endif

  return i2s_frequency;
}

#if defined(LPTIM1)


uint32_t LL_RCC_GetLPTIMClockFreq(uint32_t LPTIMxSource)
{
  uint32_t lptim_frequency = LL_RCC_PERIPH_FREQUENCY_NO;


  assert_param(IS_LL_RCC_LPTIM_CLKSOURCE(LPTIMxSource));

  if (LPTIMxSource == LL_RCC_LPTIM1_CLKSOURCE)
  {

    switch (LL_RCC_GetLPTIMClockSource(LPTIMxSource))
    {
      case LL_RCC_LPTIM1_CLKSOURCE_LSI:
        if (LL_RCC_LSI_IsReady())
        {
          lptim_frequency = LSI_VALUE;
        }
        break;

      case LL_RCC_LPTIM1_CLKSOURCE_HSI:
        if (LL_RCC_HSI_IsReady())
        {
          lptim_frequency = HSI_VALUE;
        }
        break;

      case LL_RCC_LPTIM1_CLKSOURCE_LSE:
        if (LL_RCC_LSE_IsReady())
        {
          lptim_frequency = LSE_VALUE;
        }
        break;

      case LL_RCC_LPTIM1_CLKSOURCE_PCLK1:
      default:
        lptim_frequency = RCC_GetPCLK1ClockFreq(RCC_GetHCLKClockFreq(RCC_GetSystemClockFreq()));
        break;
    }
  }

  return lptim_frequency;
}
#endif

#if defined(SAI1)
uint32_t LL_RCC_GetSAIClockFreq(uint32_t SAIxSource)
{
  uint32_t sai_frequency = LL_RCC_PERIPH_FREQUENCY_NO;


  assert_param(IS_LL_RCC_SAI_CLKSOURCE(SAIxSource));

#if defined(RCC_DCKCFGR_SAI1SRC)
  if ((SAIxSource == LL_RCC_SAI1_CLKSOURCE) || (SAIxSource == LL_RCC_SAI2_CLKSOURCE))
  {

    switch (LL_RCC_GetSAIClockSource(SAIxSource))
    {
      case LL_RCC_SAI1_CLKSOURCE_PLLSAI:
      case LL_RCC_SAI2_CLKSOURCE_PLLSAI:
        if (LL_RCC_PLLSAI_IsReady())
        {
          sai_frequency = RCC_PLLSAI_GetFreqDomain_SAI();
        }
        break;

      case LL_RCC_SAI1_CLKSOURCE_PLLI2S:
      case LL_RCC_SAI2_CLKSOURCE_PLLI2S:
        if (LL_RCC_PLLI2S_IsReady())
        {
          sai_frequency = RCC_PLLI2S_GetFreqDomain_SAI();
        }
        break;

      case LL_RCC_SAI1_CLKSOURCE_PLL:
      case LL_RCC_SAI2_CLKSOURCE_PLL:
        if (LL_RCC_PLL_IsReady())
        {
          sai_frequency = RCC_PLL_GetFreqDomain_SAI();
        }
        break;

      case LL_RCC_SAI2_CLKSOURCE_PLLSRC:
        switch (LL_RCC_PLL_GetMainSource())
        {
           case LL_RCC_PLLSOURCE_HSE:
             if (LL_RCC_HSE_IsReady())
             {
               sai_frequency = HSE_VALUE;
             }
             break;

           case LL_RCC_PLLSOURCE_HSI:
           default:
             if (LL_RCC_HSI_IsReady())
             {
               sai_frequency = HSI_VALUE;
             }
             break;
        }
        break;

      case LL_RCC_SAI1_CLKSOURCE_PIN:
      default:
        sai_frequency = EXTERNAL_CLOCK_VALUE;
        break;
    }
  }
#endif
#if defined(RCC_DCKCFGR_SAI1ASRC)
  if ((SAIxSource == LL_RCC_SAI1_A_CLKSOURCE) || (SAIxSource == LL_RCC_SAI1_B_CLKSOURCE))
  {

    switch (LL_RCC_GetSAIClockSource(SAIxSource))
    {
#if defined(RCC_PLLSAI_SUPPORT)
      case LL_RCC_SAI1_A_CLKSOURCE_PLLSAI:
      case LL_RCC_SAI1_B_CLKSOURCE_PLLSAI:
        if (LL_RCC_PLLSAI_IsReady())
        {
          sai_frequency = RCC_PLLSAI_GetFreqDomain_SAI();
        }
        break;
#endif

      case LL_RCC_SAI1_A_CLKSOURCE_PLLI2S:
      case LL_RCC_SAI1_B_CLKSOURCE_PLLI2S:
        if (LL_RCC_PLLI2S_IsReady())
        {
          sai_frequency = RCC_PLLI2S_GetFreqDomain_SAI();
        }
        break;

#if defined(RCC_SAI1A_PLLSOURCE_SUPPORT)
      case LL_RCC_SAI1_A_CLKSOURCE_PLL:
      case LL_RCC_SAI1_B_CLKSOURCE_PLL:
        if (LL_RCC_PLL_IsReady())
        {
          sai_frequency = RCC_PLL_GetFreqDomain_SAI();
        }
        break;

      case LL_RCC_SAI1_A_CLKSOURCE_PLLSRC:
      case LL_RCC_SAI1_B_CLKSOURCE_PLLSRC:
        switch (LL_RCC_PLL_GetMainSource())
        {
           case LL_RCC_PLLSOURCE_HSE:
             if (LL_RCC_HSE_IsReady())
             {
               sai_frequency = HSE_VALUE;
             }
             break;

           case LL_RCC_PLLSOURCE_HSI:
           default:
             if (LL_RCC_HSI_IsReady())
             {
               sai_frequency = HSI_VALUE;
             }
             break;
        }
        break;
#endif

      case LL_RCC_SAI1_A_CLKSOURCE_PIN:
      case LL_RCC_SAI1_B_CLKSOURCE_PIN:
      default:
        sai_frequency = EXTERNAL_CLOCK_VALUE;
        break;
    }
  }
#endif

  return sai_frequency;
}
#endif

#if defined(SDIO)

uint32_t LL_RCC_GetSDIOClockFreq(uint32_t SDIOxSource)
{
  uint32_t SDIO_frequency = LL_RCC_PERIPH_FREQUENCY_NO;


  assert_param(IS_LL_RCC_SDIO_CLKSOURCE(SDIOxSource));

  if (SDIOxSource == LL_RCC_SDIO_CLKSOURCE)
  {
#if defined(RCC_DCKCFGR_SDIOSEL) || defined(RCC_DCKCFGR2_SDIOSEL)

    switch (LL_RCC_GetSDIOClockSource(SDIOxSource))
    {
      case LL_RCC_SDIO_CLKSOURCE_PLL48CLK:
        switch (LL_RCC_GetCK48MClockSource(LL_RCC_CK48M_CLKSOURCE))
        {
          case LL_RCC_CK48M_CLKSOURCE_PLL:
            if (LL_RCC_PLL_IsReady())
            {
              SDIO_frequency = RCC_PLL_GetFreqDomain_48M();
            }
          break;

#if defined(RCC_PLLSAI_SUPPORT)
          case LL_RCC_CK48M_CLKSOURCE_PLLSAI:
          default:
            if (LL_RCC_PLLSAI_IsReady())
            {
              SDIO_frequency = RCC_PLLSAI_GetFreqDomain_48M();
            }
            break;
#endif

#if defined(RCC_PLLI2SCFGR_PLLI2SQ) && !defined(RCC_DCKCFGR_PLLI2SDIVQ)
          case LL_RCC_CK48M_CLKSOURCE_PLLI2S:
          default:
            if (LL_RCC_PLLI2S_IsReady())
            {
              SDIO_frequency = RCC_PLLI2S_GetFreqDomain_48M();
            }
            break;
#endif
        }
        break;

      case LL_RCC_SDIO_CLKSOURCE_SYSCLK:
      default:
      SDIO_frequency = RCC_GetSystemClockFreq();
      break;
    }
#else

    if (LL_RCC_PLL_IsReady())
    {
      SDIO_frequency = RCC_PLL_GetFreqDomain_48M();
    }
#endif
  }

  return SDIO_frequency;
}
#endif

#if defined(RNG)

uint32_t LL_RCC_GetRNGClockFreq(uint32_t RNGxSource)
{
  uint32_t rng_frequency = LL_RCC_PERIPH_FREQUENCY_NO;
  assert_param(IS_LL_RCC_RNG_CLKSOURCE(RNGxSource));

#if defined(RCC_DCKCFGR_CK48MSEL) || defined(RCC_DCKCFGR2_CK48MSEL)

  switch (LL_RCC_GetRNGClockSource(RNGxSource))
  {
#if defined(RCC_PLLI2SCFGR_PLLI2SQ) && !defined(RCC_DCKCFGR_PLLI2SDIVQ)
    case LL_RCC_RNG_CLKSOURCE_PLLI2S:
      if (LL_RCC_PLLI2S_IsReady())
      {
        rng_frequency = RCC_PLLI2S_GetFreqDomain_48M();
      }
      break;
#endif

#if defined(RCC_PLLSAI_SUPPORT)
    case LL_RCC_RNG_CLKSOURCE_PLLSAI:
      if (LL_RCC_PLLSAI_IsReady())
      {
        rng_frequency = RCC_PLLSAI_GetFreqDomain_48M();
      }
      break;
#endif

    case LL_RCC_RNG_CLKSOURCE_PLL:
    default:
      if (LL_RCC_PLL_IsReady())
      {
        rng_frequency = RCC_PLL_GetFreqDomain_48M();
      }
      break;
  }
#else

  if (LL_RCC_PLL_IsReady())
  {
    rng_frequency = RCC_PLL_GetFreqDomain_48M();
  }
#endif

  return rng_frequency;
}
#endif

#if defined(CEC)
uint32_t LL_RCC_GetCECClockFreq(uint32_t CECxSource)
{
  uint32_t cec_frequency = LL_RCC_PERIPH_FREQUENCY_NO;


  assert_param(IS_LL_RCC_CEC_CLKSOURCE(CECxSource));


  switch (LL_RCC_GetCECClockSource(CECxSource))
  {
    case LL_RCC_CEC_CLKSOURCE_LSE:
      if (LL_RCC_LSE_IsReady())
      {
        cec_frequency = LSE_VALUE;
      }
      break;

    case LL_RCC_CEC_CLKSOURCE_HSI_DIV488:
    default:
      if (LL_RCC_HSI_IsReady())
      {
        cec_frequency = HSI_VALUE/488U;
      }
      break;
  }

  return cec_frequency;
}
#endif

#if defined(USB_OTG_FS) || defined(USB_OTG_HS)
uint32_t LL_RCC_GetUSBClockFreq(uint32_t USBxSource)
{
  uint32_t usb_frequency = LL_RCC_PERIPH_FREQUENCY_NO;


  assert_param(IS_LL_RCC_USB_CLKSOURCE(USBxSource));

#if defined(RCC_DCKCFGR_CK48MSEL) || defined(RCC_DCKCFGR2_CK48MSEL)

  switch (LL_RCC_GetUSBClockSource(USBxSource))
  {
#if defined(RCC_PLLI2SCFGR_PLLI2SQ) && !defined(RCC_DCKCFGR_PLLI2SDIVQ)
    case LL_RCC_USB_CLKSOURCE_PLLI2S:
      if (LL_RCC_PLLI2S_IsReady())
      {
        usb_frequency = RCC_PLLI2S_GetFreqDomain_48M();
      }
      break;

#endif

#if defined(RCC_PLLSAI_SUPPORT)
    case LL_RCC_USB_CLKSOURCE_PLLSAI:
      if (LL_RCC_PLLSAI_IsReady())
      {
        usb_frequency = RCC_PLLSAI_GetFreqDomain_48M();
      }
      break;
#endif

    case LL_RCC_USB_CLKSOURCE_PLL:
    default:
      if (LL_RCC_PLL_IsReady())
      {
        usb_frequency = RCC_PLL_GetFreqDomain_48M();
      }
      break;
  }
#else

  if (LL_RCC_PLL_IsReady())
  {
    usb_frequency = RCC_PLL_GetFreqDomain_48M();
  }
#endif

  return usb_frequency;
}
#endif

#if defined(DFSDM1_Channel0)
uint32_t LL_RCC_GetDFSDMClockFreq(uint32_t DFSDMxSource)
{
  uint32_t dfsdm_frequency = LL_RCC_PERIPH_FREQUENCY_NO;


  assert_param(IS_LL_RCC_DFSDM_CLKSOURCE(DFSDMxSource));

  if (DFSDMxSource == LL_RCC_DFSDM1_CLKSOURCE)
  {

    switch (LL_RCC_GetDFSDMClockSource(DFSDMxSource))
    {
      case LL_RCC_DFSDM1_CLKSOURCE_SYSCLK:
        dfsdm_frequency = RCC_GetSystemClockFreq();
        break;

      case LL_RCC_DFSDM1_CLKSOURCE_PCLK2:
      default:
        dfsdm_frequency = RCC_GetPCLK2ClockFreq(RCC_GetHCLKClockFreq(RCC_GetSystemClockFreq()));
        break;
    }
  }
#if defined(DFSDM2_Channel0)
  else
  {

    switch (LL_RCC_GetDFSDMClockSource(DFSDMxSource))
    {
      case LL_RCC_DFSDM2_CLKSOURCE_SYSCLK:
        dfsdm_frequency = RCC_GetSystemClockFreq();
        break;

      case LL_RCC_DFSDM2_CLKSOURCE_PCLK2:
      default:
        dfsdm_frequency = RCC_GetPCLK2ClockFreq(RCC_GetHCLKClockFreq(RCC_GetSystemClockFreq()));
        break;
    }
  }
#endif

  return dfsdm_frequency;
}
uint32_t LL_RCC_GetDFSDMAudioClockFreq(uint32_t DFSDMxSource)
{
  uint32_t dfsdm_frequency = LL_RCC_PERIPH_FREQUENCY_NO;


  assert_param(IS_LL_RCC_DFSDM_AUDIO_CLKSOURCE(DFSDMxSource));

  if (DFSDMxSource == LL_RCC_DFSDM1_AUDIO_CLKSOURCE)
  {

    switch (LL_RCC_GetDFSDMAudioClockSource(DFSDMxSource))
    {
      case LL_RCC_DFSDM1_AUDIO_CLKSOURCE_I2S1:
        dfsdm_frequency = LL_RCC_GetI2SClockFreq(LL_RCC_I2S1_CLKSOURCE);
        break;

      case LL_RCC_DFSDM1_AUDIO_CLKSOURCE_I2S2:
      default:
        dfsdm_frequency = LL_RCC_GetI2SClockFreq(LL_RCC_I2S2_CLKSOURCE);
        break;
    }
  }
#if defined(DFSDM2_Channel0)
  else
  {

    switch (LL_RCC_GetDFSDMAudioClockSource(DFSDMxSource))
    {
      case LL_RCC_DFSDM2_AUDIO_CLKSOURCE_I2S1:
        dfsdm_frequency = LL_RCC_GetI2SClockFreq(LL_RCC_I2S1_CLKSOURCE);
        break;

      case LL_RCC_DFSDM2_AUDIO_CLKSOURCE_I2S2:
      default:
        dfsdm_frequency = LL_RCC_GetI2SClockFreq(LL_RCC_I2S2_CLKSOURCE);
        break;
    }
  }
#endif

  return dfsdm_frequency;
}
#endif

#if defined(DSI)
uint32_t LL_RCC_GetDSIClockFreq(uint32_t DSIxSource)
{
  uint32_t dsi_frequency = LL_RCC_PERIPH_FREQUENCY_NO;


  assert_param(IS_LL_RCC_DSI_CLKSOURCE(DSIxSource));


  switch (LL_RCC_GetDSIClockSource(DSIxSource))
  {
    case LL_RCC_DSI_CLKSOURCE_PLL:
      if (LL_RCC_PLL_IsReady())
      {
        dsi_frequency = RCC_PLL_GetFreqDomain_DSI();
      }
      break;

    case LL_RCC_DSI_CLKSOURCE_PHY:
    default:
      dsi_frequency = LL_RCC_PERIPH_FREQUENCY_NA;
      break;
  }

  return dsi_frequency;
}
#endif

#if defined(LTDC)

uint32_t LL_RCC_GetLTDCClockFreq(uint32_t LTDCxSource)
{
  uint32_t ltdc_frequency = LL_RCC_PERIPH_FREQUENCY_NO;


  assert_param(IS_LL_RCC_LTDC_CLKSOURCE(LTDCxSource));

  if (LL_RCC_PLLSAI_IsReady())
  {
     ltdc_frequency = RCC_PLLSAI_GetFreqDomain_LTDC();
  }

  return ltdc_frequency;
}
#endif

#if defined(SPDIFRX)
uint32_t LL_RCC_GetSPDIFRXClockFreq(uint32_t SPDIFRXxSource)
{
  uint32_t spdifrx_frequency = LL_RCC_PERIPH_FREQUENCY_NO;

  assert_param(IS_LL_RCC_SPDIFRX_CLKSOURCE(SPDIFRXxSource));

  switch (LL_RCC_GetSPDIFRXClockSource(SPDIFRXxSource))
  {
    case LL_RCC_SPDIFRX1_CLKSOURCE_PLLI2S:
      if (LL_RCC_PLLI2S_IsReady())
      {
        spdifrx_frequency = RCC_PLLI2S_GetFreqDomain_SPDIFRX();
      }
      break;

    case LL_RCC_SPDIFRX1_CLKSOURCE_PLL:
    default:
      if (LL_RCC_PLL_IsReady())
      {
        spdifrx_frequency = RCC_PLL_GetFreqDomain_SPDIFRX();
      }
      break;
  }

  return spdifrx_frequency;
}
#endif

uint32_t RCC_GetSystemClockFreq(void)
{
  uint32_t frequency = 0U;

  switch (LL_RCC_GetSysClkSource())
  {
    case LL_RCC_SYS_CLKSOURCE_STATUS_HSI:
      frequency = HSI_VALUE;
      break;

    case LL_RCC_SYS_CLKSOURCE_STATUS_HSE:
      frequency = HSE_VALUE;
      break;

    case LL_RCC_SYS_CLKSOURCE_STATUS_PLL:
      frequency = RCC_PLL_GetFreqDomain_SYS(LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
      break;

#if defined(RCC_PLLR_SYSCLK_SUPPORT)
    case LL_RCC_SYS_CLKSOURCE_STATUS_PLLR:
      frequency = RCC_PLL_GetFreqDomain_SYS(LL_RCC_SYS_CLKSOURCE_STATUS_PLLR);
      break;
#endif

    default:
      frequency = HSI_VALUE;
      break;
  }

  return frequency;
}

uint32_t RCC_GetHCLKClockFreq(uint32_t SYSCLK_Frequency)
{
  return __LL_RCC_CALC_HCLK_FREQ(SYSCLK_Frequency, LL_RCC_GetAHBPrescaler());
}

uint32_t RCC_GetPCLK1ClockFreq(uint32_t HCLK_Frequency)
{
  return __LL_RCC_CALC_PCLK1_FREQ(HCLK_Frequency, LL_RCC_GetAPB1Prescaler());
}

uint32_t RCC_GetPCLK2ClockFreq(uint32_t HCLK_Frequency)
{
  return __LL_RCC_CALC_PCLK2_FREQ(HCLK_Frequency, LL_RCC_GetAPB2Prescaler());
}

uint32_t RCC_PLL_GetFreqDomain_SYS(uint32_t SYSCLK_Source)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U, plloutputfreq = 0U;

  pllsource = LL_RCC_PLL_GetMainSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSI:
      pllinputfreq = HSI_VALUE;
      break;

    case LL_RCC_PLLSOURCE_HSE:
      pllinputfreq = HSE_VALUE;
      break;

    default:
      pllinputfreq = HSI_VALUE;
      break;
  }

  if (SYSCLK_Source == LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
    plloutputfreq = __LL_RCC_CALC_PLLCLK_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetP());
  }
#if defined(RCC_PLLR_SYSCLK_SUPPORT)
  else
  {
    plloutputfreq = __LL_RCC_CALC_PLLRCLK_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR());
  }
#endif

  return plloutputfreq;
}

uint32_t RCC_PLL_GetFreqDomain_48M(void)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U;

  pllsource = LL_RCC_PLL_GetMainSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSI:
      pllinputfreq = HSI_VALUE;
      break;

    case LL_RCC_PLLSOURCE_HSE:
      pllinputfreq = HSE_VALUE;
      break;

    default:
      pllinputfreq = HSI_VALUE;
      break;
  }
  return __LL_RCC_CALC_PLLCLK_48M_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetQ());
}

#if defined(DSI)
uint32_t RCC_PLL_GetFreqDomain_DSI(void)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U;
  pllsource = LL_RCC_PLL_GetMainSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSE:
      pllinputfreq = HSE_VALUE;
      break;

    case LL_RCC_PLLSOURCE_HSI:
    default:
      pllinputfreq = HSI_VALUE;
      break;
  }
  return __LL_RCC_CALC_PLLCLK_DSI_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR());
}
#endif

#if defined(RCC_DCKCFGR_I2SSRC) || defined(RCC_DCKCFGR_I2S1SRC)
uint32_t RCC_PLL_GetFreqDomain_I2S(void)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U;
  pllsource = LL_RCC_PLL_GetMainSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSE:
      pllinputfreq = HSE_VALUE;
      break;

    case LL_RCC_PLLSOURCE_HSI:
    default:
      pllinputfreq = HSI_VALUE;
      break;
  }
  return __LL_RCC_CALC_PLLCLK_I2S_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR());
}
#endif

#if defined(SPDIFRX)
uint32_t RCC_PLL_GetFreqDomain_SPDIFRX(void)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U;
  pllsource = LL_RCC_PLL_GetMainSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSE:
      pllinputfreq = HSE_VALUE;
      break;

    case LL_RCC_PLLSOURCE_HSI:
    default:
      pllinputfreq = HSI_VALUE;
      break;
  }
  return __LL_RCC_CALC_PLLCLK_SPDIFRX_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR());
}
#endif

#if defined(RCC_PLLCFGR_PLLR)
#if defined(SAI1)
uint32_t RCC_PLL_GetFreqDomain_SAI(void)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U, plloutputfreq = 0U;
  pllsource = LL_RCC_PLL_GetMainSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSE:
      pllinputfreq = HSE_VALUE;
      break;

    case LL_RCC_PLLSOURCE_HSI:
    default:
      pllinputfreq = HSI_VALUE;
      break;
  }

#if defined(RCC_DCKCFGR_PLLDIVR)
  plloutputfreq = __LL_RCC_CALC_PLLCLK_SAI_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR(), LL_RCC_PLL_GetDIVR());
#else
  plloutputfreq = __LL_RCC_CALC_PLLCLK_SAI_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR());
#endif

  return plloutputfreq;
}
#endif
#endif

#if defined(RCC_PLLSAI_SUPPORT)
uint32_t RCC_PLLSAI_GetFreqDomain_SAI(void)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U;
  pllsource = LL_RCC_PLL_GetMainSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSI:
      pllinputfreq = HSI_VALUE;
      break;

    case LL_RCC_PLLSOURCE_HSE:
      pllinputfreq = HSE_VALUE;
      break;

    default:
      pllinputfreq = HSI_VALUE;
      break;
  }
  return __LL_RCC_CALC_PLLSAI_SAI_FREQ(pllinputfreq, LL_RCC_PLLSAI_GetDivider(),
                                        LL_RCC_PLLSAI_GetN(), LL_RCC_PLLSAI_GetQ(), LL_RCC_PLLSAI_GetDIVQ());
}

#if defined(RCC_PLLSAICFGR_PLLSAIP)
uint32_t RCC_PLLSAI_GetFreqDomain_48M(void)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U;
  pllsource = LL_RCC_PLL_GetMainSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSI:
      pllinputfreq = HSI_VALUE;
      break;

    case LL_RCC_PLLSOURCE_HSE:
      pllinputfreq = HSE_VALUE;
      break;

    default:
      pllinputfreq = HSI_VALUE;
      break;
  }
  return __LL_RCC_CALC_PLLSAI_48M_FREQ(pllinputfreq, LL_RCC_PLLSAI_GetDivider(),
                                        LL_RCC_PLLSAI_GetN(), LL_RCC_PLLSAI_GetP());
}
#endif

#if defined(LTDC)
uint32_t RCC_PLLSAI_GetFreqDomain_LTDC(void)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U;
  pllsource = LL_RCC_PLL_GetMainSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSI:
      pllinputfreq = HSI_VALUE;
      break;

    case LL_RCC_PLLSOURCE_HSE:
      pllinputfreq = HSE_VALUE;
      break;

    default:
      pllinputfreq = HSI_VALUE;
      break;
  }
  return __LL_RCC_CALC_PLLSAI_LTDC_FREQ(pllinputfreq, LL_RCC_PLLSAI_GetDivider(),
                                        LL_RCC_PLLSAI_GetN(), LL_RCC_PLLSAI_GetR(), LL_RCC_PLLSAI_GetDIVR());
}
#endif
#endif

#if defined(RCC_PLLI2S_SUPPORT)
#if defined(SAI1)
uint32_t RCC_PLLI2S_GetFreqDomain_SAI(void)
{
  uint32_t plli2sinputfreq = 0U, plli2ssource = 0U, plli2soutputfreq = 0U;
  plli2ssource = LL_RCC_PLLI2S_GetMainSource();

  switch (plli2ssource)
  {
    case LL_RCC_PLLSOURCE_HSE:
      plli2sinputfreq = HSE_VALUE;
      break;

#if defined(RCC_PLLI2SCFGR_PLLI2SSRC)
    case LL_RCC_PLLI2SSOURCE_PIN:
      plli2sinputfreq = EXTERNAL_CLOCK_VALUE;
      break;
#endif

    case LL_RCC_PLLSOURCE_HSI:
    default:
      plli2sinputfreq = HSI_VALUE;
      break;
  }

#if defined(RCC_DCKCFGR_PLLI2SDIVQ)
  plli2soutputfreq = __LL_RCC_CALC_PLLI2S_SAI_FREQ(plli2sinputfreq, LL_RCC_PLLI2S_GetDivider(),
                                          LL_RCC_PLLI2S_GetN(), LL_RCC_PLLI2S_GetQ(), LL_RCC_PLLI2S_GetDIVQ());
#else
  plli2soutputfreq = __LL_RCC_CALC_PLLI2S_SAI_FREQ(plli2sinputfreq, LL_RCC_PLLI2S_GetDivider(),
                                          LL_RCC_PLLI2S_GetN(), LL_RCC_PLLI2S_GetR(), LL_RCC_PLLI2S_GetDIVR());
#endif

  return plli2soutputfreq;
}
#endif

#if defined(SPDIFRX)
uint32_t RCC_PLLI2S_GetFreqDomain_SPDIFRX(void)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U;
  pllsource = LL_RCC_PLLI2S_GetMainSource();

  switch (pllsource)
  {
    case LL_RCC_PLLSOURCE_HSE:
      pllinputfreq = HSE_VALUE;
      break;

    case LL_RCC_PLLSOURCE_HSI:
    default:
      pllinputfreq = HSI_VALUE;
      break;
  }

  return __LL_RCC_CALC_PLLI2S_SPDIFRX_FREQ(pllinputfreq, LL_RCC_PLLI2S_GetDivider(),
                                           LL_RCC_PLLI2S_GetN(), LL_RCC_PLLI2S_GetP());
}
#endif

uint32_t RCC_PLLI2S_GetFreqDomain_I2S(void)
{
  uint32_t plli2sinputfreq = 0U, plli2ssource = 0U, plli2soutputfreq = 0U;

  plli2ssource = LL_RCC_PLLI2S_GetMainSource();

  switch (plli2ssource)
  {
    case LL_RCC_PLLSOURCE_HSE:
      plli2sinputfreq = HSE_VALUE;
      break;

#if defined(RCC_PLLI2SCFGR_PLLI2SSRC)
    case LL_RCC_PLLI2SSOURCE_PIN:
      plli2sinputfreq = EXTERNAL_CLOCK_VALUE;
      break;
#endif

    case LL_RCC_PLLSOURCE_HSI:
    default:
      plli2sinputfreq = HSI_VALUE;
      break;
  }

  plli2soutputfreq = __LL_RCC_CALC_PLLI2S_I2S_FREQ(plli2sinputfreq, LL_RCC_PLLI2S_GetDivider(),
                                                   LL_RCC_PLLI2S_GetN(), LL_RCC_PLLI2S_GetR());

  return plli2soutputfreq;
}

#if defined(RCC_PLLI2SCFGR_PLLI2SQ) && !defined(RCC_DCKCFGR_PLLI2SDIVQ)

uint32_t RCC_PLLI2S_GetFreqDomain_48M(void)
{
  uint32_t plli2sinputfreq = 0U, plli2ssource = 0U, plli2soutputfreq = 0U;
  plli2ssource = LL_RCC_PLLI2S_GetMainSource();

  switch (plli2ssource)
  {
    case LL_RCC_PLLSOURCE_HSE:
      plli2sinputfreq = HSE_VALUE;
      break;

#if defined(RCC_PLLI2SCFGR_PLLI2SSRC)
    case LL_RCC_PLLI2SSOURCE_PIN:
      plli2sinputfreq = EXTERNAL_CLOCK_VALUE;
      break;
#endif

    case LL_RCC_PLLSOURCE_HSI:
    default:
      plli2sinputfreq = HSI_VALUE;
      break;
  }

  plli2soutputfreq = __LL_RCC_CALC_PLLI2S_48M_FREQ(plli2sinputfreq, LL_RCC_PLLI2S_GetDivider(),
                                                   LL_RCC_PLLI2S_GetN(), LL_RCC_PLLI2S_GetQ());

  return plli2soutputfreq;
}
#endif
#endif
#endif


#endif
