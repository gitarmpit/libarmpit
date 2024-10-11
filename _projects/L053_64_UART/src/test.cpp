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

#define STM32_CMD_START   0xdeaffead
#define STM32_CMD_END     0xbeefbeef
#define STM32_CRC_ERROR   0xdeaddead


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

static uint8_t calculateCRC(uint8_t* data, int length) {
    uint8_t crc = 0x00; 
    const uint8_t polynomial = 0x31;

    for (int i = 0; i < length; i++) {
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


static void initUART(int baudRate) {
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  LL_USART_Disable(USART1);

  //A9, AF4
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx);
  GPIO_SetAF(&tx, 4);

  //A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx);
  GPIO_SetAF(&rx, 4);

  LL_USART_InitTypeDef USART_InitStruct = {0};
  USART_InitStruct.BaudRate             = baudRate;
  USART_InitStruct.DataWidth            = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits             = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity               = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection    = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl  = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling         = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  uint32_t clk = LL_RCC_GetUSARTClockFreq(LL_RCC_USART1_CLKSOURCE);
  uint32_t br = LL_USART_GetBaudRate (USART1, clk, LL_USART_OVERSAMPLING_16);
}

static int wait_cnt = 1000;

static bool receiveByte(uint8_t& byte) {

    int i = 0;
    while (!LL_USART_IsActiveFlag_RXNE(USART1)) {
      if (++i > wait_cnt) {
        return false;
      }
    }

    //printf ("recv: number of retries: %d\n", i);

    byte = LL_USART_ReceiveData8(USART1); 
    return true;
}


static void sendByte(uint8_t byte) {
  while (!LL_USART_IsActiveFlag_TXE(USART1))
    ;

  LL_LPUART_TransmitData8(USART1, byte);
}

static bool sendMsg(uint8_t* buffer, uint32_t nBytes) {
    for (uint32_t i = 0; i < nBytes; i++) {
        sendByte(buffer[i]);
    }
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

static bool testReceiveTime() {
  uint8_t buf[256] = {0};

  uint32_t cmd = STM32_CMD_START;

  bool cmdReceived = false;
  for (int i = 0; i < 10; ++i) {
    sendMsg ((uint8_t*)&cmd, sizeof cmd);
    cmd = 0;
    if (receiveMsg((uint8_t*)&cmd, sizeof cmd) && cmd == STM32_CMD_START) {
      printf ("handshake complete\n");
      cmdReceived = true;
      break;
    }
    printf ("retrying\n");
    for (int i = 0; i < 5000; i++) {
    }
  }

  if (!cmdReceived) {
    printf ("timeout waiting for handshake\n");
    return false;
  }

  return true;

  // Expect CRC
  uint8_t expectedCRC;
  if (!receiveByte(expectedCRC)) {
    printf ("crc: receive timed out\n");
    return false;
  }

  if (!receiveMsg (buf, sizeof(STM32_TIME))) {
    printf ("receiveMsg: receive timed out\n");
    return false;
  }

  STM32_TIME* t = (STM32_TIME*)buf; 
  uint8_t crc = calculateCRC(buf, sizeof(STM32_TIME));

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
static void lowPowerRun(uint32_t range) {

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
    ;

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1); // 1.2V

  while (LL_PWR_IsActiveFlag_VOS() != 0)
    ;

  LL_RCC_MSI_Enable();

  while(LL_RCC_MSI_IsReady() != 1)
    ;

  LL_RCC_MSI_SetRange(range); // 2% errors with 1200 baud

  //LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_1); // 0% errors with 2400 baud

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



static void testSend(void) {
  uint8_t buf[20];
  int i = 0;

  uint32_t cmd = STM32_CMD_START;
  while(1) {
    sendMsg((uint8_t*)&cmd, 4);
    printf ("sending: packet %d\n", i);
    for (int i = 0; i < sizeof buf; ++i) {
      buf[i] = i;
    }
    uint8_t crc = calculateCRC(buf, sizeof buf);
    printf ("crc: %x\n", crc);
    sendByte(crc);
    sendMsg (buf, sizeof(buf));
    for (int i = 0; i < 20; ++i) {
    }
  }
}

static void testSend2(void) {
  uint8_t b = 0;
  while(1) {
    printf ("sending: %d\n", b);
    sendByte(b++);
    for (int i = 0; i < 500; ++i) {
    }
    //SysTick_Delay(1);
  }
}


// Don't print too much: it will slow things down and RX will get flooded if PC is sending a lot of data
// RXNE will remain 0, so watch out for BUSY And ORE (overrun) flags: that's a sign that RX is swamped and can't keep it
// Especially in low power run mode!
// And once a bp is hit, that's it, BUSY/ORE guaranteed, abort debugging
static void testReceive(void) {

  int current = -1;
  uint8_t b;
  int total   = 0;
  int nerrors = 0;

  while (true) {
    if (receiveByte(b)) {
      if (current == -1) {
        current = b;
        continue;
      }
      ++current;
      ++total;
      if ((current % 256) != b) {
        ++nerrors;
        //printf("total: %d, errors: %d, %%: %f\n", total, nerrors, (float)nerrors / (float)total * 100.0);
      }
      //printf ("%d\n", b);
    }
    if (total % 20 == 0) {
      //printf("total: %d, errors: %d, %%: %f\n", total, nerrors, (float)nerrors / (float)total * 100.0);
        printf ("%d\n", b);
    }
  }
}

int main2(void) {
  System_Config();

  //SystemClock_Config_MSI();
  //SystemClock_Config_HSI_32();
  SystemCoreClockUpdate();
  lowPowerRun(LL_RCC_MSIRANGE_0);

  //SysTick_Init();
  initUART(1200);
  //testSend2();
  //testReceive();

  bool rc = testReceiveTime();
  while(1);

  while (1) {
    bool rc = testReceiveTime();
    if (!rc) {
      printf ("error, re-initializing USART\n");
      initUART(600);
    }
  }

  while(1)
    ;
}


