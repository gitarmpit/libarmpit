#ifndef _ISERIAL_DEV_H
#define _ISERIAL_DEV_H

#include <stdint.h>

// Abstract Serial communication device
class ISerialDev {
  public:
    virtual void Write(const uint8_t* buffer, uint32_t nBytes) = 0;
    virtual uint16_t Read(uint8_t* buffer, uint16_t bytesRequested) = 0; 
};

#endif