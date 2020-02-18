#ifndef _ILI932x_H
#define _ILI932x_H

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#include "Adafruit_GFX.h"
#include "932x_registers.h"
#define TFTWIDTH   240
#define TFTHEIGHT  320
#define TFTLCD_DELAY 0xFF

//////////////////////
static const uint16_t ILI932x_regValues[] = {
        ILI932X_START_OSC, 0x0001, // Start oscillator
        TFTLCD_DELAY, 100, //50,     // 50 millisecond delay
        ILI932X_DRIV_OUT_CTRL, 0x0100,
        ILI932X_DRIV_WAV_CTRL, 0x0700,
        ILI932X_ENTRY_MOD, 0x1030,
        ILI932X_RESIZE_CTRL, 0x0000,
        ILI932X_DISP_CTRL2, 0x0202,
        ILI932X_DISP_CTRL3, 0x0000,
        ILI932X_DISP_CTRL4, 0x0000,
        ILI932X_RGB_DISP_IF_CTRL1, 0x0,
        ILI932X_FRM_MARKER_POS, 0x0,
        ILI932X_RGB_DISP_IF_CTRL2, 0x0,
        ILI932X_POW_CTRL1, 0x0000,
        ILI932X_POW_CTRL2, 0x0007,
        ILI932X_POW_CTRL3, 0x0000,
        ILI932X_POW_CTRL4, 0x0000,
        TFTLCD_DELAY, 200,
        ILI932X_POW_CTRL1, 0x1690,
        ILI932X_POW_CTRL2, 0x0227,
        TFTLCD_DELAY, 100, //50,
        ILI932X_POW_CTRL3, 0x001A,
        TFTLCD_DELAY, 100, //50,
        ILI932X_POW_CTRL4, 0x1800,
        ILI932X_POW_CTRL7, 0x002A,
        TFTLCD_DELAY, 100, //50,
        ILI932X_GAMMA_CTRL1, 0x0000,
        ILI932X_GAMMA_CTRL2, 0x0000,
        ILI932X_GAMMA_CTRL3, 0x0000,
        ILI932X_GAMMA_CTRL4, 0x0206,
        ILI932X_GAMMA_CTRL5, 0x0808,
        ILI932X_GAMMA_CTRL6, 0x0007,
        ILI932X_GAMMA_CTRL7, 0x0201,
        ILI932X_GAMMA_CTRL8, 0x0000,
        ILI932X_GAMMA_CTRL9, 0x0000,
        ILI932X_GAMMA_CTRL10, 0x0000,
        ILI932X_GRAM_HOR_AD, 0x0000,
        ILI932X_GRAM_VER_AD, 0x0000,
        ILI932X_HOR_START_AD, 0x0000,
        ILI932X_HOR_END_AD, 0x00EF,
        ILI932X_VER_START_AD, 0X0000,
        ILI932X_VER_END_AD, 0x013F,
        ILI932X_GATE_SCAN_CTRL1, 0xA700, // Driver Output Control (R60h)
        ILI932X_GATE_SCAN_CTRL2, 0x0003, // Driver Output Control (R61h)
        ILI932X_GATE_SCAN_CTRL3, 0x0000, // Driver Output Control (R62h)
        ILI932X_PANEL_IF_CTRL1, 0X0010, // Panel Interface Control 1 (R90h)
        ILI932X_PANEL_IF_CTRL2, 0X0000,
        ILI932X_PANEL_IF_CTRL3, 0X0003,
        ILI932X_PANEL_IF_CTRL4, 0X1100,
        ILI932X_PANEL_IF_CTRL5, 0X0000,
        ILI932X_PANEL_IF_CTRL6, 0X0000,
        ILI932X_DISP_CTRL1, 0x0133, // Main screen turn on
};

extern "C" {
void pushColorsAsm(uint16_t* data, int len, volatile uint32_t* brr,
		volatile uint32_t* bsrr, volatile uint32_t* odr, uint16_t cs,
		uint16_t wr, uint16_t rs);
}

class ILI932x: public Adafruit_GFX {
private:
    GPIO_PIN* _cs, *_rs, *_wr, *_rd, *_rst;
    GPIO_PORT* _dataPort;

    volatile uint32_t* _odr_addr;
    volatile uint32_t* _bsrr_addr; //set
    volatile uint32_t* _brr_addr; //reset

    uint16_t _csMask, _rsMask, _wrMask, _rdMask;
    uint16_t _csrstMask, _rsrstMask, _wrstMask;

public:
    ILI932x(GPIO_PIN* cs, GPIO_PIN* rs, GPIO_PIN* wr, GPIO_PIN* rd,
            GPIO_PIN* rst, GPIO_PORT* dataPort, GPIO_PORT* ctrlPort);

    void init(void);

private:

     void setWriteDir() {
        *_dataPort->GetGPIO_CR() &= ~(0xffffffff);
        *_dataPort->GetGPIO_CR() |= 0x33333333; //CNF=0(out PP) MODE=3(fast)
    }

    void setReadDir() {
        *_dataPort->GetGPIO_CR() &= ~(0xffffffff);
        *_dataPort->GetGPIO_CR() |= 0x44444444; //CNF=1(Floating Input) MODE = 0 (input)
    }

    uint8_t read8();

    void __write8(uint8_t d);

    inline void write8(uint8_t d) {
        __asm volatile(
                "str  %[data], [%[odr]]   \n\t"
                "str  %[wr],   [%[brr]]   \n\t"
                "str  %[wr],   [%[bsrr]]  \n\t"
                :: [brr] "r" (_brr_addr),
                [bsrr] "r" (_bsrr_addr),
                [odr] "r" (_odr_addr),
                [wr] "r" (_wrMask),
                [data] "r" (d)
        );
    }

    void _writeRegister16(uint16_t addr, uint16_t data);
    void writeRegister16(uint16_t addr, uint16_t data);

    // Fast block fill operation for fillScreen, fillRect, H/V line, etc.
    // Requires setAddrWindow() has previously been called to set the fill
    // bounds.  'len' is inclusive, MUST be >= 1.
    void flood(uint16_t color, uint32_t len);

    // Pass 8-bit (each) R,G,B, get back 16-bit packed color
    inline uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }

public:

    uint16_t readID();
    void reset();
    void setRotation(uint8_t x);
    void fillScreen(uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t length, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t length, uint16_t color);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, uint16_t fillcolor);

    void startPushColors();

    // Call setWindowAddr() + startPushColors() first
    // No difference in speed, can just use the default one
    void pushColors(uint16_t *data, int len);
    void pushColors2(uint16_t *data, int len);
    inline void pushColors3(uint16_t *data, uint16_t len) {
        pushColorsAsm(data, len, _brr_addr, _bsrr_addr, _odr_addr, _csMask, _wrMask, _rsMask);
    }

    //TODO: private
    // Sets the LCD address window (and address counter, on 932X).
    // Relevant to rect/screen fills and H/V lines.  Input coordinates are
    // assumed pre-sorted (e.g. x2 >= x1).
    void setAddrWindow(int x1, int y1, int x2, int y2);
 };





#endif