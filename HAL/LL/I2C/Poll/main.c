#include "main.h"

#if (USE_TIMEOUT == 1)
#ifdef SLAVE_BOARD
#define I2C_SEND_TIMEOUT_RXNE_BTF_MS 5

#else /* MASTER_BOARD */

#define I2C_SEND_TIMEOUT_TXE_MS 5
#define I2C_SEND_TIMEOUT_SB_MS        5
#define I2C_SEND_TIMEOUT_ADDR_MS      5
#endif /* SLAVE_BOARD */
#endif /* USE_TIMEOUT */

/* I2C SPEEDCLOCK define to max value: Fast Mode @400kHz */
#define I2C_SPEEDCLOCK           400000
#define I2C_DUTYCYCLE            LL_I2C_DUTYCYCLE_2

#if (USE_TIMEOUT == 1)
uint32_t Timeout                 = 0; /* Variable used for Timeout management */
#endif /* USE_TIMEOUT */


const uint8_t aLedOn[]           = "LED ON";

#ifdef SLAVE_BOARD
uint8_t      aReceiveBuffer[0xF] = {0};
__IO uint8_t ubReceiveIndex      = 0;

#else /* MASTER_BOARD */
__IO uint8_t  ubNbDataToTransmit = sizeof(aLedOn);
uint8_t*      pTransmitBuffer    = (uint8_t*)aLedOn;
#endif /* SLAVE_BOARD */

__IO uint8_t ubButtonPress       = 0;

void     SystemClock_Config(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Off(void);
void     LED_Blinking(uint32_t Period);
void     UserButton_Init(void);
void     WaitForUserButtonPress(void);
#ifdef SLAVE_BOARD
void     Configure_I2C_Slave(void);
void     Activate_I2C_Slave(void);
void     Handle_I2C_Slave(void);
uint8_t  Buffercmp8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength);
#else /* MASTER_BOARD */
void     Configure_I2C_Master(void);
void     Activate_I2C_Master(void);
void     Handle_I2C_Master(void);
#endif /* SLAVE_BOARD */

int main(void)
{
  SystemClock_Config();
  LED_Init();
  LED_Off();
  UserButton_Init();

#ifdef SLAVE_BOARD
  Configure_I2C_Slave();
  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();
  Activate_I2C_Slave();
  Handle_I2C_Slave();
#else /* MASTER_BOARD */
  Configure_I2C_Master();

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();

  Activate_I2C_Master();
  Handle_I2C_Master();
#endif /* SLAVE_BOARD */

  /* Infinite loop */
  while (1)
  {
  }
}

#ifdef SLAVE_BOARD
/**
  * @brief  This function configures I2C1 in Slave mode.
  * @note   This function is used to :
  *         -1- Enables GPIO clock.
  *         -2- Enable the I2C1 peripheral clock and configures the I2C1 pins.
  *         -3- Configure I2C1 functional parameters.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
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

  LL_I2C_Disable(I2C1);
  
  /* Configure the Own Address1 :
   *  - OwnAddress1 is SLAVE_OWN_ADDRESS
   *  - OwnAddrSize is LL_I2C_OWNADDRESS1_7BIT
   */
  LL_I2C_SetOwnAddress1(I2C1, SLAVE_OWN_ADDRESS, LL_I2C_OWNADDRESS1_7BIT);

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
}

void Activate_I2C_Slave(void)
{
  LL_I2C_Enable(I2C1);
}

#else /* MASTER_BOARD */

void Configure_I2C_Master(void)
{
  LL_RCC_ClocksTypeDef rcc_clocks;
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

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

  
  /* Disable I2C1 prior modifying configuration registers */
  LL_I2C_Disable(I2C1);
  
  /* Retrieve Clock frequencies */
  LL_RCC_GetSystemClocksFreq(&rcc_clocks);

  /* Configure the SCL Clock Speed */
  LL_I2C_ConfigSpeed(I2C1, rcc_clocks.PCLK1_Frequency, I2C_SPEEDCLOCK, I2C_DUTYCYCLE);
  
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
}

