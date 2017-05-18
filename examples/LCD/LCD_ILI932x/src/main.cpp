
#include <gpio.h>
#include <gpio_cpp.h>
#include <rcc.h>
#include <RCC_Helper.h>
#include <stdint-gcc.h>
#include <ST7781/SWTFT.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"


#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//#include "Adafruit_GFX.h"
#include "932x/registers.h"
#define TFTWIDTH   240
#define TFTHEIGHT  320
#define TFTLCD_DELAY 0xFF
#include <stdio.h>

#include "font.c"
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


#define znew (z=36969*(z&65535)+(z>>16))
#define wnew (w=18000*(w&65535)+(w>>16))
#define MWC ((znew<<16)+wnew )
#define SHR3 (jsr^=(jsr<<17), jsr^=(jsr>>13), jsr^=(jsr<<5))
#define CONG (jcong=69069*jcong+1234567)
//#define FIB ((b=a+b),(a=b-a))
#define KISS ((MWC^CONG)+SHR3)
//#define LFIB4 (c++,t[c]=t[c]+t[UC(c+58)]+t[UC(c+119)]+t[UC(c+178)])
//#define SWB (c++,bro=(x<y),t[c]=(x=t[UC(c+34)])-(y=t[UC(c+19)]+bro))
//#define UNI (KISS*2.328306e-10)
//#define VNI ((long) KISS)*4.656613e-10
//#define UC (unsigned char) /*a cast operation*/
typedef unsigned long UL;
/* Global static variables: */
static UL z=362436069, w=521288629, jsr=123456789, jcong=380116160;
static UL a=224466889, b=7584631;//, t[256];
/* Use random seeds to reset z,w,jsr,jcong,a,b, and the table
t[256]*/
//static UL x=0,y=0,bro; static unsigned char c=0;
/* Example procedure to set the table, using KISS: */
//void settable(UL i1,UL i2,UL i3,UL i4,UL i5, UL i6)
//{ int i; z=i1;w=i2,jsr=i3; jcong=i4; a=i5; b=i6;
//for(i=0;i<256;i=i+1) t[i]=KISS;
//}


//////////////////////
// GFX private vars:


//////////////////////

static const uint16_t ILI932x_regValues[]  = {
  ILI932X_START_OSC        , 0x0001, // Start oscillator
  TFTLCD_DELAY             , 100,//50,     // 50 millisecond delay
  ILI932X_DRIV_OUT_CTRL    , 0x0100,
  ILI932X_DRIV_WAV_CTRL    , 0x0700,
  ILI932X_ENTRY_MOD        , 0x1030,
  ILI932X_RESIZE_CTRL      , 0x0000,
  ILI932X_DISP_CTRL2       , 0x0202,
  ILI932X_DISP_CTRL3       , 0x0000,
  ILI932X_DISP_CTRL4       , 0x0000,
  ILI932X_RGB_DISP_IF_CTRL1, 0x0,
  ILI932X_FRM_MARKER_POS   , 0x0,
  ILI932X_RGB_DISP_IF_CTRL2, 0x0,
  ILI932X_POW_CTRL1        , 0x0000,
  ILI932X_POW_CTRL2        , 0x0007,
  ILI932X_POW_CTRL3        , 0x0000,
  ILI932X_POW_CTRL4        , 0x0000,
  TFTLCD_DELAY             , 200,
  ILI932X_POW_CTRL1        , 0x1690,
  ILI932X_POW_CTRL2        , 0x0227,
  TFTLCD_DELAY             , 100,//50,
  ILI932X_POW_CTRL3        , 0x001A,
  TFTLCD_DELAY             , 100,//50,
  ILI932X_POW_CTRL4        , 0x1800,
  ILI932X_POW_CTRL7        , 0x002A,
  TFTLCD_DELAY             , 100,//50,
  ILI932X_GAMMA_CTRL1      , 0x0000,
  ILI932X_GAMMA_CTRL2      , 0x0000,
  ILI932X_GAMMA_CTRL3      , 0x0000,
  ILI932X_GAMMA_CTRL4      , 0x0206,
  ILI932X_GAMMA_CTRL5      , 0x0808,
  ILI932X_GAMMA_CTRL6      , 0x0007,
  ILI932X_GAMMA_CTRL7      , 0x0201,
  ILI932X_GAMMA_CTRL8      , 0x0000,
  ILI932X_GAMMA_CTRL9      , 0x0000,
  ILI932X_GAMMA_CTRL10     , 0x0000,
  ILI932X_GRAM_HOR_AD      , 0x0000,
  ILI932X_GRAM_VER_AD      , 0x0000,
  ILI932X_HOR_START_AD     , 0x0000,
  ILI932X_HOR_END_AD       , 0x00EF,
  ILI932X_VER_START_AD     , 0X0000,
  ILI932X_VER_END_AD       , 0x013F,
  ILI932X_GATE_SCAN_CTRL1  , 0xA700, // Driver Output Control (R60h)
  ILI932X_GATE_SCAN_CTRL2  , 0x0003, // Driver Output Control (R61h)
  ILI932X_GATE_SCAN_CTRL3  , 0x0000, // Driver Output Control (R62h)
  ILI932X_PANEL_IF_CTRL1   , 0X0010, // Panel Interface Control 1 (R90h)
  ILI932X_PANEL_IF_CTRL2   , 0X0000,
  ILI932X_PANEL_IF_CTRL3   , 0X0003,
  ILI932X_PANEL_IF_CTRL4   , 0X1100,
  ILI932X_PANEL_IF_CTRL5   , 0X0000,
  ILI932X_PANEL_IF_CTRL6   , 0X0000,
  ILI932X_DISP_CTRL1       , 0x0133, // Main screen turn on
};

