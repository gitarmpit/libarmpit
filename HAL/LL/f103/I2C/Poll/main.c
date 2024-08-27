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


int main(void)
{
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

  LL_I2C_Disable(I2C1);
  
  /* Configure the Own Address1 :
   *  - OwnAddress1 is SLAVE_OWN_ADDRESS
   *  - OwnAddrSize is LL_I2C_OWNADDRESS1_7BIT
   */
  LL_I2C_SetOwnAddress1(I2C1, SLAVE_OWN_ADDRESS, LL_I2C_OWNADDRESS1_7BIT);

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
}

void Activate_I2C_Master(void)
{
  LL_I2C_Enable(I2C1);
}
#endif /* SLAVE_BOARD */


#ifdef SLAVE_BOARD
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

  while(!LL_I2C_IsActiveFlag_STOP(I2C1))
  {
    /* (3.1) Receive data (RXNE flag raised) **********************************/

    /* Check RXNE flag value in ISR register */
    if(LL_I2C_IsActiveFlag_RXNE(I2C1))
    {
      /* Read character in Receive Data register.
      RXNE flag is cleared by reading data in DR register */
      aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
    }

    /* (3.2) Receive data (BTF flag raised) ***********************************/

    /* Check BTF flag value in ISR register */
    if(LL_I2C_IsActiveFlag_BTF(I2C1))
    {
      /* Read character in Receive Data register.
      BTF flag is cleared by reading data in DR register */
      aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
    }

  }

  /* (4) Clear pending flags, Check Data consistency **************************/
  /* End of I2C_SlaveReceiver_MasterTransmitter_DMA Process */
  LL_I2C_ClearFlag_STOP(I2C1);

  /* Check if data request to turn on the LED2 */
  if(Buffercmp8((uint8_t*)aReceiveBuffer, (uint8_t*)aLedOn, (ubReceiveIndex-1)) == 0)
  {
    LED_On();
  }
  else
  {
    Error_Callback();
  }
}

#else /* MASTER_BOARD */

void Handle_I2C_Master(void)
{
  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
  LL_I2C_GenerateStartCondition(I2C1);

  /* Loop until SB flag is raised  */
  while(!LL_I2C_IsActiveFlag_SB(I2C1))   // Only wait for SB not MASTER_EV5 = (MSL | BUSY | SB)
    ;

  /* (4) Send Slave address with a 7-Bit SLAVE_OWN_ADDRESS for a write request */
  LL_I2C_TransmitData8(I2C1, SLAVE_OWN_ADDRESS | I2C_REQUEST_WRITE);

  while(!LL_I2C_IsActiveFlag_ADDR(I2C1))
    ;

  LL_I2C_ClearFlag_ADDR(I2C1);


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

    }

  }

  /* (7) End of transfer, Data consistency are checking into Slave process *****/
  /* Generate Stop condition */
  LL_I2C_GenerateStopCondition(I2C1);

}
#endif /* SLAVE_BOARD */

void Error_Callback(void)
{
  LED_Blinking(LED_BLINK_ERROR);
}

