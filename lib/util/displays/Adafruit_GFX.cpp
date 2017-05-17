/*
 This is the core graphics library for all our displays, providing a common
 set of graphics primitives (points, lines, circles, etc.).  It needs to be
 paired with a hardware-specific library for each display device we carry
 (to handle the lower-level functions).

 Adafruit invests time and resources providing this open source code, please
 support Adafruit & open-source hardware by purchasing products from Adafruit!
 
 Copyright (c) 2013 Adafruit Industries.  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 - Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "Adafruit_GFX.h"
//#include "font.c"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fp2.h"
#include "ee_printf.h"

#ifdef LOAD_GLCD
#include "font.c"
#endif

#ifdef LOAD_FONT2
#include "Font16.h"
#endif

#ifdef LOAD_FONT4
#include "Font32.h"
#endif

#ifdef LOAD_FONT6
#include "Font64.h"
#endif

#ifdef LOAD_FONT7
#include "Font7s.h"
#endif

#ifndef min
#define min(a,b) ((a < b) ? a : b)
#endif

//Color palette for 256 color bitmap (microsoft Paint)
const uint16_t pal256[] =
{
    0x0000, 0x8000, 0x0400, 0x8400, 0x0010, 0x8010, 0x0410, 0xc618, 0xc6f8, 0xa65e, 0x4100, 0x6100, 0x8100, 0xa100, 0xc100, 0xe100,
    0x0200, 0x2200, 0x4200, 0x6200, 0x8200, 0xa200, 0xc200, 0xe200, 0x0300, 0x2300, 0x4300, 0x6300, 0x8300, 0xa300, 0xc300, 0xe300,
    0x0400, 0x2400, 0x4400, 0x6400, 0x8400, 0xa400, 0xc400, 0xe400, 0x0500, 0x2500, 0x4500, 0x6500, 0x8500, 0xa500, 0xc500, 0xe500,
    0x0600, 0x2600, 0x4600, 0x6600, 0x8600, 0xa600, 0xc600, 0xe600, 0x0700, 0x2700, 0x4700, 0x6700, 0x8700, 0xa700, 0xc700, 0xe700,
    0x0008, 0x2008, 0x4008, 0x6008, 0x8008, 0xa008, 0xc008, 0xe008, 0x0108, 0x2108, 0x4108, 0x6108, 0x8108, 0xa108, 0xc108, 0xe108,
    0x0208, 0x2208, 0x4208, 0x6208, 0x8208, 0xa208, 0xc208, 0xe208, 0x0308, 0x2308, 0x4308, 0x6308, 0x8308, 0xa308, 0xc308, 0xe308,
    0x0408, 0x2408, 0x4408, 0x6408, 0x8408, 0xa408, 0xc408, 0xe408, 0x0508, 0x2508, 0x4508, 0x6508, 0x8508, 0xa508, 0xc508, 0xe508,
    0x0608, 0x2608, 0x4608, 0x6608, 0x8608, 0xa608, 0xc608, 0xe608, 0x0708, 0x2708, 0x4708, 0x6708, 0x8708, 0xa708, 0xc708, 0xe708,
    0x0010, 0x2010, 0x4010, 0x6010, 0x8010, 0xa010, 0xc010, 0xe010, 0x0110, 0x2110, 0x4110, 0x6110, 0x8110, 0xa110, 0xc110, 0xe110,
    0x0210, 0x2210, 0x4210, 0x6210, 0x8210, 0xa210, 0xc210, 0xe210, 0x0310, 0x2310, 0x4310, 0x6310, 0x8310, 0xa310, 0xc310, 0xe310,
    0x0410, 0x2410, 0x4410, 0x6410, 0x8410, 0xa410, 0xc410, 0xe410, 0x0510, 0x2510, 0x4510, 0x6510, 0x8510, 0xa510, 0xc510, 0xe510,
    0x0610, 0x2610, 0x4610, 0x6610, 0x8610, 0xa610, 0xc610, 0xe610, 0x0710, 0x2710, 0x4710, 0x6710, 0x8710, 0xa710, 0xc710, 0xe710,
    0x0018, 0x2018, 0x4018, 0x6018, 0x8018, 0xa018, 0xc018, 0xe018, 0x0118, 0x2118, 0x4118, 0x6118, 0x8118, 0xa118, 0xc118, 0xe118,
    0x0218, 0x2218, 0x4218, 0x6218, 0x8218, 0xa218, 0xc218, 0xe218, 0x0318, 0x2318, 0x4318, 0x6318, 0x8318, 0xa318, 0xc318, 0xe318,
    0x0418, 0x2418, 0x4418, 0x6418, 0x8418, 0xa418, 0xc418, 0xe418, 0x0518, 0x2518, 0x4518, 0x6518, 0x8518, 0xa518, 0xc518, 0xe518,
    0x0618, 0x2618, 0x4618, 0x6618, 0x8618, 0xa618, 0xffde, 0xa514, 0x8410, 0xf800, 0x07e0, 0xffe0, 0x001f, 0xf81f, 0x07ff, 0xffff
};


const uint16_t pal16[] = { 0x0000, 0x8000, 0x0400, 0x8400, 0x0010, 0x8010, 0x0410, 0x8410, 0xc618, 0xf800, 0x07e0, 0xffe0, 0x001f, 0xf81f, 0x07ff, 0xffff };




Adafruit_GFX::Adafruit_GFX(int16_t w, int16_t h) :
        WIDTH(w), HEIGHT(h)
{
    _width = WIDTH;
    _height = HEIGHT;
    rotation = 0;
    cursor_y = cursor_x = 0;
    textsize = 1;
    fgcolor = bgcolor = 0xFFFF;
    wrap = true;
    _cp437 = false;
    _isXposPixels = false;

}

// Draw a circle outline
void Adafruit_GFX::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void Adafruit_GFX::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4)
        {
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2)
        {
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8)
        {
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1)
        {
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

void Adafruit_GFX::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void Adafruit_GFX::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1)
        {
            drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2)
        {
            drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}

// Bresenham's algorithm - thx wikpedia
void Adafruit_GFX::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    for (; x0 <= x1; x0++)
    {
        if (steep)
        {
            drawPixel(y0, x0, color);
        }
        else
        {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

// Draw a rectangle
void Adafruit_GFX::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y + h - 1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x + w - 1, y, h, color);
}

void Adafruit_GFX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    // Update in subclasses if desired!
    drawLine(x, y, x, y + h - 1, color);
}

void Adafruit_GFX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    // Update in subclasses if desired!
    drawLine(x, y, x + w - 1, y, color);
}

void Adafruit_GFX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    // Update in subclasses if desired!
    for (int16_t i = x; i < x + w; i++)
    {
        drawFastVLine(i, y, h, color);
    }
}

void Adafruit_GFX::fillScreen(uint16_t color)
{
    fillRect(0, 0, _width, _height, color);
}

// Draw a rounded rectangle
void Adafruit_GFX::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    // smarter version
    drawFastHLine(x + r, y, w - 2 * r, color); // Top
    drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    drawFastVLine(x, y + r, h - 2 * r, color); // Left
    drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

// Fill a rounded rectangle
void Adafruit_GFX::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    // smarter version
    fillRect(x + r, y, w - 2 * r, h, color);

    // draw four corners
    fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

// Draw a triangle
void Adafruit_GFX::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void Adafruit_GFX::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{

    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1)
    {
        swap(y0, y1);
        swap(x0, x1);
    }
    if (y1 > y2)
    {
        swap(y2, y1);
        swap(x2, x1);
    }
    if (y0 > y1)
    {
        swap(y0, y1);
        swap(x0, x1);
    }

    if (y0 == y2)
    { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)
            a = x1;
        else if (x1 > b)
            b = x1;
        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;
        drawFastHLine(a, y0, b - a + 1, color);
        return;
    }

    int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0, dx12 = x2 - x1, dy12 = y2 - y1;
    int32_t sa = 0, sb = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2)
        last = y1;   // Include y1 scanline
    else
        last = y1 - 1; // Skip it

    for (y = y0; y <= last; y++)
    {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
         a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
         b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if (a > b)
            swap(a, b);
        drawFastHLine(a, y, b - a + 1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y <= y2; y++)
    {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
         a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
         b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if (a > b)
            swap(a, b);
        drawFastHLine(a, y, b - a + 1, color);
    }
}

void Adafruit_GFX::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{

    int16_t i, j, byteWidth = (w + 7) / 8;

    for (j = 0; j < h; j++)
    {
        for (i = 0; i < w; i++)
        {
            if (*(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)))
            {
                drawPixel(x + i, y + j, color);
            }
        }
    }
}

// Draw a 1-bit color bitmap at the specified x, y position from the
// provided bitmap buffer (must be PROGMEM memory) using color as the
// foreground color and bg as the background color.
void Adafruit_GFX::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{

    int16_t i, j, byteWidth = (w + 7) / 8;

    for (j = 0; j < h; j++)
    {
        for (i = 0; i < w; i++)
        {
            if (*(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)))
            {
                drawPixel(x + i, y + j, color);
            }
            else
            {
                drawPixel(x + i, y + j, bg);
            }
        }
    }
}

//Draw XBitMap Files (*.xbm), exported from GIMP,
//Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
//C Array can be directly used with this function
void Adafruit_GFX::drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{

    int16_t i, j, byteWidth = (w + 7) / 8;

    for (j = 0; j < h; j++)
    {
        for (i = 0; i < w; i++)
        {
            if (*(bitmap + j * byteWidth + i / 8) & (1 << (i % 8)))
            {
                drawPixel(x + i, y + j, color);
            }
        }
    }
}

void Adafruit_GFX::write(uint8_t c)
{
    if (c == '\n')
    {
        cursor_y += 8;
        cursor_x = 0;
    }
    else if (c == '\r')
    {
        // skip em
    }
    else
    {
        drawChar(cursor_x, cursor_y, c, fgcolor, bgcolor, 1);
        cursor_x += 6; //textsize * 6;

        if (wrap && (cursor_x > (_width - 6)))
        {
            cursor_y += 8;
            cursor_x = 0;
        }
    }
}


uint8_t Adafruit_GFX::printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ee_vsprintf(buf, fmt, args);
    va_end(args);

    return write (buf, -1, -1, textsize);

}

uint8_t Adafruit_GFX::printf(int16_t x, int16_t y, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ee_vsprintf(buf, fmt, args);
    va_end(args);

    return write (buf, x, y, textsize);

}

uint8_t Adafruit_GFX::printf(int16_t x, int16_t y, int16_t size, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ee_vsprintf(buf, fmt, args);
    va_end(args);

    return write (buf, x, y, size);

}


uint8_t Adafruit_GFX::write(const char *str)
{
    return write (str, -1, -1, textsize);
}

uint8_t Adafruit_GFX::write(const char *str, int16_t x, int16_t y, uint8_t size)
{
    if (size == 0)
    {
        size = textsize;
    }

    if (size > 1)
    {
        int sumX = 0;
        int poX = x;
        uint8_t h, w;

        if (x > -1 && !_isXposPixels)
        {
            getCharWH(size, &w, &h);
            poX *= w;
            cursor_x = poX;
        }

        if (y > -1)
        {
            getCharWH(size, &w, &h);
            cursor_y = y * h;
        }

        while (*str)
        {
            //int xPlus = drawUnicode(*str, poX, y, size);
            int xPlus = drawUnicode(*str, size);
            sumX += xPlus;
            str++;
            //++poX;
            poX += xPlus; /* Move cursor right       */
        }

        return sumX;
    }
    else
    {
        if (x > -1 && y > -1)
        {
            if (_isXposPixels)
            {
                cursor_x = x;
                cursor_y = y;
            }
            else
            {
                cursor_x = x*6;
                cursor_y = y*8;
            }
        }
        uint8_t len = 0;
        if (str != NULL)
        {
            size_t size = strlen(str);
            while (size--)
            {
                write(*str++);
            }
            len = strlen(str);
        }
        return len;
    }

}

