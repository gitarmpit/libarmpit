#include "UART.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_bus.h"

void UART_Comm::init(uint32_t baudRate) {

  if (_USARTx == USART1) {
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  }
  else if (_USARTx == USART2) {
    LL_APB2_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  }

  LL_USART_Disable(_USARTx);

  LL_USART_InitTypeDef USART_InitStruct = {0};
  USART_InitStruct.BaudRate             = baudRate;
  USART_InitStruct.DataWidth            = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits             = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity               = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection    = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl  = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling         = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(_USARTx, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(_USARTx);
  LL_USART_Enable(_USARTx);

}

bool UART_Comm::receiveByte(uint8_t& byte, uint32_t wait_cnt) {

    int i = 0;
    while (!LL_USART_IsActiveFlag_RXNE(_USARTx)) {
      if (++i > wait_cnt) {
        return false;
      }
    }

    byte = LL_USART_ReceiveData8(_USARTx); 
    return true;
}

bool UART_Comm::receiveMsg(uint8_t* buffer, uint32_t nBytes, uint32_t wait_cnt) {
    for (uint32_t i = 0; i < nBytes; i++) {
        if (!receiveByte(buffer[i], wait_cnt)) {
          return false;
        }
    }
    return true;
}


void UART_Comm::sendByte(uint8_t byte) {
  while (!LL_USART_IsActiveFlag_TXE(_USARTx))
    ;

  LL_USART_TransmitData8(_USARTx, byte);
}

bool UART_Comm::sendMsg(uint8_t* buffer, uint32_t nBytes) {
    for (uint32_t i = 0; i < nBytes; i++) {
        sendByte(buffer[i]);
    }
    return true;
}