class TestLCD// : public Adafruit_GFX
{
private:
    GPIO_PIN* _cs, *_rs, *_wr, *_rd, *_rst;
    GPIO_PORT* _dataPort;

    uint32_t _odr_addr;
    uint32_t _bsrr_addr; //set
    uint32_t _brr_addr; //reset

    uint16_t _csMask, _rsMask, _wrMask, _rdMask;

    //GFX
    const int16_t WIDTH, HEIGHT; // This is the 'raw' display w/h - never changes
    int16_t _width, _height, // Display w/h as modified by current rotation
            cursor_x, cursor_y;
    uint16_t textcolor, textbgcolor;
    uint8_t textsize, rotation;
    bool wrap;   // If set, 'wrap' text at right edge of display
    bool _cp437; // If set, use correct CP437 charset (default is off)


public:
    TestLCD(GPIO_PIN* cs, GPIO_PIN* rs, GPIO_PIN* wr, GPIO_PIN* rd, GPIO_PIN* rst, GPIO_PORT* dataPort, GPIO_PORT* ctrlPort)
    :   WIDTH(TFTWIDTH), HEIGHT(TFTHEIGHT)
//:Adafruit_GFX(TFTWIDTH, TFTHEIGHT)
    {
        _cs = cs;
        _rs = rs;
        _wr = wr;
        _rd = rd;
        _rst = rst;
        _dataPort = dataPort;

        _odr_addr = (uint32_t)_dataPort->GetGPIO_ODR();
        _bsrr_addr = (uint32_t)ctrlPort->GetGPIO_BSRR();
        _brr_addr = (uint32_t)ctrlPort->GetGPIO_BRR();

        _csMask = 1<<_cs->GetPinNumber();
        _rsMask = 1<<_rs->GetPinNumber();
        _wrMask = 1<<_wr->GetPinNumber();
        _rdMask = 1<<_rd->GetPinNumber();


        _cs->Set(); // Set all control bits to idle state
        _wr->Set();
        _rd->Set();
        _rs->Set();
        if (_rst)
        {
            _rst->Set();
        }

        setWriteDir(); // Set up LCD data port(s) for WRITE operations



        _width = WIDTH;
        _height = HEIGHT;
        rotation = 0;
        cursor_y = cursor_x = 0;
        textsize = 1;
        textcolor = textbgcolor = 0xFFFF;
        wrap = true;
        _cp437 = false;


        //reset();
    }


    void init(void)
    {
        uint16_t a, d;

        uint8_t i = 0;
        while (i < sizeof(ILI932x_regValues) / sizeof(uint16_t))
        {
            a = ILI932x_regValues[i++];
            d = ILI932x_regValues[i++];
            if (a == TFTLCD_DELAY)
            {
                delay(d);
            }
            else
                writeRegister16(a, d);
        }

        setRotation(rotation);
        //TODO duplicate call:
        //setAddrWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1);
    }

private:


    void GFX_setRotation(uint8_t x)
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



