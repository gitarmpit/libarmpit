
#include "config.h"
#include "gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_usart.h"
#include "systick.h"
#include <stdio.h>


#define STM32_CMD_START 0xfe

typedef struct {

	uint16_t year; 
	uint8_t month;
	uint8_t day;
	uint8_t dow;

	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t ms;

} STM32_TIME;

static uint8_t calculateCRC(uint8_t* data, uint8_t length) {
	uint8_t crc = 0x00; 
	const uint8_t polynomial = 0x31;

	for (uint8_t i = 0; i < length; i++) {
		crc ^= data[i]; 

		for (uint8_t j = 0; j < 8; j++) {
			if (crc & 0x80) {
				crc = (crc << 1) ^ polynomial; 
			}
			else {
				crc <<= 1; 
			}
		}
	}

	return crc;
}



static void initUART1() {
  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin        = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin  = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.BaudRate            = 9600;
  USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity              = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
}

static void initUART2() {
  //
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);

  LL_GPIO_AF_EnableRemap_USART1();

  GPIO_PIN tx = GPIO_GetPin("B6");
  GPIO_Setup_OutAltPP(&tx);

  GPIO_PIN rx = GPIO_GetPin("B7");
  GPIO_Setup_Floating(&rx);

  LL_USART_InitTypeDef USART_InitStruct = {0};
  USART_InitStruct.BaudRate             = 9600;
  USART_InitStruct.DataWidth            = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits             = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity               = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection    = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl  = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling         = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
}

static void initUART3() {
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx);

  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_Floating(&rx);

  LL_USART_InitTypeDef USART_InitStruct = {0};
  USART_InitStruct.BaudRate             = 9600;
  USART_InitStruct.DataWidth            = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits             = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity               = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection    = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl  = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling         = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
}

static void initUART4() {
  //
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);

  LL_GPIO_AF_EnableRemap_USART1();

  GPIO_PIN tx = GPIO_GetPin("B6");
  GPIO_Setup_OutAltPP(&tx);

  GPIO_PIN rx = GPIO_GetPin("B7");
  GPIO_Setup_Floating(&rx);

  LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);
  LL_USART_ConfigCharacter(USART1, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
  LL_USART_ConfigAsyncMode(USART1);
  int APB_Div = 1;
  LL_USART_SetBaudRate(USART1, SystemCoreClock/APB_Div, 9600);
  LL_USART_Enable(USART1);
}



static bool receiveByte(uint8_t& byte) {

    int i = 0;
    while (!LL_USART_IsActiveFlag_RXNE(USART1)) {
      if (++i > 100000) {
        return false;
      }
    }

    byte = LL_USART_ReceiveData8(USART1); 
    return true;
}

static bool receiveMsg(uint8_t* buffer, uint32_t nBytes) {
    for (uint32_t i = 0; i < nBytes; i++) {
        if (!receiveByte(buffer[i])) {
          return false;
        }
    }
    return true;
}

static void testReceiveTime() {
  uint8_t buf[256] = {0};

  // Expect the magic byte
  while (!receiveByte(buf[0])) 
    ;

  printf ("received magic byte: %d\n", buf[0]);
  if (buf[0] != STM32_CMD_START) {
    printf ("unexpected value\n");
    return;
  }

  // Send magic byte back 
  while (!LL_USART_IsActiveFlag_TXE(USART1))
    ;

  LL_USART_TransmitData8(USART1, STM32_CMD_START);

  printf ("Sent magic byte back\n");

  // Expect msg length 
  uint8_t len;
  
  if (!receiveByte(len)) {
    printf ("len: receive timed out\n");
    return;
  }

  printf ("received msg length: %d\n", len);
  if (len != sizeof (STM32_TIME)) {
    printf ("unexpected value\n");
    return;
  }

  // Expect CRC
  uint8_t expectedCRC;
  if (!receiveByte(expectedCRC)) {
    printf ("crc: receive timed out\n");
    return;
  }

  printf ("expected CRC: %x\n", expectedCRC);

  if (!receiveMsg (buf, len)) {
    printf ("receiveMsg: receive timed out\n");
    return;
  }

  STM32_TIME* t = (STM32_TIME*)buf; 
  uint8_t crc = calculateCRC(buf, len);
  printf ("calculated CRC: %x\n", crc);

  if (crc != expectedCRC) {
    printf ("CRC error\n");
    return;
  }

  printf ("%d-%d-%d  %d:%d:%d.%d, dow: %d\n", 
        t->year, t->month, t->day, 
        t->hour, t->minute, t->second, 
        t->ms, t->dow);

  printf ("\n===========================\n");

}

static void testRx() {
  uint8_t b = 0;
  while (1) {
    while (!LL_USART_IsActiveFlag_RXNE(USART1))
      ;

    b = LL_USART_ReceiveData8(USART1);
  }
}

static void testTx() {
  uint8_t b = 0;
  while (1) {
    while (!LL_USART_IsActiveFlag_TXE(USART1))
      ;

    LL_USART_TransmitData8(USART1, b++);
    SysTick_Delay(1000);
  }
}

int main(void) {

  SystemClock_Config_HSE();
  SystemCoreClockUpdate();

  SysTick_Init();
  initUART4();
  //testTx();

  while (1) {
    testReceiveTime();
  }

  while (1)
    ;
}
