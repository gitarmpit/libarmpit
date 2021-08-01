#if 0
#ifndef _LCD_H
#define _LCD_H

#define MCU_FREQ 100

#ifndef MCU_FREQ
#error  MCU_FREQ not defined
#endif

#define NOP0 ""
#define NOP1 "\tnop \n\t"
#define NOP2 "\tnop \n\t nop \n\t"
#define NOP3 "\tnop \n\t nop \n\t nop \n\t"
#define NOP4 "\tnop \n\t nop\n \t nop \n\t nop \n\t"

#if defined(STM32F4) || defined(STM32F2)
    #if MCU_FREQ > 190
        #define __DELAY__ NOP4
    #elif MCU_FREQ > 140
        #define __DELAY__ NOP3
    #else
        #define __DELAY__ NOP2
    #endif
#elif defined (STM32F1)
//  #define   USE_ASM   // Not much gain, probably not worth it even for F103
    #define __DELAY__ NOP0
#else
    #define __DELAY__ NOP4
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

#include "ILI932x.h"
#include "Adafruit_GFX.h"
#define TFTLCD_DELAY 0xFF

extern "C" {
void pushColorsAsm(uint16_t* data, int len, volatile uint32_t* brr,
        volatile uint32_t* bsrr, volatile uint32_t* odr, uint16_t cs,
        uint16_t wr, uint16_t rs);
}

class LCD: public Adafruit_GFX {
private:
    GPIO_PIN* _cs, *_rs, *_wr, *_rd, *_rst;
    GPIO_PORT* _dataPort;

    volatile uint32_t* _odr_addr;
    volatile uint32_t* _bsrr_addr; //set

    uint32_t _csMask, _rsMask, _wrMask;
    uint32_t _wrstMask;
#if defined (USE_ASM)
    uint32_t _csrstMask, _rsrstMask;
#endif

public:
    ILI932x(GPIO_PIN* cs, GPIO_PIN* rs, GPIO_PIN* wr, GPIO_PIN* rd,
            GPIO_PIN* rst, GPIO_PORT* dataPort);

    void init(void);

private:

     // Sets the LCD address window (and address counter, on 932X).
     // Relevant to rect/screen fills and H/V lines.  Input coordinates are
     // assumed pre-sorted (e.g. x2 >= x1).
     void setAddrWindow(int x1, int y1, int x2, int y2);

     void setWriteDir() {
#ifdef STM32F1
         //First 8 pins: F1 CR is 4 bits per pin, 32 bits!!!
        *_dataPort->GetGPIO_CR() &= ~(0xffffffff);
        *_dataPort->GetGPIO_CR() |= 0x33333333; //CNF=0(out PP) MODE=3(fast)
#else
        //First 8 pins: F4 Mode/speed is 2 bits per pin, 16 bits!!!
        *_dataPort->GetGPIO_MODER() |= 0x5555;  // First 8 pins: assumed we use pin 0-7
        *_dataPort->GetGPIO_OSPEEDR() |= 0xffff; // First 8 pins: assumed we use pin 0-7
        //*_dataPort->GetGPIO_OTYPER() = 0;
#endif
    }

    void setReadDir() {
#ifdef STM32F1
        *_dataPort->GetGPIO_CR() &= ~(0xffffffff);
        *_dataPort->GetGPIO_CR() |= 0x44444444; //CNF=1(Floating Input) MODE = 0 (input)
#else
        *_dataPort->GetGPIO_MODER() &= ~0xffff; // First 8 pins: assumed we use pin 0-7
#endif
    }

    uint8_t read8();

    // This is always assembly regardless of USE_ASM
    inline void write8(uint8_t d) {
        __asm volatile(
                "str  %[data], [%[odr]]   \n\t"
                "str  %[wrst],   [%[bsrr]]   \n\t"

                __DELAY__

                "str  %[wr], [%[bsrr]]   \n\t"
                ::
                [bsrr] "r" (_bsrr_addr),
                [odr]  "r" (_odr_addr),
                [wr]   "r" (_wrMask),
                [wrst] "r" (_wrstMask),
                [data] "r" (d)
        );
    }

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
    //inline void pushColors3(uint16_t *data, uint16_t len) {
    //    pushColorsAsm(data, len, _brr_addr, _bsrr_addr, _odr_addr, _csMask, _wrMask, _rsMask);
    //}

 };





#endif
#endif
