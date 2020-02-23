// Old 

#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#define PROGMEM
#include "gfxfont.h"
#include <stdint.h>

class Adafruit_GFX
{

public:

    Adafruit_GFX(int16_t w, int16_t h); // Constructor

    // This MUST be defined by the subclass:
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

    virtual void startWrite(void);
    virtual void writePixel(int16_t x, int16_t y, uint16_t color);
    virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h,
            uint16_t color);
    virtual void writeFastVLine(int16_t x, int16_t y, int16_t h,
            uint16_t color);
    virtual void writeFastHLine(int16_t x, int16_t y, int16_t w,
            uint16_t color);
    virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
            uint16_t color);
    virtual void endWrite(void);

    virtual void setRotation(uint8_t r);
    //virtual void invertDisplay(bool i);

    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
            uint16_t color);
    virtual void fillScreen(uint16_t color);
    // Optional and probably not necessary to change
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
            uint16_t color);
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
            uint16_t color);

    void  drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void  drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
    void  fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void  fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);


    void setCursor(int16_t x, int16_t y);
    void setTextColor(uint16_t c);
    void setTextColor(uint16_t c, uint16_t bg);
    void setTextWrap(bool w);
    void setFont(const GFXfont *f = 0);

    virtual void writeChar(uint16_t);
    uint8_t write(const char *str);
    uint8_t write(const wchar_t *str);
    void    drawChar(int16_t x, int16_t y, uint16_t c,  uint16_t color, uint16_t bg);

    int16_t height(void) const;
    int16_t width(void) const;

    uint8_t getRotation(void) const;

    // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
    int16_t getCursorX(void) const;
    int16_t getCursorY(void) const;

    uint8_t printf(const char *fmt, ...);

    void setYadvance (uint8_t val) { yAdvance = val; }

protected:
    const int16_t WIDTH, HEIGHT; // This is the 'raw' display w/h - never changes
    int16_t _width, _height; // Display w/h as modified by current rotation
    int16_t cursor_x, cursor_y;
    uint16_t fgcolor, bgcolor;
    uint8_t rotation;
    bool wrap;   // If set, 'wrap' text at right edge of display
    GFXfont *gfxFont;
private:
    char buf[64];
    uint8_t yAdvance;  //can override the font associated value
    uint8_t xAdvance;

};

#endif // _ADAFRUIT_GFX_H