//uint8_t Adafruit_GFX::write_float(float val, int8_t decimals, int16_t x, int16_t y, uint8_t size)
//{
//    fix16_to_str(fix16_from_float(val), buf, decimals);
//    return write (buf, x, y, size);
//}




// Draw a charact   er
void Adafruit_GFX::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{

    if ((x >= _width) || // Clip right
            (y >= _height) || // Clip bottom
            ((x + 6 * size - 1) < 0) || // Clip left
            ((y + 8 * size - 1) < 0))   // Clip top
        return;

    if (!_cp437 && (c >= 176))
        c++; // Handle 'classic' charset behavior

    for (int8_t i = 0; i < 6; i++)
    {
        uint8_t line;
        if (i == 5)
            line = 0x0;
        else
            line = *(font + (c * 5) + i);

        line = ~line;

        for (int8_t j = 0; j < 8; j++)
        {
            if (line & 0x1)
            {
                if (size == 1) // default size
                    drawPixel(x + i, y + j, bg);
                else
                {  // big size
                    fillRect(x + (i * size), y + (j * size), size, size, bg);
                }
            }
            else if (bg != color)
            {
                if (size == 1) // default size
                    drawPixel(x + i, y + j, color);
                else
                {  // big size
                    fillRect(x + i * size, y + j * size, size, size, color);
                }
            }
            line >>= 1;
        }
    }
}

