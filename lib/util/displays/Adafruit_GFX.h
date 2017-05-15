#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#include <stdint.h>

#define LOAD_GLCD
#define LOAD_FONT2
//#define LOAD_FONT4
//#define LOAD_FONT6
//#define LOAD_FONT7

//uncomment if using DMA with SPI
//#define INVERTED_565

//r:0-31, g:0-64: b:0-31

#ifdef INVERTED_565
#define COLOR565(r,g,b) \
( ((r & 0x1f) << 3 ) | ((g & 0x38) >>3 ) |  ((g & 0x7) <<13) |  ((b & 0x1f) << 8)  )
#else
#define COLOR565(r,g,b) \
	( ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F) )
#endif




#define swap(a, b) { int16_t t = a; a = b; b = t; }
//#define pgm_read_byte(x)        (*((char *)x))
//#define pgm_read_word(x)        ( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x)))


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

class Adafruit_GFX // : public Print {
{
public:

    Adafruit_GFX(int16_t w, int16_t h); // Constructor

    // This MUST be defined by the subclass:
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillScreen(uint16_t color), invertDisplay(bool i);

    // These exist only with Adafruit_GFX (no subclass overrides)
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircleHelper(int16_t x0,int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
    void drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
    void drawBmp(int16_t x, int16_t y, const uint8_t *bmp);
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
    void setCursor(int16_t x, int16_t y);
    void setTextColor(uint16_t c);
    void setBgColor(uint16_t c);
    void setTextSize(uint8_t s);
    void setTextWrap(bool w);
    void setRotation(uint8_t r);
    void cp437(bool x = true);

    virtual void write(uint8_t);
    uint8_t write(const char *str);
    uint8_t write(const char *str, int16_t x, int16_t y, uint8_t size = 0);
    uint8_t printf(const char *fmt, ...);
    uint8_t printf(int16_t x, int16_t y, const char *fmt, ...);
    uint8_t printf(int16_t x, int16_t y, int16_t size, const char *fmt, ...);

    //uint8_t write_float(float val, int8_t decimals, int16_t x = -1, int16_t y = -1, uint8_t size = 0);
    int16_t height(void) const;
    int16_t width(void) const;

    uint8_t getRotation(void) const;

    // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
    int16_t getCursorX(void) const;
    int16_t getCursorY(void) const;

    int drawUnicode(unsigned int uniCode, int size = 0);
    //int drawChar(char c, int x, int y, int size = 0);


    void setXposInPixels(bool set) { _isXposPixels = set; }

    void invertBgFg();
    void drawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, int16_t color = -1);

protected:
    const int16_t WIDTH, HEIGHT; // This is the 'raw' display w/h - never changes
    int16_t _width, _height, // Display w/h as modified by current rotation
            cursor_x, cursor_y;
    uint16_t fgcolor, bgcolor;
    uint8_t textsize, rotation;
    bool wrap;   // If set, 'wrap' text at right edge of display
    bool _cp437; // If set, use correct CP437 charset (default is off)
private:
    char buf[64];

    //defines if x position is in pixels(true) or characters (false, the default)
    bool _isXposPixels;

    static void getCharWH(uint8_t size, uint8_t* widht, uint8_t* hight);

};

#endif // _ADAFRUIT_GFX_H
