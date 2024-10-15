#include "gpio.h"

#include "UART.h"
#include <stdio.h>
#include "beep.h"
#include "low_power.h"
#include "pc_session.h"


bool RunSession() {

  int baudRate = 1200;
  PcSession session (USART1, baudRate);

  bool rc = session.Handshake();

  if (!rc) {
    beep_failure();
    return false;
  }

  uint8_t cmd;
  if (!session.ReceiveCommand(cmd)) {
    beep_failure();
    return false;
  }

  rc = false;
  if (cmd == STM32_CMD_SETTIME) {
    STM32_TIME t;
    if (session.ReceiveTime(t)) {
      setRTCTime(&t);
      rc = true;
    }
  } else if (cmd == STM32_CMD_GETTIME) {
    STM32_TIME t;
    getRTCTime(&t);
    for (int i = 0; i < 10; ++i) {
      rc = session.SendTime(t);
      if (rc) {
        break;
      }
    }
  }
  else if (cmd == STM32_CMD_GETALARM) {
    STM32_ALARM alarmA;
    getAlarmA(&alarmA);
    STM32_ALARM alarmB;
    getAlarmB(&alarmB);
    rc = session.SendAlarm(alarmA);
    if (rc) {
      session.SendAlarm(alarmB);
    }

  }
  else if (cmd == STM32_CMD_SETALARM) {
    STM32_ALARM a;
    if (session.ReceiveAlarm(a)) {
      if (a.alarmNo == 0) {
        setAlarmA(&a);
      }
      else {
        setAlarmB(&a);
      }
      rc = true;
    }
  }

  if (rc) {
    beep_success();
  } else {
    beep_failure();
  }

  return rc;
}

void testRX() {
  LL_USART_Disable(USART1);
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);
  uart.init(1200);
  uint8_t b;
  while(1) {
    if (uart.receiveByte(b, 5000)) {
      printf ("%d\n", b);
    }
  }
}

void testTX(int baudRate) {
  LL_USART_Disable(USART1);
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);
  uart.init(baudRate);
  uint8_t b = 0x4f;
  while(1) {
    uart.sendByte(b);
    //uart.sendByte(b & 0x7f);
    delay_ms(10);
    //++b;
  }
}


#if 0
bool testReceiveTime2() {
 
  LL_USART_Disable(USART1);

  beep(1000, 10);

  /*
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  */

  // A9, AF4
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);
  uart.init(1200);

  uint8_t buf[256] = {0};

  /*
  uint8_t b = 0;
  for (int i = 0; i < 10; ++i) {
    buf[i] = i+1;
  }

  for (int i = 0; i < 10; ++i) {
    uart.sendMsg (buf, 10);
    for (int x = 0; x < 2000; x++) {
    }
  }

  return true;
  */

  uint32_t wait_cnt = 20000;
  int n             = 254;
  int i             = 0;

  for (int i = 0; i < n; ++i) {
    buf[i] = i + 1;
  }

  uint8_t crc = calculateCRC(buf, n);
  uart.sendMsg(buf, n);
  uart.sendByte(crc);

  if (uart.receiveMsg(buf, n, wait_cnt)) {
    uint8_t expected_crc;
    if (uart.receiveByte(expected_crc, wait_cnt)) {
      uint8_t crc = calculateCRC(buf, n);
      if (crc == expected_crc) {
        return true;
      }
    }
  }

  return false;

  /*
  while (true) {
    for (int i = 0; i < n; ++i) {
      buf[i] = i + 1;
    }
    uint8_t crc = calculateCRC(buf, n);
    uart.sendMsg(buf, n);
    printf ("crc: %x\n", crc);
    uart.sendByte(crc);

    if (uart.receiveMsg(buf, n, wait_cnt)) {

      printf ("recv ok\n");
      uint8_t expected_crc;
      if (uart.receiveByte(expected_crc, wait_cnt)) {
        uint8_t crc = calculateCRC(buf, n);
        if (crc == expected_crc) {
          printf ("crc ok\n");
        }
        else {
          printf ("crc error\n");
        }
      }
    }
    else {
      printf ("recv error\n");
    }

    for (int i = 0; i < 100; ++i) {
    }
  }
  */

  /*
  while (true) {
    memset (buf, 0, n);
    if (uart.receiveMsg(buf, n, wait_cnt)) {

      uint8_t expected_crc;
      if (uart.receiveByte(expected_crc, wait_cnt)) {
        //printf ("%d: exp crc: %x\n", ++i, expected_crc);
        uint8_t crc = calculateCRC(buf, n);
        //printf ("crc: %x\n", crc);
        if (crc == expected_crc) {
          buzzer->PlayTone(2000, 100);
          buzzer->Stop();
        }
        else {
          buzzer->PlayTone(100, 100);
          buzzer->Stop();
        }
      }
    }
  }
  */

  uint32_t cmd      = STM32_CMD_START;
  bool cmdReceived  = false;
  for (int i = 0; i < 10; ++i) {
    cmd = STM32_CMD_START;
    beep(2000, 20);
    uart.sendMsg((uint8_t*)&cmd, sizeof cmd);
    cmd = 0;
    if (uart.receiveMsg((uint8_t*)&cmd, sizeof cmd, wait_cnt)) {
      printf ("cmd: %x\n", cmd);
    }
    printf("retrying\n");
    for (int i = 0; i < 5000; i++) {
    }
  }

  return cmdReceived;

  // Expect CRC
  uint8_t expectedCRC;
  if (!uart.receiveByte(expectedCRC, wait_cnt)) {
    printf("crc: receive timed out\n");
    return false;
  }

  if (!uart.receiveMsg(buf, sizeof(STM32_TIME), wait_cnt)) {
    printf("receiveMsg: receive timed out\n");
    return false;
  }

  STM32_TIME* t = (STM32_TIME*)buf;
  crc   = calculateCRC(buf, sizeof(STM32_TIME));

  if (crc != expectedCRC) {
    printf("CRC error\n");
    return false;
  }

  printf("%d-%d-%d  %d:%d:%d.%d, dow: %d\n", t->year, t->month, t->day, t->hour, t->minute, t->second, t->ms, t->dow);

  printf("\n===========================\n");
  return true;
}

#endif