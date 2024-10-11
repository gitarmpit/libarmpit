#pragma once 
#include <stdint.h> 
#include <windows.h>
#include <stdexcept>

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
    DWORD     Read(uint8_t* buf, int timeoutMs);
    DWORD     Read(uint8_t* buf, DWORD bytesToRead);
    bool      Write(const uint8_t* buf, DWORD length);
    bool      SetTimeout(int timeoutMs);
    ~UART() { if (_hSerial != NULL && _hSerial != INVALID_HANDLE_VALUE) { CloseHandle(_hSerial); } }
private:
    bool _initialized;
    HANDLE    _hSerial;
};