#include "main.h"
void HAL_MspInit(void)
{
  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
  if(hpcd->Instance==USB)
  {
    __HAL_RCC_USB_CLK_ENABLE();
  }

}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{
  if(hpcd->Instance==USB)
  {
    __HAL_RCC_USB_CLK_DISABLE();
  }

}
