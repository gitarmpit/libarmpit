/* I2C SPEEDCLOCK define to max value: Fast Mode @400kHz */
#define I2C_SPEEDCLOCK           400000
#define I2C_DUTYCYCLE            LL_I2C_DUTYCYCLE_2

#define SLAVE_BYTE_TO_SEND       (uint8_t)0xA5

#ifndef SLAVE_BOARD
uint8_t      aReceiveBuffer[0xF] = {0};
__IO uint8_t ubReceiveIndex      = 0;
#endif /* SLAVE_BOARD */

__IO uint8_t ubButtonPress       = 0;
__IO uint8_t ubNbDataToTransmit  = sizeof(SLAVE_BYTE_TO_SEND);
__IO uint8_t ubNbDataToReceive   = sizeof(SLAVE_BYTE_TO_SEND);


int main(void)
{
  SystemClock_Config();

  LED_Init();

  LED_Off();

  UserButton_Init();

#ifdef SLAVE_BOARD
  Configure_I2C_Slave();
  WaitForUserButtonPress();
  Activate_I2C_Slave();
  Handle_I2C_Slave();
#else /* MASTER_BOARD */
  Configure_I2C_Master();
  WaitForUserButtonPress();
  Activate_I2C_Master();
  Handle_I2C_Master();
#endif /* SLAVE_BOARD */
  while (1)
  {
  }
}

#ifdef SLAVE_BOARD
void Configure_I2C_Slave(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_UP);

  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_UP);

  NVIC_SetPriority(I2C1_EV_IRQn, 0);  
  NVIC_EnableIRQ(I2C1_EV_IRQn);

  NVIC_SetPriority(I2C1_ER_IRQn, 0);  
  NVIC_EnableIRQ(I2C1_ER_IRQn);

  LL_I2C_Disable(I2C1);
  LL_I2C_SetOwnAddress1(I2C1, SLAVE_OWN_ADDRESS, LL_I2C_OWNADDRESS1_7BIT);


  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C1, LL_I2C_MODE_I2C);
}

void Activate_I2C_Slave(void)
{
  LL_I2C_Enable(I2C1);
  
  // *  - Enable Events Interrupt
  // *  - Enable Error interrupts
  LL_I2C_EnableIT_EVT(I2C1);
  LL_I2C_EnableIT_ERR(I2C1);
}

#else /* MASTER_BOARD */

void Configure_I2C_Master(void)
{
  LL_RCC_ClocksTypeDef rcc_clocks;
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_UP);

  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_UP);


  // Configure Event IT:
  NVIC_SetPriority(I2C1_EV_IRQn, 0);  
  NVIC_EnableIRQ(I2C1_EV_IRQn);

  // Configure Error IT:
  NVIC_SetPriority(I2C1_ER_IRQn, 0);  
  NVIC_EnableIRQ(I2C1_ER_IRQn);

  LL_I2C_Disable(I2C1);
  LL_RCC_GetSystemClocksFreq(&rcc_clocks);
  LL_I2C_ConfigSpeed(I2C1, rcc_clocks.PCLK1_Frequency, I2C_SPEEDCLOCK, I2C_DUTYCYCLE);
  
  /* Configure the 7bits Own Address2     */
  /* Reset Values of :
   *     - OwnAddress2 is 0x00
   *     - Own Address2 is disabled
   */
  //LL_I2C_SetOwnAddress2(I2C1, 0x00);
  //LL_I2C_DisableOwnAddress2(I2C1);

  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C1, LL_I2C_MODE_I2C);
}

void Activate_I2C_Master(void)
{
  LL_I2C_Enable(I2C1);
  LL_I2C_EnableIT_EVT(I2C1);
  LL_I2C_EnableIT_ERR(I2C1);
}
#endif /* SLAVE_BOARD */


#ifdef SLAVE_BOARD
void Handle_I2C_Slave(void)
{
  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
}

/**
  * @brief  Function called from I2C IRQ Handler when TXE flag is set
  *         Function is in charge of transmit a byte on I2C lines.
  * @param  None
  * @retval None
  */
void Slave_Ready_To_Transmit_Callback(void)
{
  ubNbDataToTransmit--;  

  if(ubNbDataToTransmit == 0)
  {
    LL_I2C_DisableIT_BUF(I2C1);
  }
  
  LL_I2C_TransmitData8(I2C1, SLAVE_BYTE_TO_SEND);
}

void Slave_Complete_Callback(void)
{
  LED_On();
}

#else /* MASTER_BOARD */

void Handle_I2C_Master(void)
{
  /* (1) Prepare acknowledge for Master data reception ************************/
  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
  
  /* (2) Initiate a Start condition to the Slave device ***********************/
  /* Master Generate Start condition */
  LL_I2C_GenerateStartCondition(I2C1);

}

void Master_Reception_Callback(void)
{
  if(ubNbDataToReceive > 3)
  {
    /* Read character in Receive Data register.
    RXNE flag is cleared by reading data in RXDR register */
    aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
    
    /* Update ubNbDataToReceive variable */
    ubNbDataToReceive--;
  }
  else if((ubNbDataToReceive == 2) || (ubNbDataToReceive == 3))
  {
    /* Disable Buffer Interrupts */
    LL_I2C_DisableIT_BUF(I2C1);
  }
  else if(ubNbDataToReceive == 1)
  {
    /* Disable Buffer Interrupts */
    LL_I2C_DisableIT_BUF(I2C1);

    /* Generate Stop condition */
    LL_I2C_GenerateStopCondition(I2C1);
    
    /* Read character in Receive Data register.
    RXNE flag is cleared by reading data in RXDR register */
    aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
    
    /* Update ubNbDataToReceive variable */
    ubNbDataToReceive--;
    
    /* Call function Master Complete Callback */
    Master_Complete_Callback();
  }
}

