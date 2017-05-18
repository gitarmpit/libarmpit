
// Graphics library by ladyada/adafruit with init code from Rossum
// MIT license

#ifndef _ADAFRUIT_TFTLCD_H_
#define _ADAFRUIT_TFTLCD_H_


#include "../Adafruit_GFX.h"

#include "gpio_cpp.h"

class Adafruit_TFTLCD : public Adafruit_GFX
{

private:


    GPIO_PIN* _cs;
    GPIO_PIN* _cd;
    GPIO_PIN* _wr;
    GPIO_PIN* _rd;
    GPIO_PIN* _rst;
    GPIO_PORT* _dataPort;
    uint8_t  driver;

    void     init(),
            // These items may have previously been defined as macros
            // in pin_magic.h.  If not, function versions are declared:
            write8(uint8_t value),
            setWriteDir(void),
            setReadDir(void),
            writeRegister8(uint8_t a, uint8_t d),
            writeRegister16(uint16_t a, uint16_t d),
            writeRegister24(uint8_t a, uint32_t d),
            writeRegister32(uint8_t a, uint32_t d),
            writeRegisterPair(uint8_t aH, uint8_t aL, uint16_t d),
            setLR(void),
            flood(uint16_t color, uint32_t len);
    uint8_t read8(void);
public:

  Adafruit_TFTLCD(GPIO_PIN* cs, GPIO_PIN* cd, GPIO_PIN* wr, GPIO_PIN* rd, GPIO_PIN* rst, GPIO_PORT* dataPort);

  void     begin(uint16_t id = 0x9325);
  void     drawPixel(int16_t x, int16_t y, uint16_t color);
  void     drawFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
  void     drawFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
  void     fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c);
  void     fillScreen(uint16_t color);
  void     reset(void);
  void     setRegisters8(uint8_t *ptr, uint8_t n);
  void     setRegisters16(uint16_t *ptr, uint8_t n);
  void     setRotation(uint8_t x);
       // These methods are public in order for BMP examples to work:
  void     setAddrWindow(int x1, int y1, int x2, int y2);
  void     pushColors(uint16_t *data, uint8_t len, bool first);

  uint16_t color565(uint8_t r, uint8_t g, uint8_t b),
           readPixel(int16_t x, int16_t y),
           readID(void);
  uint32_t readReg(uint8_t r);


};


#endif
