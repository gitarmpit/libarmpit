#include "config.h"
#include "gpio.h"
#include "stm32l0xx_ll_rcc.h"

#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_lpuart.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_bus.h"
#include <stdio.h>
#include "systick.h"

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

static void MX_LPUART1_UART_Init(void)
{
  LL_LPUART_InitTypeDef LPUART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPUART1);

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  /**LPUART1 GPIO Configuration
  PB10   ------> LPUART1_TX
  PB11   ------> LPUART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  LPUART_InitStruct.BaudRate = 300;
  LPUART_InitStruct.DataWidth = LL_LPUART_DATAWIDTH_8B;
  LPUART_InitStruct.StopBits = LL_LPUART_STOPBITS_1;
  LPUART_InitStruct.Parity = LL_LPUART_PARITY_NONE;
  LPUART_InitStruct.TransferDirection = LL_LPUART_DIRECTION_TX_RX;
  LPUART_InitStruct.HardwareFlowControl = LL_LPUART_HWCONTROL_NONE;
  LL_LPUART_Init(LPUART1, &LPUART_InitStruct);
  LL_LPUART_Enable(LPUART1);

}


static void initUART() {
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  LL_USART_Disable(USART1);

  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx);
  GPIO_SetAF(&tx, 4);

  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx);
  GPIO_SetAF(&rx, 4);

  LL_USART_InitTypeDef USART_InitStruct = {0};
  USART_InitStruct.BaudRate             = 1200;
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

static void initUART2(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx);
  GPIO_SetAF(&tx, 4);

  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx);
  GPIO_SetAF(&rx, 4);

  LL_USART_InitTypeDef USART_InitStruct = {0};
  USART_InitStruct.BaudRate = 9600;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);

}


static int wait_cnt =  100000;

static bool receiveByte(uint8_t& byte) {

    int i = 0;
    while (!LL_LPUART_IsActiveFlag_RXNE(LPUART1)) {
      if (++i > wait_cnt) {
        return false;
      }
    }

    //printf ("recv: number of retries: %d\n", i);

    byte = LL_LPUART_ReceiveData8(LPUART1); 
    return true;
}

static void sendByte(uint8_t byte) {
  while (!LL_LPUART_IsActiveFlag_TXE(LPUART1))
    ;

  LL_LPUART_TransmitData8(LPUART1, byte);
}

static bool receiveMsg(uint8_t* buffer, uint32_t nBytes) {
    for (uint32_t i = 0; i < nBytes; i++) {
        if (!receiveByte(buffer[i])) {
          return false;
        }
    }
    return true;
}

static bool testReceiveTime() {
  uint8_t buf[256] = {0};

  // Expect the magic byte
  while (!receiveByte(buf[0])) 
    ;

  //printf ("received magic byte: %d\n", buf[0]);
  if (buf[0] != STM32_CMD_START) {
    printf ("unexpected value of start: %x\n", buf[0]);
    return false;
  }


  // Send magic byte back 
  sendByte(STM32_CMD_START);

  //printf ("Sent magic byte back\n");

  // Expect msg length 
  uint8_t len;
  
  if (!receiveByte(len)) {
    printf ("len: receive timed out\n");
    return false;
  }

  //printf ("received msg length: %d\n", len);
  if (len != sizeof (STM32_TIME)) {
    printf ("unexpected value of len: %d\n", len);
    return false;
  }

  // Expect CRC
  uint8_t expectedCRC;
  if (!receiveByte(expectedCRC)) {
    printf ("crc: receive timed out\n");
    return false;
  }

  //printf ("expected CRC: %x\n", expectedCRC);

  if (!receiveMsg (buf, len)) {
    printf ("receiveMsg: receive timed out\n");
    return false;
  }

  STM32_TIME* t = (STM32_TIME*)buf; 
  uint8_t crc = calculateCRC(buf, len);
  //printf ("calculated CRC: %x\n", crc);

  if (crc != expectedCRC) {
    printf ("CRC error\n");
    return false;
  }

  printf ("%d-%d-%d  %d:%d:%d.%d, dow: %d\n", 
        t->year, t->month, t->day, 
        t->hour, t->minute, t->second, 
        t->ms, t->dow);

  printf ("\n===========================\n");
  return true;

}

// 54uA  L053C8T6 48-pin
static void lowPowerRun() {

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
    ;

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1); // 1.2V

  while (LL_PWR_IsActiveFlag_VOS() != 0)
    ;

  LL_RCC_MSI_Enable();

  while(LL_RCC_MSI_IsReady() != 1)
    ;

  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_0); // 64 KHz
  LL_RCC_MSI_SetCalibTrimming(0);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
    ;


  PWR->CR |= PWR_CR_LPSDSR;  // Low-power deepsleep/Sleep/Low-power run
  PWR->CR |= PWR_CR_LPRUN; // Low-power run mode
  PWR->CR &= ~PWR_CR_PVDE;   // Turn PVD off (is it off by default?) 

  SystemCoreClockUpdate();
}

static void testRecv(void) {
  uint8_t b = 0;
  while(1) {
    bool rc = receiveByte(b);
    if (rc)
      printf ("b: %x\n", b);
  }
}

static void testSend(void) {
  uint8_t b = 0;
  while(1) {
    printf ("sending: %d\n", b);
    sendByte(b++);
    for (int i = 0; i < 5000; ++i) {
    }
  }
}


int main(void) {
  System_Config();

  SystemCoreClockUpdate();
  lowPowerRun();

  //SysTick_Init();
  //initUART();
  MX_LPUART1_UART_Init();
  //testRecv();
  testSend();

  
  while (1) {
    bool rc = testReceiveTime();
    if (!rc) {
      printf ("error, re-initializing USART\n");
      initUART();
    }
  }

  while(1)
    ;
}


