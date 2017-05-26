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
#include "glcdfont.c"
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include "ee_printf.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
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
    fgcolor = bgcolor = 0xFFFF;
    wrap = true;
    gfxFont = 0;
    yAdvance = 0;
    xAdvance = 0;
}

// Bresenham's algorithm - thx wikpedia
void Adafruit_GFX::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1)
    {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
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
            writePixel(y0, x0, color);
        }
        else
        {
            writePixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void Adafruit_GFX::startWrite()
{
    // Overwrite in subclasses if desired!
}

void Adafruit_GFX::writePixel(int16_t x, int16_t y, uint16_t color)
{
    // Overwrite in subclasses if startWrite is defined!
    drawPixel(x, y, color);
}

// (x,y) is topmost point; if unsure, calling function
// should sort endpoints or call writeLine() instead
void Adafruit_GFX::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    // Overwrite in subclasses if startWrite is defined!
    // Can be just writeLine(x, y, x, y+h-1, color);
    // or writeFillRect(x, y, 1, h, color);
    drawFastVLine(x, y, h, color);
}

// (x,y) is leftmost point; if unsure, calling function
// should sort endpoints or call writeLine() instead
void Adafruit_GFX::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    // Overwrite in subclasses if startWrite is defined!
    // Example: writeLine(x, y, x+w-1, y, color);
    // or writeFillRect(x, y, w, 1, color);
    drawFastHLine(x, y, w, color);
}

void Adafruit_GFX::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    // Overwrite in subclasses if desired!
    fillRect(x, y, w, h, color);
}

void Adafruit_GFX::endWrite()
{
    // Overwrite in subclasses if startWrite is defined!
}

// (x,y) is topmost point; if unsure, calling function
// should sort endpoints or call drawLine() instead
void Adafruit_GFX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    // Update in subclasses if desired!
    startWrite();
    writeLine(x, y, x, y + h - 1, color);
    endWrite();
}

// (x,y) is leftmost point; if unsure, calling function
// should sort endpoints or call drawLine() instead
void Adafruit_GFX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    // Update in subclasses if desired!
    startWrite();
    writeLine(x, y, x + w - 1, y, color);
    endWrite();
}

void Adafruit_GFX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    // Update in subclasses if desired!
    startWrite();
    for (int16_t i = x; i < x + w; i++)
    {
        writeFastVLine(i, y, h, color);
    }
    endWrite();
}

void Adafruit_GFX::fillScreen(uint16_t color)
{
    // Update in subclasses if desired!
    fillRect(0, 0, _width, _height, color);
}

void Adafruit_GFX::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    // Update in subclasses if desired!
    if (x0 == x1)
    {
        if (y0 > y1)
            _swap_int16_t(y0, y1);
        drawFastVLine(x0, y0, y1 - y0 + 1, color);
    }
    else if (y0 == y1)
    {
        if (x0 > x1)
            _swap_int16_t(x0, x1);
        drawFastHLine(x0, y0, x1 - x0 + 1, color);
    }
    else
    {
        startWrite();
        writeLine(x0, y0, x1, y1, color);
        endWrite();
    }
}

// Draw a circle outline
void Adafruit_GFX::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    startWrite();
    writePixel(x0, y0 + r, color);
    writePixel(x0, y0 - r, color);
    writePixel(x0 + r, y0, color);
    writePixel(x0 - r, y0, color);

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

        writePixel(x0 + x, y0 + y, color);
        writePixel(x0 - x, y0 + y, color);
        writePixel(x0 + x, y0 - y, color);
        writePixel(x0 - x, y0 - y, color);
        writePixel(x0 + y, y0 + x, color);
        writePixel(x0 - y, y0 + x, color);
        writePixel(x0 + y, y0 - x, color);
        writePixel(x0 - y, y0 - x, color);
    }
    endWrite();
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
            writePixel(x0 + x, y0 + y, color);
            writePixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2)
        {
            writePixel(x0 + x, y0 - y, color);
            writePixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8)
        {
            writePixel(x0 - y, y0 + x, color);
            writePixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1)
        {
            writePixel(x0 - y, y0 - x, color);
            writePixel(x0 - x, y0 - y, color);
        }
    }
}

void Adafruit_GFX::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    startWrite();
    writeFastVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
    endWrite();
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
            writeFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            writeFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2)
        {
            writeFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            writeFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}

void Adafruit_GFX::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    startWrite();
    writeFastHLine(x, y, w, color);
    writeFastHLine(x, y + h - 1, w, color);
    writeFastVLine(x, y, h, color);
    writeFastVLine(x + w - 1, y, h, color);
    endWrite();
}

void Adafruit_GFX::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    // smarter version
    startWrite();
    writeFastHLine(x + r, y, w - 2 * r, color); // Top
    writeFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    writeFastVLine(x, y + r, h - 2 * r, color); // Left
    writeFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
    endWrite();
}

