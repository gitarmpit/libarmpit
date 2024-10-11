#include "gpio.h"

#include <stdio.h>
#include "UART.h"

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


bool testReceiveTime() {

  //A9, AF4
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx);
  GPIO_SetAF(&tx, 4);

  //A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx);
  GPIO_SetAF(&rx, 4);


  UART_Comm uart(USART1);
  uart.init(1200);

  uint8_t buf[256] = {0};

  uint32_t cmd = STM32_CMD_START;

  uint32_t wait_cnt = 2000;

  bool cmdReceived = false;
  for (int i = 0; i < 10; ++i) {
    uart.sendMsg ((uint8_t*)&cmd, sizeof cmd);
    cmd = 0;
    if (uart.receiveMsg((uint8_t*)&cmd, sizeof cmd, wait_cnt) && cmd == STM32_CMD_START) {
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
  if (!uart.receiveByte(expectedCRC, wait_cnt)) {
    printf ("crc: receive timed out\n");
    return false;
  }

  if (!uart.receiveMsg (buf, sizeof(STM32_TIME), wait_cnt)) {
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





