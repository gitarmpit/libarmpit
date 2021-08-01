#if defined(USE_FULL_LL_DRIVER)

#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_bus.h"
#if defined(PWR)

ErrorStatus LL_PWR_DeInit(void)
{
  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_PWR);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_PWR);
  return SUCCESS;
}

#endif /* defined(PWR) */
#endif /* USE_FULL_LL_DRIVER */