// Fill a rounded rectangle
void Adafruit_GFX::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    // smarter version
    startWrite();
    writeFillRect(x + r, y, w - 2 * r, h, color);

    // draw four corners
    fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
    endWrite();
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
        _swap_int16_t(y0, y1);
        _swap_int16_t(x0, x1);
    }
    if (y1 > y2)
    {
        _swap_int16_t(y2, y1);
        _swap_int16_t(x2, x1);
    }
    if (y0 > y1)
    {
        _swap_int16_t(y0, y1);
        _swap_int16_t(x0, x1);
    }

    startWrite();
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
        writeFastHLine(a, y0, b - a + 1, color);
        endWrite();
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
            _swap_int16_t(a, b);
        writeFastHLine(a, y, b - a + 1, color);
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
            _swap_int16_t(a, b);
        writeFastHLine(a, y, b - a + 1, color);
    }
    endWrite();
}

void Adafruit_GFX::drawChar(int16_t x, int16_t y, uint16_t c, uint16_t color, uint16_t bg)
{

    if (!gfxFont)
    { // 'Classic' built-in font

        if ((x >= _width) || // Clip right
                (y >= _height) || // Clip bottom
                ((x + 6 - 1) < 0) || // Clip left
                ((y + 8 - 1) < 0))   // Clip top
            return;

        if (c >= 176)
            c++; // Handle 'classic' charset behavior

        startWrite();
        for (int8_t i = 0; i < 5; i++)
        { // Char bitmap = 5 columns
            uint8_t line = font[c * 5 + i];
            for (int8_t j = 0; j < 8; j++, line >>= 1)
            {
                if (line & 1)
                {
                    writePixel(x + i, y + j, color);
                }
                else if (bg != color)
                {
                    writePixel(x + i, y + j, bg);
                }
            }
        }
        if (bg != color)
        { // If opaque, draw vertical line for last column
            writeFastVLine(x + 5, y, 8, bg);
        }
        endWrite();

    }
    else
    { // Custom font

        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!

        c -= gfxFont->first;
        GFXglyph *glyph = &gfxFont->glyph[c];
        uint8_t *bitmap = gfxFont->bitmap;

        uint16_t bo = glyph->bitmapOffset;
        uint8_t w = glyph->width, h = glyph->height;
        int8_t xo = glyph->xOffset, yo = glyph->yOffset;
        uint8_t xx, yy, bits = 0, bit = 0;
        int16_t xo16 = 0, yo16 = 0;

        // Todo: Add character clipping here

        // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        // has typically been used with the 'classic' font to overwrite old
        // screen contents with new data.  This ONLY works because the
        // characters are a uniform size; it's not a sensible thing to do with
        // proportionally-spaced fonts with glyphs of varying sizes (and that
        // may overlap).  To replace previously-drawn text when using a custom
        // font, use the getTextBounds() function to determine the smallest
        // rectangle encompassing a string, erase the area with fillRect(),
        // then draw new text.  This WILL infortunately 'blink' the text, but
        // is unavoidable.  Drawing 'background' pixels will NOT fix this,
        // only creates a new set of problems.  Have an idea to work around
        // this (a canvas object type for MCUs that can afford the RAM and
        // displays supporting setAddrWindow() and pushColors()), but haven't
        // implemented this yet.

        startWrite();
        for (yy = 0; yy < h; yy++)
        {
            for (xx = 0; xx < w; xx++)
            {
                if (!(bit++ & 7))
                {
                    bits = bitmap[bo++];
                }
                if (bits & 0x80)
                {
                    writePixel(x + xo + xx, y + yo + yy, color);
                }
                bits <<= 1;
            }
        }
        endWrite();

    } // End classic vs custom font
}

void Adafruit_GFX::writeChar(uint16_t c)
{
    if (!gfxFont)
    { // 'Classic' built-in font

        if (c == '\n')
        {                        // Newline?
            cursor_x = 0;                     // Reset x to zero,
            cursor_y += 8;          // advance y one line
        }
        else if (c != '\r')
        {                 // Ignore carriage returns
            if (wrap && ((cursor_x + 6) > _width))
            { // Off right?
                cursor_x = 0;                 // Reset x to zero,
                cursor_y += 8;      // advance y one line
            }
            drawChar(cursor_x*6, cursor_y*8, c, fgcolor, bgcolor);
            ++cursor_x;          // Advance x one char
        }

    }
    else
    { // Custom font

        if (c == '\n')
        {
            cursor_x = 0;
            //cursor_y += (int16_t)gfxFont->yAdvance;
            ++cursor_y;
        }
        else if (c != '\r')
        {
            uint16_t first = gfxFont->first;
            if ((c >= first) && (c <= gfxFont->last))
            {
                GFXglyph *glyph = &gfxFont->glyph[c - first];
                uint8_t w = glyph->width, h = glyph->height;
                if ((w > 0) && (h > 0))
                { // Is there an associated bitmap?
                    int16_t xo = glyph->xOffset; // sic
                    if (wrap && ((cursor_x + (xo + w)) > _width))
                    {
                        cursor_x = 0;
                        //cursor_y += (int16_t)gfxFont->yAdvance;
                        ++cursor_y;
                    }
                    drawChar(cursor_x, (cursor_y + 1) * yAdvance, c, fgcolor, bgcolor);
                }
                cursor_x += xAdvance ? xAdvance : glyph->xAdvance;
            }
        }

    }
}


