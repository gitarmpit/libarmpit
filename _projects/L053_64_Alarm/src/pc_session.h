#ifndef _PC_SESSION_H

#define _PC_SESSION_H

#include "UART.h"
#include "rtc.h"

#define STM32_CMD_START      0xdeaffead
#define STM32_ACK  0xaa
#define STM32_CMD_SETTIME    0x1
#define STM32_CMD_GETTIME    0x2


class PcSession {
  public:
    PcSession(USART_TypeDef* USARTx, int baudRate);
     
    bool Handshake();
    bool ReceiveCommand(uint8_t& cmd);
    bool ReceiveTime(STM32_TIME& time);
    bool SendTime(STM32_TIME& time);

  private:
    void SendHandshake();
    bool ReceiveHandshake();
    void SendAck();
    bool ReceiveAck();


    UART_Comm _uart;
    uint32_t _wait_cnt;
};

#endif