void Activate_I2C_Master(void)
{
  LL_I2C_Enable(I2C1);
}
#endif /* SLAVE_BOARD */

void LED_Init(void)
{
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);

  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

void LED_On(void)
{
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

void LED_Off(void)
{
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :   
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
  
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(Period);
  }
}

void UserButton_Init(void)
{
  USER_BUTTON_GPIO_CLK_ENABLE();

  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);

  USER_BUTTON_SYSCFG_SET_EXTI();

  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn, 0x03);  
}

void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    LL_mDelay(LED_BLINK_FAST);
  }
  /* Turn LED2 off */
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

#ifdef SLAVE_BOARD
/**
  * @brief  This Function handle Slave events to perform a reception process
  * @note  This function is composed in different steps :
  *        -1- Prepare acknowledge for Slave address reception
  *        -2- Wait ADDR flag and check address match code and direction
  *        -3- Loop until end of transfer received (STOP flag raised)
  *             -3.1- Receive data (RXNE flag raised)
  *             -3.2- Receive data (BTF flag raised)
  *        -4- Clear pending flags, Check Data consistency
  * @param  None
  * @retval None
  */
void Handle_I2C_Slave(void)
{
  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

  while(!LL_I2C_IsActiveFlag_ADDR(I2C1))
  {
  }

  if(LL_I2C_GetTransferDirection(I2C1) == LL_I2C_DIRECTION_READ)
  {
    LL_I2C_ClearFlag_ADDR(I2C1);
  }
  else
  {
    LL_I2C_ClearFlag_ADDR(I2C1);
    Error_Callback();
  }


#if (USE_TIMEOUT == 1)
  Timeout = I2C_SEND_TIMEOUT_RXNE_BTF_MS;
#endif /* USE_TIMEOUT */

  while(!LL_I2C_IsActiveFlag_STOP(I2C1))
  {
    /* (3.1) Receive data (RXNE flag raised) **********************************/

    /* Check RXNE flag value in ISR register */
    if(LL_I2C_IsActiveFlag_RXNE(I2C1))
    {
      /* Read character in Receive Data register.
      RXNE flag is cleared by reading data in DR register */
      aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);

#if (USE_TIMEOUT == 1)
      Timeout = I2C_SEND_TIMEOUT_RXNE_BTF_MS;
#endif /* USE_TIMEOUT */
    }

    /* (3.2) Receive data (BTF flag raised) ***********************************/

    /* Check BTF flag value in ISR register */
    if(LL_I2C_IsActiveFlag_BTF(I2C1))
    {
      /* Read character in Receive Data register.
      BTF flag is cleared by reading data in DR register */
      aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);

#if (USE_TIMEOUT == 1)
      Timeout = I2C_SEND_TIMEOUT_RXNE_BTF_MS;
#endif /* USE_TIMEOUT */
    }

#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED2 to blinking mode */
        LED_Blinking(LED_BLINK_SLOW);
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* (4) Clear pending flags, Check Data consistency **************************/

  /* End of I2C_SlaveReceiver_MasterTransmitter_DMA Process */
  LL_I2C_ClearFlag_STOP(I2C1);

  /* Check if data request to turn on the LED2 */
  if(Buffercmp8((uint8_t*)aReceiveBuffer, (uint8_t*)aLedOn, (ubReceiveIndex-1)) == 0)
  {
    /* Turn LED2 On:
     *  - Expected bytes have been received
     *  - Slave Rx sequence completed successfully
     */
    LED_On();
  }
  else
  {
    /* Call Error function */
    Error_Callback();
  }
}

/**
  * @brief  Compares two 8-bit buffers and returns the comparison result.
  * @param  pBuffer1: pointer to the source buffer to be compared to.
  * @param  pBuffer2: pointer to the second source buffer to be compared to the first.
  * @param  BufferLength: buffer's length.
  *    - 0: Comparison is OK (the two Buffers are identical)
  *    - Value different from 0: Comparison is NOK (Buffers are different)
  */
uint8_t Buffercmp8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

#else /* MASTER_BOARD */