    void setWriteDir()
    {
       *_dataPort->GetGPIO_CR() &= ~(0xffffffff);
       *_dataPort->GetGPIO_CR() |= 0x33333333; //CNF=0(out PP) MODE=3(fast)
    }

    void setReadDir()
    {
        *_dataPort->GetGPIO_CR() &= ~(0xffffffff);
        //floating
        *_dataPort->GetGPIO_CR() |= 0x44444444; //CNF=1(Floating Input) MODE = 0 (input)


        //pulldown
//        *_dataPort->GetGPIO_CR() |= 0x88888888;
//        uint16_t out = _dataPort->GetOutput();
//        out &= ~0xff;

    }

    uint8_t read8()
    {
        _rd->Reset();
        delay_us(10);
        uint8_t rc = GPIOA::GetInstance()->GetInput() & 0xff;
        _rd->Set();
        return rc;
    }


    inline void write8(uint8_t d)
    {
        __asm volatile(

                " str  %[data], [%[odr]]          \n\t"
                " str  %[wr],    [%[brr]]    \n\t"
              //  "nop\n\t; nop\n\t; nop\n\t"
                " str  %[wr],    [%[bsrr]]      \n\t"

                :: [brr]   "r" (_brr_addr),
                [bsrr]     "r" (_bsrr_addr),
                [odr]     "r" (_odr_addr),
                [wr]       "r" (_wrMask),
                [data]    "r" (d)
           );
    }


    void writeRegister16(uint16_t addr, uint16_t data)
    {
        __asm volatile(

                " str  %[cs],    [%[brr]]    \n"

                " str  %[rs],    [%[brr]]    \n\t"

                " str  %[a_hi], [%[odr]]          \n\t"
                " str  %[wr],    [%[brr]]    \n\t"
              //  "nop\n\t; nop\n\t; nop\n\t"
                " str  %[wr],    [%[bsrr]]      \n\t"

                " str  %[a_lo], [%[odr]]          \n\t"
                " str  %[wr],    [%[brr]]    \n\t"
              //  "nop\n\t; nop\n\t; nop\n\t"
                " str  %[wr],    [%[bsrr]]      \n\t"



                " str  %[rs],    [%[bsrr]]    \n\t"


                " str  %[d_hi], [%[odr]]          \n\t"
                " str  %[wr],    [%[brr]]    \n\t"
              //  "nop\n\t; nop\n\t; nop\n\t"
                " str  %[wr],    [%[bsrr]]      \n\t"

                " str  %[d_lo], [%[odr]]          \n\t"
                " str  %[wr],    [%[brr]]    \n\t"
              //  "nop\n\t; nop\n\t; nop\n\t"
                " str  %[wr],    [%[bsrr]]      \n\t"

                " str  %[cs],    [%[bsrr]]    \n\t"


                :: [brr]   "r" (_brr_addr),
                [bsrr]     "r" (_bsrr_addr),
                [odr]     "r" (_odr_addr),
                [cs]       "r" (_csMask),
                [wr]       "r" (_wrMask),
                [rs]       "r" (_rsMask),
                [a_hi]    "r" (0),
                [a_lo]    "r" (addr),
                [d_hi]    "r" (data>>8),
                [d_lo]    "r" (data)
           );
    }




    // Sets the LCD address window (and address counter, on 932X).
    // Relevant to rect/screen fills and H/V lines.  Input coordinates are
    // assumed pre-sorted (e.g. x2 >= x1).
    void setAddrWindow(int x1, int y1, int x2, int y2)
    {
        // Values passed are in current (possibly rotated) coordinate
        // system.  932X requires hardware-native coords regardless of
        // MADCTL, so rotate inputs as needed.  The address counter is
        // set to the top-left corner -- although fill operations can be
        // done in any direction, the current screen rotation is applied
        // because some users find it disconcerting when a fill does not
        // occur top-to-bottom.
        int x, y, t;
        switch (rotation)
        {
        default:
            x = x1;
            y = y1;
            break;
        case 1:
            t = y1;
            y1 = x1;
            x1 = TFTWIDTH - 1 - y2;
            y2 = x2;
            x2 = TFTWIDTH - 1 - t;
            x = x2;
            y = y1;
            break;
        case 2:
            t = x1;
            x1 = TFTWIDTH - 1 - x2;
            x2 = TFTWIDTH - 1 - t;
            t = y1;
            y1 = TFTHEIGHT - 1 - y2;
            y2 = TFTHEIGHT - 1 - t;
            x = x2;
            y = y2;
            break;
        case 3:
            t = x1;
            x1 = y1;
            y1 = TFTHEIGHT - 1 - x2;
            x2 = y2;
            y2 = TFTHEIGHT - 1 - t;
            x = x1;
            y = y2;
            break;
        }
        writeRegister16(0x0050, x1); // Set address window
        writeRegister16(0x0051, x2);
        writeRegister16(0x0052, y1);
        writeRegister16(0x0053, y2);
        writeRegister16(0x0020, x); // Set address counter to top left
        writeRegister16(0x0021, y);

    }

