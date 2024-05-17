#include "main.h"



#define SLAVE_BYTE_TO_SEND       (uint8_t)0xA5

__IO uint8_t ubNbDataToTransmit     = sizeof(SLAVE_BYTE_TO_SEND);
__IO uint8_t ubNbDataToReceive      = sizeof(SLAVE_BYTE_TO_SEND);
__IO uint8_t ubButtonPress       = 0;

uint8_t      aReceiveBuffer[0xF] = {0};
__IO uint8_t ubReceiveIndex      = 0;


void Configure_I2C_Slave(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);

  // SCL: B10, SDA: B11

  /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);

  /* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_11, LL_GPIO_PULL_UP);

  NVIC_SetPriority(I2C2_EV_IRQn, 0);  
  NVIC_EnableIRQ(I2C2_EV_IRQn);

  NVIC_SetPriority(I2C2_ER_IRQn, 0);  
  NVIC_EnableIRQ(I2C2_ER_IRQn);

  if (LL_I2C_DeInit(I2C2) != SUCCESS) 
  {
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Enable Clock stretching */
  /* Reset Value is Clock stretching enabled */
  //LL_I2C_EnableClockStretching(I2C2);
  
  /* Enable General Call                  */
  /* Reset Value is General Call disabled */
  //LL_I2C_EnableGeneralCall(I2C2);

  /* Configure the 7bits Own Address2     */
  /* Reset Values of :
   *     - OwnAddress2 is 0x00
   *     - Own Address2 is disabled
   */
  //LL_I2C_SetOwnAddress2(I2C2, 0x00);
  //LL_I2C_DisableOwnAddress2(I2C2);

  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C2, LL_I2C_MODE_I2C);

  /* Set fields of initialization structure:
   *  - Peripheral Mode     = Mode I2C
   *  - Clock Speed         = fast Mode @400kHz
   *  - Duty Cycle          = fast mode duty cycle tlow/thigh = 2
   *  - Own Address1        = SLAVE_OWN_ADDRESS
   *  - Type of Acknowledge = Acknowledge bytes/address received
   *  - Own Address 1 Size  = Slave Address in 7-bit
  */
  i2c_initstruct.PeripheralMode  = LL_I2C_MODE_I2C;
  i2c_initstruct.ClockSpeed      = I2C_SPEEDCLOCK;
  i2c_initstruct.DutyCycle       = I2C_DUTYCYCLE;
  i2c_initstruct.OwnAddress1     = SLAVE_OWN_ADDRESS;
  i2c_initstruct.TypeAcknowledge = LL_I2C_ACK;
  i2c_initstruct.OwnAddrSize     = LL_I2C_OWNADDRESS1_7BIT;

  /* Initialize I2C instance according to parameters defined in initialization structure. */  
  if (LL_I2C_Init(I2C2, &i2c_initstruct) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  LL_I2C_Disable(I2C2);
}


void Activate_I2C_Slave(void)
{
  LL_I2C_Enable(I2C2);
  LL_I2C_EnableIT_EVT(I2C2);  // Enable Event Int
  LL_I2C_EnableIT_ERR(I2C2);  // Enbale Error Int
}


void Handle_I2C_Slave(void)
{
  LL_I2C_AcknowledgeNextData(I2C2, LL_I2C_ACK);
}

void Slave_Ready_To_Transmit_Callback(void)
{
  ubNbDataToTransmit--;  

  if(ubNbDataToTransmit == 0)
  {
    /* Disable Buffer Interrupts */
    LL_I2C_DisableIT_BUF(I2C2);
  }
  
  /* Send the Byte requested by the Master */
  LL_I2C_TransmitData8(I2C2, SLAVE_BYTE_TO_SEND);
}

void Slave_Complete_Callback(void)
{
  LL_I2C_DisableIT_EVT(I2C2);
  LL_I2C_DisableIT_ERR(I2C2);
}


