#include "UART.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_dma.h"

#include <stdio.h>
#include "low_power.h"


extern "C" void USART1_IRQHandler(void) {
  if (LL_USART_IsActiveFlag_IDLE(USART1)) {
    LL_USART_ClearFlag_IDLE (USART1);
  }
}


extern "C" void DMA1_Channel2_3_IRQHandler(void) {
  if (LL_DMA_IsActiveFlag_TC3(DMA1)) {
    LL_DMA_ClearFlag_TC3(DMA1);
    printf ("TC\n");
  }
}


UART_Comm::UART_Comm(USART_TypeDef* USARTx) : _USARTx(USARTx) {
  deinit();
  _rxBufReadPos = 0;
}

void UART_Comm::init(uint32_t baudRate, bool initIRQ) {

  if (_USARTx == USART1) {
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
    _dmaChannelTX = LL_DMA_CHANNEL_2;
    _dmaChannelRX = LL_DMA_CHANNEL_3;
    _dmaRequest = LL_DMA_REQUEST_3;
  }
  else if (_USARTx == USART2) {
    LL_APB2_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    _dmaChannelTX = LL_DMA_CHANNEL_4;
    _dmaChannelRX = LL_DMA_CHANNEL_5;
    _dmaRequest = LL_DMA_REQUEST_4;
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
  while((!(LL_USART_IsActiveFlag_TEACK(_USARTx))) || (!(LL_USART_IsActiveFlag_REACK(_USARTx)))) { 
  }

  LL_USART_EnableIT_IDLE(_USARTx);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  // TX
  LL_DMA_DisableChannel(DMA1, _dmaChannelTX);
  LL_DMA_SetPeriphRequest(DMA1, _dmaChannelTX, _dmaRequest);
  LL_DMA_SetDataTransferDirection(DMA1, _dmaChannelTX, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  LL_DMA_SetChannelPriorityLevel(DMA1, _dmaChannelTX, LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, _dmaChannelTX, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMA1, _dmaChannelTX, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1, _dmaChannelTX, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1, _dmaChannelTX, LL_DMA_PDATAALIGN_BYTE);
  LL_DMA_SetMemorySize(DMA1, _dmaChannelTX, LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_EnableIT_TC(DMA1, _dmaChannelTX);
  LL_DMA_EnableIT_TE(DMA1, _dmaChannelTX);
  LL_USART_EnableDMAReq_TX(_USARTx);

  // RX
  LL_DMA_DisableChannel(DMA1, _dmaChannelRX);
  LL_DMA_SetPeriphRequest(DMA1, _dmaChannelRX, _dmaRequest);
  LL_DMA_SetDataTransferDirection(DMA1, _dmaChannelRX, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  LL_DMA_SetChannelPriorityLevel(DMA1, _dmaChannelRX, LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, _dmaChannelRX, LL_DMA_MODE_CIRCULAR);
  LL_DMA_SetPeriphIncMode(DMA1, _dmaChannelRX, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1, _dmaChannelRX, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1, _dmaChannelRX, LL_DMA_PDATAALIGN_BYTE);
  LL_DMA_SetMemorySize(DMA1, _dmaChannelRX, LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_EnableIT_TC(DMA1, _dmaChannelRX);
  LL_DMA_EnableIT_TE(DMA1, _dmaChannelRX);
  LL_USART_EnableDMAReq_RX(_USARTx);

  if (_USARTx == USART1 && initIRQ) {
    NVIC_SetPriority(USART1_IRQn, 0);
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0);
    NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
    _isIRQ = true;
  }
  else {
    _isIRQ = false;
  }
}

void UART_Comm::deinit() {
  LL_USART_DeInit(_USARTx);
  LL_USART_Disable(_USARTx);
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

bool UART_Comm::receiveMsg(uint8_t* buf, uint32_t nBytes, uint32_t wait_cnt) {
    for (uint32_t i = 0; i < nBytes; i++) {
        if (!receiveByte(buf[i], wait_cnt)) {
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

bool UART_Comm::sendMsg(uint8_t* buf, uint32_t nBytes) {
    for (uint32_t i = 0; i < nBytes; i++) {
        sendByte(buf[i]);
    }
    return true;
}

void UART_Comm::sendDMA(const uint8_t* buf, uint32_t nBytes) {
  LL_DMA_DisableChannel(DMA1, _dmaChannelTX);

  LL_DMA_ConfigAddresses(
      DMA1, _dmaChannelTX, (uint32_t)buf, (uint32_t) & (_USARTx->TDR), LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetDataLength(DMA1, _dmaChannelTX, nBytes);

  LL_DMA_EnableChannel(DMA1, _dmaChannelTX);

  if (_USARTx == USART1) {
    while (!LL_DMA_IsActiveFlag_TC2(DMA1))
      ;
    LL_DMA_ClearFlag_TC2(DMA1);
  } else if (_USARTx == USART2) {
    while (!LL_DMA_IsActiveFlag_TC4(DMA1))
      ;
    LL_DMA_ClearFlag_TC4(DMA1);
  }
  LL_DMA_DisableChannel(DMA1, _dmaChannelTX);
}

void UART_Comm::startDMARX() {

  LL_DMA_DisableChannel(DMA1, _dmaChannelRX);

  LL_DMA_ConfigAddresses(DMA1, _dmaChannelRX, (uint32_t) &(_USARTx->RDR), (uint32_t) _rxbuf, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  LL_DMA_SetDataLength(DMA1, _dmaChannelRX, DMA_RX_BUFFER_SIZE);
  LL_DMA_EnableChannel(DMA1, _dmaChannelRX);
}

uint16_t UART_Comm::readDMA(uint8_t* buf, uint16_t bytesRequested) {

  uint16_t rxBufWritePos = DMA_RX_BUFFER_SIZE - LL_DMA_GetDataLength(DMA1, _dmaChannelRX);

  uint16_t bytesAvailable = (rxBufWritePos - _rxBufReadPos + DMA_RX_BUFFER_SIZE) % DMA_RX_BUFFER_SIZE;
  uint16_t bytesRead      = 0;
  if (bytesAvailable > 0) {
    bytesRead = (bytesRequested > bytesAvailable) ? bytesAvailable : bytesRequested;
    for (int i = 0; i < bytesRead; i++) {
      buf[i] = _rxbuf[(_rxBufReadPos + i) % DMA_RX_BUFFER_SIZE];
    }
    _rxBufReadPos = (_rxBufReadPos + bytesRead) % DMA_RX_BUFFER_SIZE;
  }

  return bytesRead;
}

bool UART_Comm::receiveMsgDMA(uint8_t* buf, uint16_t nBytes, uint32_t nretries) {

   uint16_t pos = 0;
   while (1) {
      uint16_t nread = readDMA(&buf[pos], nBytes);
      if (nread == nBytes) {
         return true;
      }
      if (--nretries == 0) {
         return false;
      }
      pos += nread;
      nBytes -= nread;
   }
}