void Adafruit_GFX::setCursor(int16_t x, int16_t y)
{
    uint8_t w, h;
    getCharWH(textsize, &w, &h);
    cursor_x = x*w;
    cursor_y = y*h;
}

int16_t Adafruit_GFX::getCursorX(void) const
{
    return cursor_x;
}

int16_t Adafruit_GFX::getCursorY(void) const
{
    return cursor_y;
}

void Adafruit_GFX::setTextSize(uint8_t s)
{
    textsize = (s > 0) ? s : 1;
}

void Adafruit_GFX::setTextColor(uint16_t c)
{
    // For 'transparent' background, we'll set the bg
    // to the same as fg instead of using a flag
    fgcolor = c;
}

void Adafruit_GFX::setBgColor(uint16_t b)
{
    bgcolor = b;
}

void Adafruit_GFX::setTextWrap(bool w)
{
    wrap = w;
}

uint8_t Adafruit_GFX::getRotation(void) const
{
    return rotation;
}

void Adafruit_GFX::setRotation(uint8_t x)
{
    rotation = (x & 3);
    switch (rotation)
    {
    case 0:
    case 2:
        _width = WIDTH;
        _height = HEIGHT;
        break;
    case 1:
    case 3:
        _width = HEIGHT;
        _height = WIDTH;
        break;
    }
}

