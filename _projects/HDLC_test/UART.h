#pragma once 
#include <stdint.h> 
#include <windows.h>
#include <stdexcept>
#include "ISerialDev.h"

class UARTException : public std::runtime_error {
public:
    explicit UARTException(const std::string& message)
        : std::runtime_error(message) {}
};

class UART
{
public:
    UART();
    bool      Initialize(const char* port, int baudRate);
    uint32_t  WaitRead(int timeoutMs);
    bool      Read(uint8_t* buf, DWORD bytesToRead, int timeoutMs);
    bool      Write(const uint8_t* buf, DWORD length);
    bool      SetTimeout(int timeoutMs);
    uint32_t  Read(uint8_t* buf, DWORD bytesToRead);
    ~UART() { if (_hSerial != NULL && _hSerial != INVALID_HANDLE_VALUE) { CloseHandle(_hSerial); } }
private:
    bool _initialized;
    HANDLE    _hSerial;
};

// Serial TCP over UART
class SerialDev_UART : public ISerialDev {
public:
  SerialDev_UART(UART* uart) : _uart (uart) {}
  virtual void Write(const uint8_t* buffer, uint32_t nBytes) {
    _uart->Write(buffer, nBytes);
  }
  virtual uint16_t Read(uint8_t* buffer, uint16_t bytesRequested) {
      uint32_t rxBytes = _uart->WaitRead(100);
      if (rxBytes) {
          return _uart->Read(buffer, rxBytes);
      }
      else {
          return 0;
      }
  }

private:
  UART* _uart;

};
