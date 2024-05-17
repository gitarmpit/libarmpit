#include "main.h"


#define I2C_SPEEDCLOCK           400000
#define I2C_DUTYCYCLE            LL_I2C_DUTYCYCLE_2

#define SLAVE_BYTE_TO_SEND       (uint8_t)0xA5

__IO uint8_t ubNbDataToTransmit     = sizeof(SLAVE_BYTE_TO_SEND);
__IO uint8_t ubNbDataToReceive      = sizeof(SLAVE_BYTE_TO_SEND);
__IO uint8_t ubButtonPress       = 0;

uint8_t      aReceiveBuffer[0xF] = {0};
__IO uint8_t ubReceiveIndex      = 0;


/**
  * @brief  This function configures I2C1 in Master mode.
  * @note   This function is used to :
  *         -1- Enables GPIO clock.
  *         -2- Enable the I2C1 peripheral clock and configures the I2C1 pins.
  *         -3- Configure NVIC for I2C1.
  *         -4- Configure and Enable I2C1 functional parameters.
  *         -5- Disable I2C1 (to prevent misplaced start/stop condition during
  *                                      GPIO initialization phase on Slave side).
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_I2C_Master(void)
{
  /* (1) Enables GPIO clock  **********************/

  /* Enable the peripheral clock of GPIOB */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  /* (2) Enable the I2C1 peripheral clock *************************************/

  /* Enable the peripheral clock for I2C1 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_UP);

  /* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_UP);

  /* (3) Configure NVIC for I2C1 **********************************************/

  /* Configure Event IT:
   *  - Set priority for I2C1_EV_IRQn
   *  - Enable I2C1_EV_IRQn
   */
  NVIC_SetPriority(I2C1_EV_IRQn, 0);  
  NVIC_EnableIRQ(I2C1_EV_IRQn);

  /* Configure Error IT:
   *  - Set priority for I2C1_ER_IRQn
   *  - Enable I2C1_ER_IRQn
   */
  NVIC_SetPriority(I2C1_ER_IRQn, 0);  
  NVIC_EnableIRQ(I2C1_ER_IRQn);

  /* (4) Configure I2C1 functional parameters ********************************/
  /* Reset I2C1 data registers */
  if (LL_I2C_DeInit(I2C1) != SUCCESS) 
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Configure the Own Address1                   */
  /* Reset Values of :
   *     - OwnAddress1 is 0x00
   *     - OwnAddrSize is LL_I2C_OWNADDRESS1_7BIT
   */
  //LL_I2C_SetOwnAddress1(I2C1, 0x00, LL_I2C_OWNADDRESS1_7BIT);

  /* Enable Clock stretching */
  /* Reset Value is Clock stretching enabled */
  //LL_I2C_EnableClockStretching(I2C1);

  /* Enable General Call                  */
  /* Reset Value is General Call disabled */
  //LL_I2C_EnableGeneralCall(I2C1);

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

  /* Set fields of initialization structure:
   *  - Peripheral Mode     = Mode I2C
   *  - Clock Speed         = fast Mode @400kHz
   *  - Duty Cycle          = fast mode duty cycle tlow/thigh = 2
   *  - Own Address1        = SLAVE_OWN_ADDRESS
   *  - Type of Acknowledge = Acknowledge bytes/address received
   *  - Own Address 1 Size  = Slave Address in 7-bit
  */
  LL_I2C_InitTypeDef i2c_initstruct;

  i2c_initstruct.PeripheralMode  = LL_I2C_MODE_I2C;
  i2c_initstruct.ClockSpeed      = I2C_SPEEDCLOCK;
  i2c_initstruct.DutyCycle       = I2C_DUTYCYCLE;
  i2c_initstruct.OwnAddress1     = 0x00;
  i2c_initstruct.TypeAcknowledge = LL_I2C_ACK;
  i2c_initstruct.OwnAddrSize     = LL_I2C_OWNADDRESS1_7BIT;

  /* Initialize I2C instance according to parameters defined in initialization structure. */  
  if (LL_I2C_Init(I2C1, &i2c_initstruct) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Disable I2C1 (to prevent misplaced start/stop condition during
   *                          GPIO initialization phase on Master side)
   */
  LL_I2C_Disable(I2C1);
}


void Activate_I2C_Master(void)
{
  LL_I2C_Enable(I2C1);
  LL_I2C_EnableIT_EVT(I2C1);
  LL_I2C_EnableIT_ERR(I2C1);
}

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


/**
  * @brief  Function called from I2C IRQ Handler when STOP flag is set
  *         Function is in charge of checking data received,
  *         LED2 is On if data are correct.
  * @param  None
  * @retval None
  */
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
      aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C2);
      
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

/**
  * Brief   This function handles I2C1 (Master) error interrupt request.
  * Param   None
  * Retval  None
  */
void I2C1_ER_IRQHandler(void)
{
  /* Call Error function */
  Error_Callback();
}
