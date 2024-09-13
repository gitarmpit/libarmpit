#include "stm32l0xx.h"

#if !defined  (HSE_VALUE)
  #define HSE_VALUE    ((uint32_t)8000000U) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (MSI_VALUE)
  #define MSI_VALUE    ((uint32_t)2097152U) /*!< Value of the Internal oscillator in Hz*/
#endif /* MSI_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000U) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */


/* Note: Following vector table addresses must be defined in line with linker
         configuration. */
/*!< Uncomment the following line if you need to relocate the vector table
     anywhere in Flash or Sram, else the vector table is kept at the automatic
     remap of boot address selected */
/* #define USER_VECT_TAB_ADDRESS */

#if defined(USER_VECT_TAB_ADDRESS)
/*!< Uncomment the following line if you need to relocate your vector Table
     in Sram else user remap will be done in Flash. */
/* #define VECT_TAB_SRAM */
#if defined(VECT_TAB_SRAM)
#define VECT_TAB_BASE_ADDRESS   SRAM_BASE       /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#else
#define VECT_TAB_BASE_ADDRESS   FLASH_BASE      /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#endif /* VECT_TAB_SRAM */
#endif /* USER_VECT_TAB_ADDRESS */

uint32_t SystemCoreClock = 2097152U; /* 32.768 kHz * 2^6 */

const uint8_t AHBPrescTable[16] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};
const uint8_t APBPrescTable[8] = {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U};
const uint8_t PLLMulTable[9] = {3U, 4U, 6U, 8U, 12U, 16U, 24U, 32U, 48U};

void SystemInit (void)
{
  /* Configure the Vector Table location add offset address ------------------*/
#if defined (USER_VECT_TAB_ADDRESS)
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#endif /* USER_VECT_TAB_ADDRESS */
}

void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0U, pllmul = 0U, plldiv = 0U, pllsource = 0U, msirange = 0U;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00U:  /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> RCC_ICSCR_MSIRANGE_Pos;
      SystemCoreClock = (32768U * (1U << (msirange + 1U)));
      break;
    case 0x04U:  /* HSI used as system clock */
      if ((RCC->CR & RCC_CR_HSIDIVF) != 0U)
      {
        SystemCoreClock = HSI_VALUE / 4U;
      }
      else
      {
        SystemCoreClock = HSI_VALUE;
      }
      break;
    case 0x08U:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    default:  /* PLL used as system clock */
      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmul = RCC->CFGR & RCC_CFGR_PLLMUL;
      plldiv = RCC->CFGR & RCC_CFGR_PLLDIV;
      pllmul = PLLMulTable[(pllmul >> RCC_CFGR_PLLMUL_Pos)];
      plldiv = (plldiv >> RCC_CFGR_PLLDIV_Pos) + 1U;

      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

      if (pllsource == 0x00U)
      {
        /* HSI oscillator clock selected as PLL clock entry */
        if ((RCC->CR & RCC_CR_HSIDIVF) != 0U)
        {
          SystemCoreClock = (((HSI_VALUE / 4U) * pllmul) / plldiv);
        }
        else
        {
          SystemCoreClock = (((HSI_VALUE) * pllmul) / plldiv);
        }
      }
      else
      {
        /* HSE selected as PLL clock entry */
        SystemCoreClock = (((HSE_VALUE) * pllmul) / plldiv);
      }
      break;
  }
  /* Compute HCLK clock frequency --------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}

