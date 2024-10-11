#include "pc_session.h"
#include "beep.h"
#include "gpio.h"
#include "low_power.h"
#include <stdio.h>

static uint8_t calculateCRC(uint8_t* data, int length) {
  uint8_t crc              = 0x00;
  const uint8_t polynomial = 0x31;

  for (int i = 0; i < length; i++) {
    crc ^= data[i];

    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ polynomial;
      } else {
        crc <<= 1;
      }
    }
  }

  return crc;
}


PcSession::PcSession(USART_TypeDef* USARTx, int baudRate) : _uart(USARTx) {

  _wait_cnt = 10000;
  LL_USART_Disable(USARTx);

  // A9, AF4
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  _uart.init(baudRate);
}

void PcSession::SendHandshake() {
  uint32_t cmd     = STM32_CMD_START;
  _uart.sendMsg((uint8_t*)&cmd, sizeof cmd);
}
 
bool PcSession::ReceiveHandshake() {
  uint32_t cmd = 0;
  return (_uart.receiveMsg((uint8_t*)&cmd, sizeof cmd, _wait_cnt) && cmd == STM32_CMD_START);
}

void PcSession::SendAck() {
  uint8_t ack = STM32_ACK;
  _uart.sendByte(ack);
}

bool PcSession::ReceiveAck() {
  uint8_t ack;
  bool rc = false;
  if (_uart.receiveByte(ack, _wait_cnt)) {
    rc = (ack == STM32_ACK);
  }
  return rc;
}

bool PcSession::Handshake() {

  bool cmdReceived = false;
  for (int i = 0; i < 5; ++i) {
    beep(2000, 20);
    SendHandshake();
    if (ReceiveHandshake()) {
      SendAck();
      cmdReceived = true;
      break;
    }

    delay_ms(2000);
  }
  return cmdReceived;
}

bool PcSession::ReceiveCommand(uint8_t& cmd) {
  return _uart.receiveByte(cmd, _wait_cnt);
}

bool PcSession::ReceiveTime(STM32_TIME& time) {
  
  bool rc = false;

  if (_uart.receiveMsg((uint8_t*)&time, sizeof time, _wait_cnt)) {
    uint8_t expected_crc;
    if (_uart.receiveByte(expected_crc, _wait_cnt)) {
      uint8_t crc = calculateCRC((uint8_t*)&time, sizeof time);
      rc  = (crc == expected_crc);
    }
  }

  if (rc) {
    SendAck();
  }

  return rc;
}


bool PcSession::SendTime(STM32_TIME& time) {
  _uart.sendMsg((uint8_t*)&time, sizeof time);
  uint8_t crc = calculateCRC((uint8_t*)&time, sizeof time);
  _uart.sendByte(crc);
  return ReceiveAck();
}