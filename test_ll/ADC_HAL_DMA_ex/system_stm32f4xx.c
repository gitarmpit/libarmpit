#include "stm32f4xx.h"

#if !defined (HSE_VALUE)
  #define HSE_VALUE ((uint32_t)8000000)
#endif

#if !defined (HSI_VALUE)
  #define HSI_VALUE ((uint32_t)16000000)
#endif
#define VECT_TAB_OFFSET 0x00
uint32_t SystemCoreClock = 16000000;
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};
#if defined (DATA_IN_ExtSDRAM)
  static void SystemInit_ExtMemCtl(void);
#endif
void SystemInit(void)
{
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
  #endif

  RCC->CR |= (uint32_t)0x00000001;
  RCC->CFGR = 0x00000000;
  RCC->CR &= (uint32_t)0xFEF6FFFF;
  RCC->PLLCFGR = 0x24003010;
  RCC->CR &= (uint32_t)0xFFFBFFFF;
  RCC->CIR = 0x00000000;

#if defined (DATA_IN_ExtSDRAM)
  SystemInit_ExtMemCtl();
#endif

#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET;
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
#endif
}
void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;


  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:




      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;

      if (pllsource != 0)
      {

        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }
      else
      {

        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }

      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
      SystemCoreClock = pllvco/pllp;
      break;
    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }


  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];

  SystemCoreClock >>= tmp;
}

#if defined (DATA_IN_ExtSDRAM)
void SystemInit_ExtMemCtl(void)
{
  register uint32_t tmpreg = 0, timeout = 0xFFFF;
  register __IO uint32_t index;



  RCC->AHB1ENR |= 0x0000007D;


  GPIOA->AFR[0] = 0xC0000000;
  GPIOA->AFR[1] = 0x00000000;

  GPIOA->MODER = 0xA8008000;

  GPIOA->OSPEEDR = 0x0C00C000;

  GPIOA->OTYPER = 0x00000000;

  GPIOA->PUPDR = 0x64000000;


  GPIOC->AFR[0] = 0x00CC0000;
  GPIOC->AFR[1] = 0x00000000;

  GPIOC->MODER = 0x00000A00;

  GPIOC->OSPEEDR = 0x00000F00;

  GPIOC->OTYPER = 0x00000000;

  GPIOC->PUPDR = 0x00000000;


  GPIOD->AFR[0] = 0x000000CC;
  GPIOD->AFR[1] = 0xCC000CCC;

  GPIOD->MODER = 0xA02A000A;

  GPIOD->OSPEEDR = 0xF03F000F;

  GPIOD->OTYPER = 0x00000000;

  GPIOD->PUPDR = 0x00000000;


  GPIOE->AFR[0] = 0xC00000CC;
  GPIOE->AFR[1] = 0xCCCCCCCC;

  GPIOE->MODER = 0xAAAA800A;

  GPIOE->OSPEEDR = 0xFFFFC00F;

  GPIOE->OTYPER = 0x00000000;

  GPIOE->PUPDR = 0x00000000;


  GPIOF->AFR[0] = 0x00CCCCCC;
  GPIOF->AFR[1] = 0xCCCCC000;

  GPIOF->MODER = 0xAA800AAA;

  GPIOF->OSPEEDR = 0xFFC00FFF;

  GPIOF->OTYPER = 0x00000000;

  GPIOF->PUPDR = 0x00000000;


  GPIOG->AFR[0] = 0x00CC0CCC;
  GPIOG->AFR[1] = 0xC000000C;

  GPIOG->MODER = 0x80020A2A;

  GPIOG->OSPEEDR = 0xC0030F3F;

  GPIOG->OTYPER = 0x00000000;

  GPIOG->PUPDR = 0x00000000;



  RCC->AHB3ENR |= 0x00000001;


  FMC_Bank5_6->SDCR[0] = 0x00001954;
  FMC_Bank5_6->SDTR[0] = 0x01115351;



  FMC_Bank5_6->SDCMR = 0x00000011;
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
  }


  for (index = 0; index<1000; index++);


  FMC_Bank5_6->SDCMR = 0x00000012;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
  }


  FMC_Bank5_6->SDCMR = 0x000000F3;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
  }


  FMC_Bank5_6->SDCMR = 0x00044014;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
  }


  tmpreg = FMC_Bank5_6->SDRTR;
  FMC_Bank5_6->SDRTR = (tmpreg | (0x0000056A<<1));


  tmpreg = FMC_Bank5_6->SDCR[0];
  FMC_Bank5_6->SDCR[0] = (tmpreg & 0xFFFFFDFF);
}
#endif
