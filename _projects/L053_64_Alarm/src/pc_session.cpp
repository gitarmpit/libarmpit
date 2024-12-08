#include "pc_session.h"
#include "beep.h"
#include "gpio.h"
#include "low_power.h"
#include <stdio.h>

static uint8_t calculateCRC(const uint8_t* data, int length) {
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
  
  bool isIRQ = false;
  _uart.init(baudRate, isIRQ);
  _uart.startDMARX();
}

void PcSession::SendHandshake() {
  uint32_t cmd     = STM32_CMD_START;
  _uart.sendDMA((uint8_t*)&cmd, sizeof cmd);
}
 
bool PcSession::ReceiveHandshake() {
  uint32_t cmd = 0;
  return (_uart.receiveMsgDMA((uint8_t*)&cmd, sizeof cmd, _wait_cnt) && cmd == STM32_CMD_START);
}

void PcSession::SendAck() {
  uint8_t ack = STM32_ACK;
  _uart.sendDMA(&ack, 1);
}

bool PcSession::ReceiveAck() {
  uint8_t ack;
  bool rc = false;
  if (_uart.receiveMsgDMA(&ack, 1, _wait_cnt)) {
    rc = (ack == STM32_ACK);
  }
  return rc;
}

bool PcSession::SendData(const uint8_t* buf, uint16_t len) {
  _uart.sendDMA(buf, len);
  uint8_t crc = calculateCRC(buf, len);
  _uart.sendDMA(&crc, 1);
  return ReceiveAck();
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
  return _uart.receiveMsgDMA(&cmd, 1, _wait_cnt);
}

bool PcSession::ReceiveTime(STM32_TIME& time) {
  
  bool rc = false;

  if (_uart.receiveMsgDMA((uint8_t*)&time, sizeof time, _wait_cnt)) {
    uint8_t expected_crc;
    if (_uart.receiveMsgDMA(&expected_crc, 1, _wait_cnt)) {
      uint8_t crc = calculateCRC((uint8_t*)&time, sizeof time);
      rc  = (crc == expected_crc);
    }
  }

  if (rc) {
    SendAck();
  }

  return rc;
}

bool PcSession::ReceiveAlarm(STM32_ALARM& a) {
  bool rc = false;

  if (_uart.receiveMsgDMA((uint8_t*)&a, sizeof a, _wait_cnt)) {
    uint8_t expected_crc;
    if (_uart.receiveMsgDMA(&expected_crc, 1, _wait_cnt)) {
      uint8_t crc = calculateCRC((uint8_t*)&a, sizeof a);
      rc  = (crc == expected_crc);
    }
  }

  if (rc) {
    SendAck();
  }

  return rc;
}


bool PcSession::SendTime(STM32_TIME& time) {
  return SendData((const uint8_t*)&time, sizeof time);
}

bool PcSession::SendAlarm(const STM32_ALARM& a, const STM32_ALARM& b) {
  return SendData((const uint8_t*)&a, sizeof a) &&
         SendData((const uint8_t*)&b, sizeof b);
}


bool PcSession::GetTuneNo(uint8_t& tuneNo) {
  return _uart.receiveMsgDMA(&tuneNo, 1, _wait_cnt);
}