// Enable (or disable) Code Page 437-compatible charset.
// There was an error in glcdfont.c for the longest time -- one character
// (#176, the 'light shade' block) was missing -- this threw off the index
// of every character that followed it.  But a TON of code has been written
// with the erroneous character indices.  By default, the library uses the
// original 'wrong' behavior and old sketches will still work.  Pass 'true'
// to this function to use correct CP437 character values in your code.
void Adafruit_GFX::cp437(bool x)
{
    _cp437 = x;
}

// Return the size of the display (per current rotation)
int16_t Adafruit_GFX::width(void) const
{
    return _width;
}

int16_t Adafruit_GFX::height(void) const
{
    return _height;
}

//void Adafruit_GFX::invertDisplay(bool i)
//{
//    // Do nothing, must be subclassed if supported
//}

////////////////////////////////////////////////////////////////////////

void Adafruit_GFX::getCharWH(uint8_t size, uint8_t* w, uint8_t* h)
{
    if (size == 1)
    {
        *h = 8;
        *w = 6;
    }
    else if (size == 2)
    {
        *h = chr_hgt_f16;
        *w = 7;
    }
#ifdef LOAD_FONT4
    else if (size == 4)
    {
        *h = chr_hgt_f32;
        *w = 17;
    }
#endif
#ifdef LOAD_FONT6
    else if (size == 6)
    {
        *h = chr_hgt_f64;
        *w = 15;
    }
#endif
#ifdef LOAD_FONT7
    else if (size == 7)
    {
        *h = chr_hgt_f7s;
        *w = 12;
    }
#endif

}

/***************************************************************************************
 ** Function name:           drawUnicode
 ** Descriptions:            draw a unicode
 ***************************************************************************************/