    // Fast block fill operation for fillScreen, fillRect, H/V line, etc.
    // Requires setAddrWindow() has previously been called to set the fill
    // bounds.  'len' is inclusive, MUST be >= 1.
    void flood(uint16_t color, uint32_t len)
    {
        uint16_t blocks;
        uint8_t i, hi = color >> 8, lo = color;
        static volatile uint32_t* brr;
        static volatile uint32_t* bsrr;
        static GPIO_PIN_N w_pin;

        if (brr == 0)
        {
            brr = _wr->GetPort()->GetGPIO_BRR();
            bsrr = _wr->GetPort()->GetGPIO_BSRR();
            w_pin = _wr->GetPinNumber();
        }

        _cs->Reset();

        //command
        _rs->Reset();
        write8(0x00); // High byte of GRAM register...
        write8(0x22); // Write data to GRAM

        // Write first pixel normally, decrement counter by 1
        _rs->Set(); //data
        write8(hi);
        write8(lo);
        len--;

        blocks = (uint16_t) (len / 64); // 64 pixels/block
        if (hi == lo)
        {
            // High and low bytes are identical.  Leave prior data
            // on the port(s) and just toggle the write strobe.
            while (blocks--)
            {
                i = 16; // 64 pixels/block / 4 pixels/pass
                do
                {
                    for (uint8_t s = 0; s < 8; ++s)
                    {
                        //_wr->Reset();
                        *brr = 1 << w_pin;

                        //_wr->Set();
                        *bsrr = 1 << w_pin;
                    }
                } while (--i);
            }
            // Fill any remaining pixels (1 to 64)
            for (i = (uint8_t) len & 63; i--;)
            {
//                _wr->Reset();
//                _wr->Set();
//                _wr->Reset();
//                _wr->Set();

                *brr = 1 << w_pin;
                *bsrr = 1 << w_pin;
                *brr = 1 << w_pin;
                *bsrr = 1 << w_pin;


            }
        }
        else
        {
            while (blocks--)
            {
                i = 16; // 64 pixels/block / 4 pixels/pass
                do
                {
                    write8(hi);
                    write8(lo);
                    write8(hi);
                    write8(lo);
                    write8(hi);
                    write8(lo);
                    write8(hi);
                    write8(lo);
                } while (--i);
            }
            for (i = (uint8_t) len & 63; i--;)
            {
                write8(hi);
                write8(lo);
            }
        }
        _cs->Set();
    }



    // Pass 8-bit (each) R,G,B, get back 16-bit packed color
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }




    // Issues 'raw' an array of 16-bit color values to the LCD; used
    // externally by BMP examples.  Assumes that setWindowAddr() has
    // previously been set to define the bounds.  Max 255 pixels at
    // a time (BMP examples read in small chunks due to limited RAM).
    void pushColors(uint16_t *data, uint8_t len, bool first)
    {
        uint16_t color;
        uint8_t hi, lo;

        _cs->Reset();

        if (first == true)
        { // Issue GRAM write command only on first call
            _rs->Reset();

            write8(0x00);
            write8(0x2C);
        }

        _rs->Set();

        while (len--)
        {
            color = *data++;
            hi = color >> 8; // Don't simplify or merge these
            lo = color;      // lines, there's macro shenanigans
            write8(hi);         // going on.
            write8(lo);
        }
        _cs->Set();
    }



