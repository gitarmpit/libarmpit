#ifndef _HX8357_H
#define _HX8357_H

#define MCU_FREQ 100

#ifndef MCU_FREQ
#error  MCU_FREQ not defined
#endif


//Can force delay directly
//#define __DELAY__ NOP3

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREEN2  0x0F0F

#include "../Adafruit_GFX.h"
#include "HX8357_registers.h"
#include "gpio.h"

#define TFTWIDTH   320
#define TFTHEIGHT  480


class HX8357: public Adafruit_GFX {
private:
    GPIO_PIN* _cs, *_rs, *_wr, *_rd, *_rst;
    GPIO_PORT* _dataPort;


public:
    HX8357(GPIO_PIN* cs, GPIO_PIN* rs, GPIO_PIN* wr, GPIO_PIN* rd,
            GPIO_PIN* rst, GPIO_PORT* dataPort);

    void init(void);

private:

     void setWriteDir()
     {
         *_dataPort->GPIO_PDDR = 0xff;
     }

    void setReadDir() {
        *_dataPort->GPIO_PDDR = 0;
    }

    uint8_t read8();

    inline void write8(uint8_t d)
    {
        *_dataPort->GPIO_PDOR = d;
        GPIO_ResetPin(_wr);
        //__nop();    __nop();     __nop();
        GPIO_SetPin(_wr);
    }

    void writeCommand(uint8_t addr);
    void writeData(uint8_t data);
    uint8_t readData();

    // Fast block fill operation for fillScreen, fillRect, H/V line, etc.
    // Requires setAddrWindow() has previously been called to set the fill
    // bounds.  'len' is inclusive, MUST be >= 1.

    // Pass 8-bit (each) R,G,B, get back 16-bit packed color
    inline uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }

public:

    // TODO make private
    //uint16_t readRegister16(uint16_t addr);
    //void writeRegister16(uint16_t addr, uint16_t data);
    void flood(uint16_t color, uint32_t len);
    uint16_t readID();
    void reset();
    void setRotation(uint8_t x);
    void fillScreen(uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t length, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t length, uint16_t color);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, uint16_t fillcolor);

    //TODO: private
    void setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
 };





#endif