uint8_t Adafruit_GFX::write(const wchar_t *str)
{
    uint8_t len = 0;
    if (str != 0)
    {
        size_t size = wcslen(str);
        while (size--)
        {
            writeChar(*str++);
        }
        len = wcslen(str);
    }
    return len;

}

void Adafruit_GFX::setCursor(int16_t x, int16_t y)
{
    if (xAdvance > 0)
    {
        cursor_x = x * xAdvance;
    }
    else
    {
        cursor_x = x;
    }
    cursor_y = y;
}

int16_t Adafruit_GFX::getCursorX(void) const
{
    return cursor_x;
}

int16_t Adafruit_GFX::getCursorY(void) const
{
    return cursor_y;
}

void Adafruit_GFX::setTextColor(uint16_t c)
{
    // For 'transparent' background, we'll set the bg
    // to the same as fg instead of using a flag
    fgcolor = bgcolor = c;
}

void Adafruit_GFX::setTextColor(uint16_t c, uint16_t b)
{
    fgcolor = c;
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

void Adafruit_GFX::setFont(const GFXfont *f)
{
    if (f)
    {            // Font struct pointer passed in?
        if (!gfxFont)
        { // And no current font struct?
          // Switching from classic to new font behavior.
          // Move cursor pos down 6 pixels so it's on baseline.
          //cursor_y += 6;
        }
    }
    else if (gfxFont)
    { // NULL passed.  Current font struct defined?
      // Switching from new to classic font behavior.
      // Move cursor pos up 6 pixels so it's at top-left of char.
      //cursor_y -= 6;
    }
    gfxFont = (GFXfont *) f;
    if (gfxFont)
    {
        yAdvance = gfxFont->yAdvance;
        if (gfxFont->xAdvance != 0)
        {
            xAdvance = gfxFont->xAdvance;
        }
    }
}

uint8_t Adafruit_GFX::printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
    va_end(args);

}

uint8_t Adafruit_GFX::printf(uint8_t x, uint8_t y, const char *fmt, ...)
{
    setCursor(x, y);
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
    va_end(args);
}


uint8_t Adafruit_GFX::printf(const char *fmt, va_list args)
{
    ee_vsprintf(buf, fmt, args);
    uint8_t len = 0;
    size_t size = strlen(buf);
    char* p = buf;
    while (size--)
    {
        writeChar(*p++);
    }
    return size;

}

void Adafruit_GFX::drawBmp(int16_t x, int16_t y, const uint8_t *bmp)
{
    WINBMPFILEHEADER* hdr1 = (WINBMPFILEHEADER*) bmp;
    WIN3XBITMAPHEADER* hdr2 = (WIN3XBITMAPHEADER*) &bmp[sizeof(WINBMPFILEHEADER)];

    if (hdr2->BitsPerPixel != 4 && hdr2->BitsPerPixel != 8 && hdr2->BitsPerPixel != 16)
    {
        printf(0, 0, "Not supported color depth: %d", hdr2->BitsPerPixel);
        return;
    }

    const uint16_t* bitmap16 = (uint16_t*) &bmp[hdr1->BitmapOffset];
    const uint8_t* bitmap8 = &bmp[hdr1->BitmapOffset];

    uint16_t wread = hdr2->Width;
    uint16_t h = abs(hdr2->Height);

    if (hdr2->BitsPerPixel == 4)
    {
        if (wread / 8 * 8 != wread)
        {
            wread = (wread / 8 + 1) * 8;
        }
        wread /= 2;
    }
    else if ((hdr2->BitsPerPixel == 16) && hdr2->Width / 2 * 2 != hdr2->Width)
    {
        wread = (hdr2->Width / 2 + 1) * 2;
    }
    else if ((hdr2->BitsPerPixel == 8) && hdr2->Width / 4 * 4 != hdr2->Width)
    {
        wread = (hdr2->Width / 4 + 1) * 4;
    }

    //wread = 108;

    uint16_t c = 0;
    for (uint32_t j = 0; j < h; ++j)
    {
        uint32_t wtotal = 0;
        for (uint32_t i = 0; i < wread; ++i)
        {
            if (hdr2->BitsPerPixel == 4)
            {
                c = bitmap8[(hdr2->Height - j - 1) * wread + i];
                uint16_t c1 = c >> 4;
                uint16_t c2 = c & 0xf;

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
                    c = bitmap16[(hdr2->Height - j - 1) * wread + i];
                }
                else if (hdr2->BitsPerPixel == 8)
                {
                    c = bitmap8[(hdr2->Height - j - 1) * wread + i];
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




