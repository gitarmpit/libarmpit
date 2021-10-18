#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#define PROGMEM
#include "gfxfont.h"
#include <stdint.h>
#include <stdarg.h>

#ifdef INVERTED_565
#define COLOR565(r,g,b) \
( ((r & 0x1f) << 3 ) | ((g & 0x38) >>3 ) |  ((g & 0x7) <<13) |  ((b & 0x1f) << 8)  )
#else
#define COLOR565(r,g,b) \
    ( ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F) )
#endif


#pragma pack(push, 2)
typedef struct _WinBMPFileHeader
{
    uint16_t   FileType;     /* File type, always 4D42h ("BM") */
    uint32_t  FileSize;     /* Size of the file in bytes */
    uint16_t   Reserved1;    /* Always 0 */
    uint16_t   Reserved2;    /* Always 0 */
    uint32_t  BitmapOffset; /* Starting position of image data in bytes */
} WINBMPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 2)

typedef struct _Win3xBitmapHeader
{
    uint32_t Size;            /* Size of this header in bytes */
    uint32_t  Width;           /* Image width in pixels */
    uint32_t  Height;          /* Image height in pixels */
    uint16_t  Planes;          /* Number of color planes */
    uint16_t  BitsPerPixel;    /* Number of bits per pixel */
    /* Fields added for Windows 3.x follow this line */
    uint32_t Compression;     /* Compression methods used */
    uint32_t SizeOfBitmap;    /* Size of bitmap in bytes */
    uint32_t  HorzResolution;  /* Horizontal resolution in pixels per meter */
    uint32_t  VertResolution;  /* Vertical resolution in pixels per meter */
    uint32_t ColorsUsed;      /* Number of colors in the image */
    uint32_t ColorsImportant; /* Minimum number of important colors */
} WIN3XBITMAPHEADER;
#pragma pack(pop)


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
    void setBgColor(uint16_t b);
    void setTextWrap(bool w);
    void setFont(const GFXfont *f = 0);

    int16_t height(void) const { return _height; }
    int16_t width(void) const { return _width; }

    uint8_t getRotation(void) const;

    // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
    int16_t getCursorX(void) const;
    int16_t getCursorY(void) const;

    void setYadvance (uint8_t val) { yAdvance = val; }
    void setXadvance (uint8_t val) { xAdvance = val; }


    uint8_t write(const wchar_t *str);
    uint8_t write(const char *str);
    uint8_t write(int16_t x, int16_t y, const char *str, uint8_t size = 0);
    uint8_t printf(const char *fmt, ...);
    uint8_t printf(uint8_t x, uint8_t y, const char *fmt, ...);
    void drawBmp(int16_t x, int16_t y, const uint8_t *bmp);
    void setTextSize(uint8_t textSize);
    void drawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, int16_t color = -1);

protected:

    uint8_t printf(const char *fmt, va_list args);
    virtual void writeChar(uint16_t);
    void    drawChar(int16_t x, int16_t y, uint16_t c,  uint16_t color, uint16_t bg);

    const int16_t WIDTH, HEIGHT; // This is the 'raw' display w/h - never changes
    int16_t _width, _height; // Display w/h as modified by current rotation
    int16_t cursor_x, cursor_y;
    uint16_t fgcolor, bgcolor;
    uint8_t rotation;
    bool wrap;   // If set, 'wrap' text at right edge of display
    GFXfont *gfxFont;
private:
    char buf[1024];
    uint8_t yAdvance;  //can override the font associated value
    uint8_t xAdvance;

    //ignored for custom fonts;
    int textsize_x;
    int textsize_y;


};

#endif // _ADAFRUIT_GFX_H
