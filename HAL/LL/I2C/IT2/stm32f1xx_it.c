#include "stm32f1xx_it.h"

extern __IO uint8_t ubNbDataToTransmit;
extern __IO uint8_t ubNbDataToReceive;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f1xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles external lines 10 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void USER_BUTTON_IRQHANDLER(void)
{
  /* Manage Flags */
  if(LL_EXTI_IsActiveFlag_0_31(USER_BUTTON_EXTI_LINE) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(USER_BUTTON_EXTI_LINE);

    /* Manage code in main.c.*/
    UserButton_Callback(); 
  }
}

/**
  * Brief   This function handles I2C2 (Slave) event interrupt request.
  * Param   None
  * Retval  None
  */
void I2C2_EV_IRQHandler(void)
{
  /* Check ADDR flag value in ISR register */
  if(LL_I2C_IsActiveFlag_ADDR(I2C2))
  {
    /* Verify the slave transfer direction, a write direction, Slave enters transmitter mode */
    if(LL_I2C_GetTransferDirection(I2C2) == LL_I2C_DIRECTION_WRITE)
    {
      /* Enable Buffer Interrupts */
      LL_I2C_EnableIT_BUF(I2C2);

      /* Clear ADDR flag value in ISR register */
      LL_I2C_ClearFlag_ADDR(I2C2);
    }
    else
    {
      /* Clear ADDR flag value in ISR register */
      LL_I2C_ClearFlag_ADDR(I2C2);

      /* Call Error function */
      Error_Callback();
    }
  }
  /* Check TXE flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_TXE(I2C2))
  {
    /* Call function Slave Ready to Transmit Callback */
    Slave_Ready_To_Transmit_Callback();
  }
  /* Check BTF flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_BTF(I2C2))
  {
    /* Send the next byte */
    /* Call function Slave Ready to Transmit Callback */
    Slave_Ready_To_Transmit_Callback();
  }
}

/**
  * Brief   This function handles I2C2 (Slave) error interrupt request.
  * Param   None
  * Retval  None
  */
void I2C2_ER_IRQHandler(void)
{
  /* Normal use case, if all bytes are sent and Acknowledge failure appears */
  /* This correspond to the end of communication */
  if((ubNbDataToTransmit == 0) && (LL_I2C_IsActiveFlag_AF(I2C2)))
  {
    /* Clear AF flag value in ISR register */
    LL_I2C_ClearFlag_AF(I2C2);

    /* Call function Slave Complete Callback */
    Slave_Complete_Callback();
  }
  else
  {
    /* Call Error function */
    Error_Callback();
  }
}

