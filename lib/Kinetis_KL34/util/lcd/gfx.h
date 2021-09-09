#ifndef _GFX_H
#define _GFX_H

#define PROGMEM
#include "gfxfont.h"
#include <stdint.h>
#include <stdarg.h>
#include "common.h"

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


void    GFX_Init(int16_t w, int16_t h, void(*drawPixel)(int16_t,int16_t,uint16_t));
uint8_t GFX_writew(const wchar_t *str);
uint8_t GFX_write(const char *str);
uint8_t GFX_write_xy(const char *str, int16_t x, int16_t y, uint8_t size);
uint8_t GFX_printf(const char *fmt, ...);
uint8_t GFX_printf_xy(uint8_t x, uint8_t y, const char *fmt, ...);
void    GFX_drawBmp(int16_t x, int16_t y, const uint8_t *bmp);
void    GFX_setTextSize(uint8_t textSize);
void    GFX_setTextColor(uint16_t c);
void    GFX_setBgColor(uint16_t b);
void    GFX_setCursor(int16_t x, int16_t y);
void    GFX_writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void    GFX_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void    GFX_writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void    GFX_writeFastHLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void    GFX_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void    GFX_drawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, int16_t color);
void    GFX_writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void    GFX_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void    GFX_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void    GFX_drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void    GFX_fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void    GFX_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void    GFX_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void    GFX_fillScreen(uint16_t color);
int16_t GFX_getCursorX(void);
int16_t GFX_getCursorY(void);
void    GFX_setTextWrap(BOOL w);
uint8_t GFX_getRotation(void);
void    GFX_setRotation(uint8_t x);
void    GFX_setFont(const GFXfont *f);


#endif // _ADAFRUIT_GFX_H
