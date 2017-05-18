// Code provided by Smoke And Wires
// http://www.smokeandwires.co.nz
// This code has been taken from the Adafruit TFT Library and modified
//  by us for use with our TFT Shields / Modules
// For original code / licensing please refer to
// https://github.com/adafruit/TFTLCD-Library

// IMPORTANT: SEE COMMENTS @ LINE 15 REGARDING SHIELD VS BREAKOUT BOARD USAGE.

// Graphics library by ladyada/adafruit with init code from Rossum
// MIT license

#ifndef _SWTFT_H_
#define _SWTFT_H_

#include "../Adafruit_GFX.h"
#include "gpio_cpp.h"

class SWTFT : public Adafruit_GFX
{

 public:

  SWTFT(GPIO_PIN* cs, GPIO_PIN* cd, GPIO_PIN* wr, GPIO_PIN* rd, GPIO_PIN* rst, GPIO_PORT* dataPort);

  void     begin(uint16_t id = 0x9325);
  void     drawPixel(int16_t x, int16_t y, uint16_t color);
  /////////////////////////////////////////
  void     writeA16B3(uint16_t address, uint8_t ub, uint8_t mb, uint8_t lb);
  void     drawPixel3(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b);
  void     drawLine3Byte(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t r, uint8_t g, uint8_t b) ;
  //////////////////////////////////////////
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

 private:

  GPIO_PIN* _cs;
  GPIO_PIN* _cd;
  GPIO_PIN* _wr;
  GPIO_PIN* _rd;
  GPIO_PIN* _rst;
  GPIO_PORT* _dataPort;

  void     init(),
           // These items may have previously been defined as macros
           // If not, function versions are declared:
           write8(uint8_t value),
           setWriteDir(void),
           setReadDir(void),
           writeRegister8(uint8_t a, uint8_t d),
           writeRegister16(uint16_t a, uint16_t d),
           writeRegisterPair(uint8_t aH, uint8_t aL, uint16_t d),
           setLR(void),
           flood(uint16_t color, uint32_t len);
  uint8_t  driver;

  uint8_t  read8(void);
};


#endif //_SWTFT_H_