/**
  * @brief  This Function handle Master events to perform a transmission process
  * @note  This function is composed in different steps :
  *        -1- Prepare acknowledge for Master data reception.
  *        -2- Initiate a Start condition to the Slave device.
  *        -3- Loop until Start Bit transmitted (SB flag raised).
  *        -4- Send Slave address with a 7-Bit SLAVE_OWN_ADDRESS for a write request.
  *        -5- Loop until Address Acknowledgement received (ADDR flag raised).
  *        -6- Clear ADDR flag and loop until end of transfer (ubNbDataToTransmit == 0).
  *             -6.1 Transmit data (TXE flag raised).
  *        -7- End of transfer, Data consistency are checking into Slave process.
  * @param  None
  * @retval None
  */
void Handle_I2C_Master(void)
{
  /* (1) Prepare acknowledge for Master data reception ************************/
  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
  
  /* (2) Initiate a Start condition to the Slave device ***********************/
  /* Master Generate Start condition */
  LL_I2C_GenerateStartCondition(I2C1);

  /* (3) Loop until Start Bit transmitted (SB flag raised) ********************/

#if (USE_TIMEOUT == 1)
  Timeout = I2C_SEND_TIMEOUT_SB_MS;
#endif /* USE_TIMEOUT */

  /* Loop until SB flag is raised  */
  while(!LL_I2C_IsActiveFlag_SB(I2C1))
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED2 to blinking mode */
        LED_Blinking(LED_BLINK_SLOW);
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* (4) Send Slave address with a 7-Bit SLAVE_OWN_ADDRESS for a write request */
    LL_I2C_TransmitData8(I2C1, SLAVE_OWN_ADDRESS | I2C_REQUEST_WRITE);

  /* (5) Loop until Address Acknowledgement received (ADDR flag raised) *******/

#if (USE_TIMEOUT == 1)
  Timeout = I2C_SEND_TIMEOUT_ADDR_MS;
#endif /* USE_TIMEOUT */

  /* Loop until ADDR flag is raised  */
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1))
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED2 to blinking mode */
        LED_Blinking(LED_BLINK_SLOW);
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* (6) Clear ADDR flag and loop until end of transfer (ubNbDataToTransmit == 0) */

  /* Clear ADDR flag value in ISR register */
  LL_I2C_ClearFlag_ADDR(I2C1);

#if (USE_TIMEOUT == 1)
  Timeout = I2C_SEND_TIMEOUT_TXE_MS;
#endif /* USE_TIMEOUT */

  /* Loop until TXE flag is raised  */
  while(ubNbDataToTransmit > 0)
  {
    /* (6.1) Transmit data (TXE flag raised) **********************************/

    /* Check TXE flag value in ISR register */
    if(LL_I2C_IsActiveFlag_TXE(I2C1))
    {
      /* Write data in Transmit Data register.
      TXE flag is cleared by writing data in TXDR register */
      LL_I2C_TransmitData8(I2C1, (*pTransmitBuffer++));

      ubNbDataToTransmit--;

#if (USE_TIMEOUT == 1)
      Timeout = I2C_SEND_TIMEOUT_TXE_MS;
#endif /* USE_TIMEOUT */
    }

#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED2 to blinking mode */
        LED_Blinking(LED_BLINK_SLOW);
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* (7) End of transfer, Data consistency are checking into Slave process *****/
  /* Generate Stop condition */
  LL_I2C_GenerateStopCondition(I2C1);

  /* Turn LED2 On:
   *  - Expected bytes have been sent
   *  - Master Tx sequence completed successfully
   */
  LED_On();
}
#endif /* SLAVE_BOARD */

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}/******************************************************************************/
/*   IRQ HANDLER TREATMENT Functions                                          */
/******************************************************************************/
/**
  * @brief  Function to manage User push-button
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update User push-button variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
}

/**
  * @brief  Function called in case of error detected in I2C IT Handler
  * @param  None
  * @retval None
  */
void Error_Callback(void)
{
  /* Unexpected event : Set LED2 to Blinking mode to indicate error occurs */
  LED_Blinking(LED_BLINK_ERROR);
}

