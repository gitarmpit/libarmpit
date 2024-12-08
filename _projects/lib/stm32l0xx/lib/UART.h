#ifndef _UART 
#define _UART
#include "ISerialDev.h"

#include "stm32l0xx_ll_usart.h"
#include "cbuf.h"

#define DMA_RX_BUFFER_SIZE 8

class UART_Comm {
public:
  UART_Comm (USART_TypeDef* USARTx);

  void init(uint32_t baudRate, bool initIRQ);

  bool receiveByte(uint8_t& byte, uint32_t wait_cnt);
  bool receiveMsg(uint8_t* buffer, uint32_t nBytes, uint32_t wait_cnt);
  void sendByte(uint8_t byte);
  bool sendMsg(uint8_t* buffer, uint32_t nBytes);
  void deinit();
  void sendDMA(const uint8_t* buffer, uint32_t nBytes);
  void startDMARX();
  uint16_t readDMA(uint8_t* buffer, uint16_t bytesRequested); 
  bool     receiveMsgDMA(uint8_t* buf, uint16_t nBytes, uint32_t nretries);

~UART_Comm() { deinit(); }

private:
  USART_TypeDef* _USARTx;
  uint32_t       _dmaChannelRX;
  uint32_t       _dmaChannelTX;
  uint32_t       _dmaRequest;
  uint8_t        _rxbuf[DMA_RX_BUFFER_SIZE];
  uint16_t       _rxBufReadPos;
  cbuf           _cbuf;
  bool           _isIRQ; // use UART/DMA IRQ or not 
};

// Serial TCP over UART
class SerialDev_UART : public ISerialDev {
public:
  SerialDev_UART(UART_Comm* uart) : _uart (uart) {}
  virtual void Write(const uint8_t* buffer, uint32_t nBytes) {
    _uart->sendDMA(buffer, nBytes);
  }
  virtual uint16_t Read(uint8_t* buffer, uint16_t bytesRequested) {
    return _uart->readDMA(buffer, bytesRequested);
  }

private:
  UART_Comm* _uart;
};

#endif