int Adafruit_GFX::drawUnicode(unsigned int uniCode, int size)
{

    uniCode -= 32;

    unsigned int width = 0;
    unsigned int height = 0;
    char gap = 0;
    const unsigned char* flash_address = 0;

#ifdef LOAD_FONT2
    if (size == 2)
    {
        flash_address = chrtbl_f16[uniCode];
        width = widtbl_f16[uniCode];
        height = chr_hgt_f16;
        gap = 1;
    }
#endif

    #ifdef LOAD_FONT4
    if (size == 4)
    {
        flash_address = chrtbl_f32[uniCode];
        width = widtbl_f32[uniCode];
        height = chr_hgt_f32;
        gap = -3;
    }
#endif

    #ifdef LOAD_FONT6
    if (size == 6)
    {
        flash_address = chrtbl_f64[uniCode];
        width = widtbl_f64[uniCode];
        height = chr_hgt_f64;
        gap = -3;
    }
#endif
#ifdef LOAD_FONT7
    if (size == 7)
    {
        flash_address = chrtbl_f7s[uniCode];
        width = widtbl_f7s[uniCode];
        height = chr_hgt_f7s;
        gap = 2;
    }
#endif

    //y *= height;

    if (flash_address == 0)
    {
        return 0;
    }

    int x = cursor_x;
    int y = cursor_y;

    int w = (width + 7) / 8;
    int pX = 0;
    int pY = y;
    int8_t line = 0;

    for (unsigned int i = 0; i < height; i++)
    {
        if (fgcolor != bgcolor)
        {
            drawFastHLine(x, pY, width + gap, bgcolor);
        }
        for (int k = 0; k < w; k++)
        {
            line = flash_address[w * i + k];

            pX = x + k * 8;
            if (line & 0x80)
                drawPixel(pX, pY, fgcolor);
            if (line & 0x40)
                drawPixel(pX + 1, pY, fgcolor);
            if (line & 0x20)
                drawPixel(pX + 2, pY, fgcolor);
            if (line & 0x10)
                drawPixel(pX + 3, pY, fgcolor);
            if (line & 0x8)
                drawPixel(pX + 4, pY, fgcolor);
            if (line & 0x4)
                drawPixel(pX + 5, pY, fgcolor);
            if (line & 0x2)
                drawPixel(pX + 6, pY, fgcolor);
            if (line & 0x1)
                drawPixel(pX + 7, pY, fgcolor);
        }
        ++pY;
    }

    cursor_x += (width + gap);

    return (width + gap);
}

void Adafruit_GFX::invertBgFg()
{
    uint16_t tmp = bgcolor;
    bgcolor = fgcolor;
    fgcolor = tmp;
}

//int Adafruit_GFX::drawChar(char c, int x, int y, int size)
//{
//    return drawUnicode(c, x, y, size);
//}


/*
 * int Adafruit_GFX::drawUnicode(unsigned int uniCode, int x, int y, int size)
{

    uniCode -= 32;

    unsigned int width = 0;
    unsigned int height = 0;
    char gap = 0;
    const unsigned char* flash_address = 0;

#ifdef LOAD_FONT2
    if (size == 2)
    {
        flash_address = chrtbl_f16[uniCode];
        width = widtbl_f16[uniCode];
        height = chr_hgt_f16;
        gap = 1;
    }
#endif

    #ifdef LOAD_FONT4
    if (size == 4)
    {
        flash_address = chrtbl_f32[uniCode];
        width = widtbl_f32[uniCode];
        height = chr_hgt_f32;
        gap = -3;
    }
#endif

    #ifdef LOAD_FONT6
    if (size == 6)
    {
        flash_address = chrtbl_f64[uniCode];
        width = widtbl_f64[uniCode];
        height = chr_hgt_f64;
        gap = -3;
    }
#endif
#ifdef LOAD_FONT7
    if (size == 7)
    {
        flash_address = chrtbl_f7s[uniCode];
        width = widtbl_f7s[uniCode];
        height = chr_hgt_f7s;
        gap = 2;
    }
#endif

    y *= (height + 0);

    if (flash_address == 0)
    {
        return 0;
    }

    int w = (width + 7) / 8;
    int pX = 0;
    int pY = y;
    int8_t line = 0;

    for (unsigned int i = 0; i < height; i++)
    {
        if (textcolor != textbgcolor)
        {
            drawFastHLine(x, pY, width + gap, textbgcolor);
        }
        for (int k = 0; k < w; k++)
        {
            line = flash_address[w * i + k];

            pX = x + k * 8;
            if (line & 0x80)
                drawPixel(pX, pY, textcolor);
            if (line & 0x40)
                drawPixel(pX + 1, pY, textcolor);
            if (line & 0x20)
                drawPixel(pX + 2, pY, textcolor);
            if (line & 0x10)
                drawPixel(pX + 3, pY, textcolor);
            if (line & 0x8)
                drawPixel(pX + 4, pY, textcolor);
            if (line & 0x4)
                drawPixel(pX + 5, pY, textcolor);
            if (line & 0x2)
                drawPixel(pX + 6, pY, textcolor);
            if (line & 0x1)
                drawPixel(pX + 7, pY, textcolor);
        }
        ++pY;
    }

    cursor_x += (width + gap);

    return (width + gap);
}
 */