public:


    uint16_t readID()
    {
        setWriteDir();

        _cs->Reset();

        _rs->Reset();  //command

        //_rd->Set();
        //_wr->Set();

        write8(0);
        write8(0);

        _cs->Set();

        /////read data
        setReadDir();

        _cs->Reset();
        //
        _rs->Set();  //data


        uint16_t  result = read8() << 8;
        //  //delay_us(10);
        result |= read8();

        _cs->Set();
        setWriteDir();

        return result;

    }


    void reset() {

        _cs->Set();
        //  _cd->Set();
        _wr->Set();
        _rd->Set();


        _rst->Reset();
        delay(2);
        _rst->Set();

        setWriteDir();
//        _rs->Set();  //data
//
//        _cs->Reset();
//        write8(0);
//        write8(0);
//        _cs->Set();
//
//        _cs->Reset();
//        write8(0);
//        write8(0);

        // Data transfer sync
        _cs->Reset();
        _rs->Reset();
        write8(0x00);
        // Three extra 0x00s
        for (uint8_t i = 0; i < 3; i++)
        {
            _wr->Reset();
            _wr->Set();
        }

        _cs->Set();
        delay(200);

    }

    //GFX
    void setTextColor(uint16_t c)
    {
        // For 'transparent' background, we'll set the bg
        // to the same as fg instead of using a flag
        textcolor = textbgcolor = c;
    }

    //GFX
    void setTextColor(uint16_t c, uint16_t b)
    {
        textcolor = c;
        textbgcolor = b;
    }

    void setRotation(uint8_t x)
    {

        // Call parent rotation func first -- sets up rotation flags, etc.
       GFX_setRotation(x);
        // Then perform hardware-specific rotation operations...

        //_cs->Reset();

        uint16_t t;
        switch (rotation)
        {
        default:
            t = 0x1030;
            break;
        case 1:
            t = 0x1028;
            break;
        case 2:
            t = 0x1000;
            break;
        case 3:
            t = 0x1018;
            break;
        }
        writeRegister16(0x0003, t); // MADCTL
        // For 932X, init default full-screen address window:
        setAddrWindow(0, 0, _width - 1, _height - 1);
    }

    void fillScreen(uint16_t color)
    {

        // For the 932X, a full-screen address window is already the default
        // state, just need to set the address pointer to the top-left corner.
        // Although we could fill in any direction, the code uses the current
        // screen rotation because some users find it disconcerting when a
        // fill does not occur top-to-bottom.
        uint16_t x, y;
        switch (rotation)
        {
        default:
            x = 0;
            y = 0;
            break;
        case 1:
            x = TFTWIDTH - 1;
            y = 0;
            break;
        case 2:
            x = TFTWIDTH - 1;
            y = TFTHEIGHT - 1;
            break;
        case 3:
            x = 0;
            y = TFTHEIGHT - 1;
            break;
        }
        //_cs->Reset();
        writeRegister16(0x0020, x);
        writeRegister16(0x0021, y);
        flood(color, (long) TFTWIDTH * (long) TFTHEIGHT);
    }

    void drawFastHLine(int16_t x, int16_t y, int16_t length, uint16_t color)
    {
        int16_t x2;

        // Initial off-screen clipping
        if ((length <= 0) || (y < 0) || (y >= _height) || (x >= _width) || ((x2 = (x + length - 1)) < 0))
            return;

        if (x < 0)
        {        // Clip left
            length += x;
            x = 0;
        }
        if (x2 >= _width)
        { // Clip right
            x2 = _width - 1;
            length = x2 - x + 1;
        }

        setAddrWindow(x, y, x2, y);
        flood(color, length);

        setAddrWindow(0, 0, _width - 1, _height - 1);
    }

    void drawFastVLine(int16_t x, int16_t y, int16_t length, uint16_t color)
    {
        int16_t y2;

        // Initial off-screen clipping
        if ((length <= 0) || (x < 0) || (x >= _width) || (y >= _height) || ((y2 = (y + length - 1)) < 0))
            return;
        if (y < 0)
        {         // Clip top
            length += y;
            y = 0;
        }
        if (y2 >= _height)
        { // Clip bottom
            y2 = _height - 1;
            length = y2 - y + 1;
        }

        setAddrWindow(x, y, x, y2);
        flood(color, length);
        setAddrWindow(0, 0, _width - 1, _height - 1);
    }

    //GFX
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
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

    void drawPixel(int16_t x, int16_t y, uint16_t color)
    {

        // Clip
        if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
            return;

        //_cs->Reset();

        int16_t t;
        switch (rotation)
        {
        case 1:
            t = x;
            x = TFTWIDTH - 1 - y;
            y = t;
            break;
        case 2:
            x = TFTWIDTH - 1 - x;
            y = TFTHEIGHT - 1 - y;
            break;
        case 3:
            t = x;
            x = y;
            y = TFTHEIGHT - 1 - t;
            break;
        }

        writeRegister16(0x0020, x);
        writeRegister16(0x0021, y);
        writeRegister16(0x0022, color);


        //_cs->Set();
    }

    void fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, uint16_t fillcolor)
    {
        int16_t x2, y2;

        // Initial off-screen clipping
        if ((w <= 0) || (h <= 0) || (x1 >= _width) || (y1 >= _height) || ((x2 = x1 + w - 1) < 0) || ((y2 = y1 + h - 1) < 0))
            return;
        if (x1 < 0)
        { // Clip left
            w += x1;
            x1 = 0;
        }
        if (y1 < 0)
        { // Clip top
            h += y1;
            y1 = 0;
        }
        if (x2 >= _width)
        { // Clip right
            x2 = _width - 1;
            w = x2 - x1 + 1;
        }
        if (y2 >= _height)
        { // Clip bottom
            y2 = _height - 1;
            h = y2 - y1 + 1;
        }

        setAddrWindow(x1, y1, x2, y2);
        flood(fillcolor, (uint32_t) w * (uint32_t) h);
        setAddrWindow(0, 0, _width - 1, _height - 1);
    }

    void setTextSize(uint8_t s)
    {
        textsize = (s > 0) ? s : 1;
    }

    inline void setCursor(int16_t x, int16_t y)
    {
        cursor_x = x;
        cursor_y = y;
    }

    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
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
                        drawPixel(x + i, y + j, color);
                    else
                    {  // big size
                        fillRect(x + (i * size), y + (j * size), size, size, color);
                    }
                }
                else if (bg != color)
                {
                    if (size == 1) // default size
                        drawPixel(x + i, y + j, bg);
                    else
                    {  // big size
                        fillRect(x + i * size, y + j * size, size, size, bg);
                    }
                }
                line >>= 1;
            }
        }
    }

    void write(uint8_t c)
    {
        if (c == '\n')
        {
            cursor_y += textsize * 8;
            cursor_x = 0;
        }
        else if (c == '\r')
        {
            // skip em
        }
        else
        {
            drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
            cursor_x += textsize * 6;
            if (wrap && (cursor_x > (_width - textsize * 6)))
            {
                cursor_y += textsize * 8;
                cursor_x = 0;
            }
        }
    }

    inline void write(const char *str)
    {
        if (str != NULL)
        {
            write((const uint8_t *) str, strlen(str));
        }
    }

    inline void write(const uint8_t *buffer, uint32_t size)
    {
        while (size--)
        {
            write(*buffer++);
        }

    }
    inline void write(const char *buffer, uint32_t size)
    {
        write((const uint8_t *) buffer, size);
    }

    int drawUnicode(unsigned int uniCode, int x, int y, int size)
    {

        if (size)
            uniCode -= 32;

        unsigned int width = 0;
        unsigned int height = 0;
        //unsigned int flash_address = 0;
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

        if (flash_address == 0)
        {
            return 0;
        }

        int w = (width + 7) / 8;
        int pX = 0;
        int pY = y;
        //int color = 0;
        int8_t line = 0;

        //fillRect(x,pY,width+gap,height,textbgcolor);

        for (unsigned int i = 0; i < height; i++)
        {
            if (textcolor != textbgcolor)
            {
                if (textsize == 1)
                    drawFastHLine(x, pY, width + gap, textbgcolor);
                else
                    fillRect(x, pY, (width + gap) * textsize, textsize, textbgcolor);
            }
            for (int k = 0; k < w; k++)
            {
                line = flash_address[w * i + k];

                //line = ~line;

                if (1)
                {
                    if (textsize == 1)
                    {
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
                    else
                    {
                        pX = x + k * 8 * textsize;
                        if (line & 0x80)
                            fillRect(pX, pY, textsize, textsize, textcolor);
                        if (line & 0x40)
                            fillRect(pX + textsize, pY, textsize, textsize, textcolor);
                        if (line & 0x20)
                            fillRect(pX + 2 * textsize, pY, textsize, textsize, textcolor);
                        if (line & 0x10)
                            fillRect(pX + 3 * textsize, pY, textsize, textsize, textcolor);
                        if (line & 0x8)
                            fillRect(pX + 4 * textsize, pY, textsize, textsize, textcolor);
                        if (line & 0x4)
                            fillRect(pX + 5 * textsize, pY, textsize, textsize, textcolor);
                        if (line & 0x2)
                            fillRect(pX + 6 * textsize, pY, textsize, textsize, textcolor);
                        if (line & 0x1)
                            fillRect(pX + 7 * textsize, pY, textsize, textsize, textcolor);
                    }
                }
            }
            pY += textsize;
        }
        return (width + gap) * textsize;        // x +
    }

    inline int drawString(char *string, int poX, int poY, int size)
    {
        int sumX = 0;

        while (*string)
        {
            int xPlus = drawUnicode(*string, poX, poY, size);
            sumX += xPlus;
            string++;
            poX += xPlus; /* Move cursor right       */
        }
        return sumX;
    }

};