void Master_Complete_Callback(void)
{
  if(ubNbDataToReceive == 3)
  {
    /* Prepare the generation of a Non ACKnowledge condition after next received bytes */
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
    
    /* Read character in Receive Data register.
    RXNE flag is cleared by reading data in RXDR register */
    aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
    ubNbDataToReceive--;

    /* Disable Buffer Interrupts */
    LL_I2C_DisableIT_BUF(I2C1);
  }
  else if(ubNbDataToReceive == 2)
  {
    /* Generate Stop condition */
    LL_I2C_GenerateStopCondition(I2C1);

    /* Read character from Receive Data register.
    RXNE flag is cleared by reading data in RXDR register */
    aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
    ubNbDataToReceive--;

    /* Read character from shift register.
    RXNE flag is cleared by reading data in RXDR register */
    aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
    ubNbDataToReceive--;
  }
  else
  {
    if(ubNbDataToReceive > 0)
    {
      /* Read character from shift register.
      RXNE flag is cleared by reading data in RXDR register */
      aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
      
      /* Update ubNbDataToReceive variable */
      ubNbDataToReceive--;
    }
  }

  if(ubNbDataToReceive == 0)
  {
    /* (1) Disable I2C1 transfer event/error interrupts:
     *  - Disable Events Interrupt
     *  - Disable Error interrupts
     */
    LL_I2C_DisableIT_EVT(I2C1);
    LL_I2C_DisableIT_ERR(I2C1);

    /* Read Received character.
    RXNE flag is cleared by reading of RXDR register */
    if(aReceiveBuffer[ubReceiveIndex-1] == SLAVE_BYTE_TO_SEND)
    {
      /* Turn LED2 On:
       * - Expected byte has been received
       * - Master Rx sequence completed successfully
       */
      LED_On();
    }
    else
    {
      /* Call Error function */
      Error_Callback();
    }
  }
}
#endif /* SLAVE_BOARD */



#ifdef SLAVE_BOARD
/**
  * Brief   This function handles I2C1 (Slave) event interrupt request.
  */
void I2C1_EV_IRQHandler(void)
{
  /* Check ADDR flag value in ISR register */
  if(LL_I2C_IsActiveFlag_ADDR(I2C1))
  {
    /* Verify the slave transfer direction, a write direction, Slave enters transmitter mode */
    if(LL_I2C_GetTransferDirection(I2C1) == LL_I2C_DIRECTION_WRITE)
    {
      /* Enable Buffer Interrupts */
      LL_I2C_EnableIT_BUF(I2C1);

      /* Clear ADDR flag value in ISR register */
      LL_I2C_ClearFlag_ADDR(I2C1);
    }
    else
    {
      /* Clear ADDR flag value in ISR register */
      LL_I2C_ClearFlag_ADDR(I2C1);

      /* Call Error function */
      Error_Callback();
    }
  }
  /* Check TXE flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_TXE(I2C1))
  {
    /* Call function Slave Ready to Transmit Callback */
    Slave_Ready_To_Transmit_Callback();
  }
  /* Check BTF flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_BTF(I2C1))
  {
    /* Send the next byte */
    /* Call function Slave Ready to Transmit Callback */
    Slave_Ready_To_Transmit_Callback();
  }
}

void I2C1_ER_IRQHandler(void)
{
  /* Normal use case, if all bytes are sent and Acknowledge failure appears */
  /* This correspond to the end of communication */
  if((ubNbDataToTransmit == 0) && (LL_I2C_IsActiveFlag_AF(I2C1)))
  {
    /* Clear AF flag value in ISR register */
    LL_I2C_ClearFlag_AF(I2C1);

    /* Call function Slave Complete Callback */
    Slave_Complete_Callback();
  }
  else
  {
    /* Call Error function */
    Error_Callback();
  }
}

#else /* MASTER_BOARD */

/**
  * Brief   This function handles I2C1 (Master) interrupt request.
  * Param   None
  * Retval  None
  */
void I2C1_EV_IRQHandler(void)
{
  /* Check SB flag value in ISR register */
  if(LL_I2C_IsActiveFlag_SB(I2C1))
  {
    /* Send Slave address with a 7-Bit SLAVE_OWN_ADDRESS for a read request */
    LL_I2C_TransmitData8(I2C1, SLAVE_OWN_ADDRESS | I2C_REQUEST_READ);
  }
  /* Check ADDR flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_ADDR(I2C1))
  {
    if(ubNbDataToReceive == 1)
    {
      /* Prepare the generation of a Non ACKnowledge condition after next received byte */
      LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
      
      /* Enable Buffer Interrupts */
      LL_I2C_EnableIT_BUF(I2C1);
    }
    else if(ubNbDataToReceive == 2)
    {
      /* Prepare the generation of a Non ACKnowledge condition after next received byte */
      LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
      
      /* Enable Pos */
      LL_I2C_EnableBitPOS(I2C1);
    }
    else
    {
      /* Enable Buffer Interrupts */
      LL_I2C_EnableIT_BUF(I2C1);
    }

    /* Clear ADDR flag value in ISR register */
    LL_I2C_ClearFlag_ADDR(I2C1);
  }
  /* Check BTF flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_BTF(I2C1))
  {
    /* Call function Master Complete Callback */
    Master_Complete_Callback();
  }
  /* Check RXNE flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_RXNE(I2C1))
  {
    /* Call function Master Reception Callback */
    Master_Reception_Callback();
  }
}

void I2C1_ER_IRQHandler(void)
{
  Error_Callback();
}
#endif /* SLAVE_BOARD */
