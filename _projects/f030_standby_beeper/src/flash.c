#include "config.h"
#include "stm32f0xx.h"

#define FLASH_POSITION_IWDGSW_BIT        8U
#define OB_VDDA_ANALOG_ON          ((uint8_t)0x20U) /*!< Analog monitoring on VDDA Power source ON */
#define OB_VDDA_ANALOG_OFF         ((uint8_t)0x00U) /*!< Analog monitoring on VDDA Power source OFF */

#define HAL_IS_BIT_SET(REG, BIT)         (((REG) & (BIT)) == (BIT))
#define HAL_IS_BIT_CLR(REG, BIT)         (((REG) & (BIT)) == 0U)

#define __HAL_FLASH_GET_FLAG(__FLAG__)   (((FLASH->SR) & (__FLAG__)) == (__FLAG__))
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)   ((FLASH->SR) = (__FLAG__))

uint8_t FLASH_OB_GetUser(void)
{
  return (uint8_t)((READ_REG(FLASH->OBR) & FLASH_OBR_USER) >> FLASH_POSITION_IWDGSW_BIT);
}


static BOOL FLASH_WaitForLastOperation(void)
{
  /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
     Even if the FLASH operation fails, the BUSY flag will be reset and an error
     flag will be set */
     
  while(__HAL_FLASH_GET_FLAG(FLASH_SR_BSY)) 
    ;
  
  /* Check FLASH End of Operation flag  */
  if (__HAL_FLASH_GET_FLAG(FLASH_SR_EOP))
  {
    /* Clear FLASH End of Operation pending bit */
    __HAL_FLASH_CLEAR_FLAG(FLASH_SR_EOP);
  }
  
  if(__HAL_FLASH_GET_FLAG(FLASH_SR_WRPERR)  || 
     __HAL_FLASH_GET_FLAG(FLASH_SR_PGERR))
  {
    return FALSE;
  }

  return TRUE;
}


void HAL_FLASH_Lock(void)
{
  SET_BIT(FLASH->CR, FLASH_CR_LOCK);
}

void HAL_FLASH_Unlock(void)
{

  if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET)
  {
    /* Authorize the FLASH Registers access */
    WRITE_REG(FLASH->KEYR, FLASH_KEY1);
    WRITE_REG(FLASH->KEYR, FLASH_KEY2);

    /* Verify Flash is unlocked */
    if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET)
    {
    }
  }

}

void HAL_FLASH_OB_Unlock(void)
{
  if (HAL_IS_BIT_CLR(FLASH->CR, FLASH_CR_OPTWRE))
  {
    /* Authorizes the Option Byte register programming */
    WRITE_REG(FLASH->OPTKEYR, FLASH_OPTKEY1);
    WRITE_REG(FLASH->OPTKEYR, FLASH_OPTKEY2);
  }
  
}

void HAL_FLASH_OB_Lock(void)
{
  /* Clear the OPTWRE Bit to lock the FLASH Option Byte Registers access */
  CLEAR_BIT(FLASH->CR, FLASH_CR_OPTWRE);
}


void HAL_FLASH_OB_Launch(void) {

  /* Set the OBL_Launch bit to launch the option byte loading */
  SET_BIT(FLASH->CR, FLASH_CR_OBL_LAUNCH);
  
  /* Wait for last operation to be completed */
  FLASH_WaitForLastOperation();
}

void FLASH_OB_UserConfig(uint8_t UserConfig)
{

  //assert_param(IS_OB_IWDG_SOURCE((UserConfig&OB_IWDG_SW)));
  //assert_param(IS_OB_STOP_SOURCE((UserConfig&OB_STOP_NO_RST)));
  //assert_param(IS_OB_STDBY_SOURCE((UserConfig&OB_STDBY_NO_RST)));
  //assert_param(IS_OB_BOOT1((UserConfig&OB_BOOT1_SET)));
  //assert_param(IS_OB_VDDA_ANALOG((UserConfig&OB_VDDA_ANALOG_ON)));
  //assert_param(IS_OB_SRAM_PARITY((UserConfig&OB_SRAM_PARITY_RESET)));

  /* Wait for last operation to be completed */
  if (FLASH_WaitForLastOperation()) {
  
    /* Enable the Option Bytes Programming operation */
    SET_BIT(FLASH->CR, FLASH_CR_OPTPG); 
 
#if defined(FLASH_OBR_BOOT_SEL)
    OB->USER = UserConfig;
#else
    OB->USER = (UserConfig | 0x88U);
#endif

    /* Wait for last operation to be completed */
    FLASH_WaitForLastOperation();

    /* if the program operation is completed, disable the OPTPG Bit */
    CLEAR_BIT(FLASH->CR, FLASH_CR_OPTPG);
  }
  
}