/////////////////////////////////////////

void test()
{
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    GPIO_PIN* cs = portB->GetPin(GPIO_PIN0);
    cs->SetupGPIO_OutPP();
    cs->SetSpeedHigh();

    GPIO_PIN* rs = portB->GetPin(GPIO_PIN1);
    rs->SetupGPIO_OutPP();
    rs->SetSpeedHigh();

    GPIO_PIN* rd = portB->GetPin(GPIO_PIN2);
    rd->SetupGPIO_OutPP();
    rd->SetSpeedHigh();

    GPIO_PIN* wr = portB->GetPin(GPIO_PIN10);
    wr->SetupGPIO_OutPP();
    wr->SetSpeedHigh();

    GPIO_PIN* rst = portB->GetPin(GPIO_PIN11);
    rst->SetupGPIO_OutPP();
    rst->SetSpeedHigh();

//    _cs = cs;
//    _rs = cd;
//    _rd = rd;
//    _wr= wr;
//    _rst = rst;

    rst->Set();

    delay(500);
    TestLCD t(cs, rs, wr, rd, rst, portA, portB);
    t.reset();
    //delay(500);
    uint16_t id2 = t.readID();
    t.init();
    id2 = t.readID();
//    t.drawPixel(100, 100, BLACK);
//    t.drawPixel(100, 101, BLACK);
//    t.drawPixel(100, 102, BLACK);
//    t.drawPixel(100, 103, BLACK);
//    t.drawPixel(100, 104, BLACK);
//    t.drawPixel(100, 105, BLACK);
//    t.drawPixel(100, 106, BLACK);

    t.fillScreen(0xE0E1);
    t.fillScreen(0x0001);
    t.fillScreen(0xfffe);
    t.fillScreen(0x1234);

    //t.fillRect(50, 50, 100, 100, RED);
    t.setRotation(1);
    uint32_t i = 0xffffffff;
    char buf[32];
    char c = 48;
    t.setTextColor(0x9393, 0xE0E0);

    for (uint8_t  i = 0; i < 100; ++i)
    {
        t.drawPixel(10 + i, 100, 0);
    }


    //    while (1)
//    {
//
//        t.drawLine(KISS % 319, KISS % 239, KISS % 319, KISS % 239, 0x9393);
//
//        delay_us(10000);
//    }

    char* b[] = { "12121212121212121",
                  "89898989898989898",
                  "57302394855555555" };



    //t.setTextSize(2);

    while (1)
    {
        //t.drawChar(c, 100, 100, 7);
        //sprintf (buf, "%10lu", i);
//        t.drawString(b [ i % 3], 10, 10, 4);
//        t.drawString(b [ i % 3], 10, 60, 4);
//        t.drawString(b [ i % 3], 10, 110, 4);
//        t.drawString(b [ i % 3], 10, 160, 4);
//        t.drawString(b [ i++ % 3], 10, 200, 4);

        t.drawString(b [ i % 3], 10, 10, 6);
        t.drawString(b [ i % 3], 10, 100, 6);
        t.drawString(b [ i++ % 3], 10, 200, 6);



//        t.setCursor(10, 10);
//        t.write(b [ i % 3]);
//        t.setCursor(10, 60);
//        t.write(b [ i % 3]);
//        t.setCursor(10, 110);
//        t.write(b [ i % 3]);
//        t.setCursor(10, 160);
//        t.write(b [ i % 3]);
//        t.setCursor(10, 200);
//        t.write(b [ i++ % 3]);
    }
    while(1)
        ;

}


int main()
{
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
    //RCC_EnableHSI(true);
    test();

}
