#include "UART.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_dma.h"

#include <stdio.h>
#include "low_power.h"

void UART_Comm::init(uint32_t baudRate) {

  if (_USARTx == USART1) {
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
    _dmaChannelTX = LL_DMA_CHANNEL_2;
    _dmaChannelRX = LL_DMA_CHANNEL_3;
  }
  else if (_USARTx == USART2) {
    LL_APB2_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    _dmaChannelTX = LL_DMA_CHANNEL_4;
    _dmaChannelRX = LL_DMA_CHANNEL_5;
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

}

static void init() {

  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2); // the default
  LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);
  LL_USART_ConfigCharacter(USART1, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

  // the default values on reset
  // LL_USART_SetHWFlowCtrl(USART1, LL_USART_HWCONTROL_NONE);
  // LL_USART_SetOverSampling(USART1, LL_USART_OVERSAMPLING_16);
  LL_RCC_GetUSARTClockFreq(LL_RCC_USART1_CLKSOURCE);
  LL_USART_SetBaudRate(USART1, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200); 
  LL_USART_Enable(USART1);
  while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1)))) { 
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
        delay_ms(20);
    }
    return true;
}

void UART_Comm::startDMATX(uint8_t* buffer, uint32_t nBytes, bool wait) {

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  LL_DMA_DisableChannel(DMA1, _dmaChannelTX);

  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMA_REQUEST_3);
  LL_DMA_SetDataTransferDirection(DMA1, _dmaChannelTX, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  LL_DMA_SetChannelPriorityLevel(DMA1, _dmaChannelTX, LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, _dmaChannelTX, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMA1, _dmaChannelTX, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1, _dmaChannelTX, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1, _dmaChannelTX, LL_DMA_PDATAALIGN_BYTE);
  LL_DMA_SetMemorySize(DMA1, _dmaChannelTX, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_ConfigAddresses(DMA1, _dmaChannelTX, (uint32_t) buffer, (uint32_t) &(_USARTx->TDR), LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  LL_DMA_SetDataLength(DMA1, _dmaChannelTX, nBytes);

  LL_DMA_EnableIT_TC(DMA1, _dmaChannelTX);
  LL_DMA_EnableIT_TE(DMA1, _dmaChannelTX);
  LL_USART_EnableDMAReq_TX(_USARTx);

  LL_DMA_EnableChannel(DMA1, _dmaChannelTX);

  if (wait) {
    if (_USARTx == USART1) {
      while (!LL_DMA_IsActiveFlag_TC2(DMA1))
        ;
      LL_DMA_ClearFlag_TC2(DMA1);
    }
    else if (_USARTx == USART2) {
      while (!LL_DMA_IsActiveFlag_TC4(DMA1))
        ;
      LL_DMA_ClearFlag_TC4(DMA1);
    }
  }

}

void UART_Comm::startDMARX(uint8_t* buffer, uint32_t nBytes) {

  LL_DMA_EnableChannel(DMA1, _dmaChannelRX);
}




