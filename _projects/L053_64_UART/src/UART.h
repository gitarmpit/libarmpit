#ifndef _UART 
#define _UART

#include "stm32l0xx_ll_usart.h"


class UART_Comm {
public:
  UART_Comm (USART_TypeDef* USARTx) 
  : _USARTx(USARTx) {}

  void init(uint32_t baudRate);

  bool receiveByte(uint8_t& byte, uint32_t wait_cnt);
  bool receiveMsg(uint8_t* buffer, uint32_t nBytes, uint32_t wait_cnt);
  void sendByte(uint8_t byte);
  bool sendMsg(uint8_t* buffer, uint32_t nBytes);

private:
  USART_TypeDef* _USARTx;
};

#endif