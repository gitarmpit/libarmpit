#ifndef _PC_SESSION_H

#define _PC_SESSION_H

#include "UART.h"
#include "rtc.h"

#define STM32_CMD_START      0xdeaffead
#define STM32_ACK  0xaa
#define STM32_CMD_SETTIME    0x1
#define STM32_CMD_GETTIME    0x2
#define STM32_CMD_SETALARM   0x3
#define STM32_CMD_GETALARM   0x4
#define STM32_CMD_PLAYTUNE   0x5



class PcSession {
  public:
    PcSession(USART_TypeDef* USARTx, int baudRate);
     
    bool Handshake();
    bool ReceiveCommand(uint8_t& cmd);
    bool ReceiveTime(STM32_TIME& time);
    bool SendTime(STM32_TIME& time);
    bool ReceiveAlarm(STM32_ALARM& a);
    bool SendAlarm(const STM32_ALARM& a, const STM32_ALARM& b);
    bool GetTuneNo(uint8_t& tuneNo);

  private:
    void SendHandshake();
    bool ReceiveHandshake();
    void SendAck();
    bool ReceiveAck();
    // Send data, send crc, receive ack: crc ok
    bool SendData(const uint8_t* buf, uint16_t len);


    UART_Comm _uart;
    uint32_t _wait_cnt;
};

#endif