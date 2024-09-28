#pragma once 
#include "ftd2xx.h"
#include <stdint.h> 

class Ft232
{
public:
    Ft232();
    bool      Initialize(int baudRate);
    uint32_t  WaitRead(int timeoutMs);
    bool      Read(uint8_t* buf, int& bytesReceived, int timeoutMs);
    bool      Read(uint8_t* buf, uint32_t bytesToRead);
    bool      Send(const uint8_t* buf, int size);
    bool      SetTimeout(int timeoutMs);
    ~Ft232() { FT_Close(_ftHandle); }
private:
    bool _initialized;
    FT_HANDLE _ftHandle;
};