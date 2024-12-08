#include "gpio.h"

#include "UART.h"
#include <cstdlib>
#include <stdio.h>
#include "beep.h"
#include "low_power.h"
#include "pc_session.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_dma.h"
#include "systick.h"
#include "MSLP.h"

void testRecvDMA() {
  int baudRate = 1200;

  // A9, AF4
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);

  bool isIRQ = true;
  uart.init(baudRate, isIRQ);
  uart.startDMARX();

  uint8_t buf[32];
  while (1) {
    // if (uart.readDMA(buf, 1)) {
    //   printf ("recv: %d\n", buf[0]);
    // }
    
    //if (LL_DMA_IsActiveFlag_TC3(DMA1)) {
    //  LL_DMA_ClearFlag_TC3(DMA1);
    //  printf ("TC\n");
    //}

    delay_ms(1);
  }
}

void testMSLP_TX() {
  int baudRate = 1200;

  // A9, AF4
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);

  bool isIRQ = false;
  uart.init(baudRate, isIRQ);
  uart.startDMARX();

  const int ds = 16;
  uint8_t data[ds];
  for (int i = 0; i < ds; ++i) {
    data[i] = i+1;
  }  

  data[1] = 0x7E;
  data[2] = 0x7D;
  data[3] = 0x7F;
  
  srand (READ_REG(*((uint32_t *)UID_BASE)));
  uint16_t seq =(uint16_t) rand();

  SerialDev_UART comm (&uart);
  MSLP mslp (&comm, seq);

  //mslp.Connect();
  mslp.SendData(data, sizeof data);

  //mslp.SendData(data, 1);

  while(1)
    ;

}

/*
void testHDLC_TX() {
  int baudRate = 1200;

  // A9, AF4
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);

  uart.init(baudRate);
  uart.startDMARX();

  uint8_t data[DATA_SIZE];
  for (int i = 0; i < DATA_SIZE; ++i) {
    data[i] = i+1;
  }  

  data[1] = 0x7E;
  data[2] = 0x7D;
  data[3] = 0x7F;
  
  srand (READ_REG(*((uint32_t *)UID_BASE)));
  uint16_t seq =(uint16_t) rand();

  UART_IComm comm (&uart);
  HDLC hdlc (&comm, seq);

  hdlc.SendSYN(seq);
  hdlc.SendData(data, sizeof data);

  hdlc.SendData(data, 1);

  while(1)
    ;
}

void testHDLC_RX() {
  int baudRate = 1200;

  // A9, AF4
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);

  uart.init(baudRate);
  uart.startDMARX();

  uint8_t data[DATA_SIZE];

  srand (READ_REG(*((uint32_t *)UID_BASE)));
  uint16_t seq =(uint16_t) rand();

  UART_IComm comm (&uart);
  HDLC hdlc (&comm, seq);

  //srand (READ_REG(*((uint32_t *)UID_BASE)));
  //printf ("%d\n", RAND_MAX);

  //while(1) {
  //  printf ("%d\n", rand());
  //  delay_ms(1000);
  //}

  while (1) {
    uint16_t dataSize;
    if (hdlc.ReceiveData(data, dataSize)) {
      if (dataSize) {
        for (int i = 0; i < dataSize; ++i) {
          printf("%x ", data[i]);
        }
        printf("\n");
        //delay_ms(10);
      }
    }
    else {
      printf ("receivedData failed\n");
      break;
    }
  }


  while(1)
    ;
}
*/


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
    rc = session.SendAlarm(alarmA, alarmB);

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
  else if (cmd == STM32_CMD_PLAYTUNE) {
    uint8_t tuneNo;
    rc = session.GetTuneNo(tuneNo);
    if (rc) {
      beep_alarm(tuneNo);
      return true;
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
  bool isIRQ = false;
  uart.init(1200, isIRQ);
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
  bool isIRQ = false;
  uart.init(baudRate, isIRQ);
  uint8_t b = 0x4f;
  while(1) {
    uart.sendByte(b);
    //uart.sendByte(b & 0x7f);
    delay_ms(1000);
    ++b;
  }
}

void testDMA_TX(int baudRate) {
  LL_USART_Disable(USART1);
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);
  bool isIRQ = false;
  uart.init(baudRate, isIRQ);

  uint8_t buf[256];
  for (int i = 0; i < 256; ++i) {
    buf[i] = i;
  }
  uart.sendDMA(buf, sizeof buf);
  uint8_t buf2[] = { 11, 12, 13, 14, 15, 16};
  uart.sendDMA(buf2, sizeof buf2);
  while(1)
    ;

}

void testDMA_RX(int baudRate) {
  LL_USART_Disable(USART1);
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);
  bool isIRQ = false;
  uart.init(baudRate, isIRQ);

  const uint8_t len = 8;
  uint8_t buf[len] = {0xff};
  uart.startDMARX();
  int cnt = 0;
  while (1) {
    bool rc = uart.receiveMsgDMA(buf, len, 40000);
    if (rc) {
      for (int i = 0; i < len; ++i) {
        printf ("%d ", buf[i]);
        if (buf[i] != (cnt % 256)) {
          //printf ("expected: %d, got: %d, cnt: %d\n", cnt % 256, buf[i], cnt);
          //while(1);
        }
        ++cnt;
      }
      printf ("\n");
      //SysTick_Delay(4);
    }
    else {
      printf ("read timed out\n");
    }
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