void Adafruit_GFX::drawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, int16_t color)
{
    if (color == -1)
    {
        color = fgcolor;
    }

    int x, y;
    int dx, dy;
    int err;
    int twoASquare, twoBSquare;
    int xend, yend;

    twoASquare = 2*rx*rx;
    twoBSquare = 2*ry*ry;

    x = rx;
    y = 0;

    dx = ry*ry*(1 - 2*rx);
    dy = rx*rx;
    err = 0;

    xend = twoBSquare*rx;
    yend = 0;
    while (xend  >= yend )
    {
        drawPixel(x0+x, y0+y, color);
        drawPixel(x0-x, y0+y, color);
        drawPixel(x0-x, y0-y, color);
        drawPixel(x0+x, y0-y, color);

        ++y;
        yend += twoASquare;
        err += dy;
        dy += twoASquare;
        if ((2*err + dx) > 0 )
        {
            --x;
            xend -= twoBSquare;
            err += dx;
            dx += twoBSquare;
        }
    }


    x = 0;
    y = ry;
    dx = ry*ry;
    dy = rx*rx*(1 - 2*ry);
    err = 0;
    xend = 0;
    yend = twoASquare*ry;

    while ( xend <= yend )
    {
        drawPixel(x0+x, y0+y, color);
        drawPixel(x0-x, y0+y, color);
        drawPixel(x0-x, y0-y, color);
        drawPixel(x0+x, y0-y, color);
        ++x;
        xend += twoBSquare;
        err += dx;
        dx += twoBSquare;

        if ((2*err + dy) > 0 )
        {
            --y;
            yend -= twoASquare;
            err += dy;
            dy += twoASquare;
        }
    }
}


void Adafruit_GFX::drawBmp(int16_t x, int16_t y, const uint8_t *bmp)
{
    WINBMPFILEHEADER* hdr1 = (WINBMPFILEHEADER*)bmp;
    WIN3XBITMAPHEADER*  hdr2 = (WIN3XBITMAPHEADER*)&bmp[sizeof(WINBMPFILEHEADER)];

    if (hdr2->BitsPerPixel != 4 &&
        hdr2->BitsPerPixel != 8 &&
        hdr2->BitsPerPixel != 16)
    {
        printf (0, 0, "Not supported color depth: %d", hdr2->BitsPerPixel);
        return;
    }

    const uint16_t* bitmap16 = (uint16_t*)&bmp[hdr1->BitmapOffset];
    const uint8_t* bitmap8 = &bmp[hdr1->BitmapOffset];

    uint16_t wread = hdr2->Width;
    uint16_t h = abs(hdr2->Height);

    if (hdr2->Width / 2 * 2 != hdr2->Width)
    {
        wread = (hdr2->Width / 2 + 1) * 2;
    }

    if (hdr2->BitsPerPixel == 4)
    {
        wread /= 2;
    }


    uint16_t c = 0;
    for (uint32_t j = 0; j < h; ++j)
    {
        uint32_t wtotal = 0;
        for (uint32_t i = 0; i < wread; ++i)
        {
            if (hdr2->BitsPerPixel == 4)
            {
                c = bitmap8[(hdr2->Height-j-1)*wread + i];
                uint16_t c1 = c>>4;
                uint16_t c2 = c&0xf;

                if (wtotal <= hdr2->Width)
                {
                    drawPixel(x + wtotal++, y + j, pal16[c1]);
                }
                if (wtotal <= hdr2->Width)
                {
                    drawPixel(x + wtotal++, y + j, pal16[c2]);
                }
            }
            else
            {
                if (hdr2->BitsPerPixel == 16)
                {
                    c = bitmap16[(hdr2->Height-j-1)*wread + i];
                }
                else if (hdr2->BitsPerPixel == 8)
                {
                    c = bitmap8[(hdr2->Height-j-1)*wread + i];
                    c = pal256[c];
                }

                if (i < hdr2->Width)
                {
                    drawPixel(x + i, y + j, c);
                }
            }
        }
    }

}
