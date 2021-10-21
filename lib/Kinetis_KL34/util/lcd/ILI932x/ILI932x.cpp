#include "ILI932x.h"

ILI932x::ILI932x(GPIO_PIN* cs, GPIO_PIN* rs, GPIO_PIN* wr, GPIO_PIN* rd,
        GPIO_PIN* rst, GPIO_PORT* dataPort) :
        Adafruit_GFX(TFTWIDTH, TFTHEIGHT) {

    _cs = cs;
    _rs = rs;
    _wr = wr;
    _rd = rd;
    _rst = rst;

    _dataPort = dataPort;

    for (int i = 0; i < 8; ++i)
    {
        GPIO_PIN p = GPIO_GetPin(dataPort, (GPIO_PIN_N)i);
        GPIO_SetupOut(&p);
    }

    GPIO_SetupOut(_cs);
    GPIO_SetupOut(_rs);
    GPIO_SetupOut(_wr);
    GPIO_SetupOut(_rd);
    GPIO_SetupOut(_rst);

    GPIO_SetPin(_cs);
    GPIO_SetPin(_rs);
    GPIO_SetPin(_wr);
    GPIO_SetPin(_rd);
    GPIO_SetPin(_rst);

}

uint8_t ILI932x::read8() {
    GPIO_ResetPin(_rd);
    //__nop();    __nop();     __nop();
    uint8_t rc = *_dataPort->GPIO_PDIR & 0xff;
    GPIO_SetPin(_rd);
    return rc;
}

void ILI932x::writeCommand(uint16_t addr)
{
    GPIO_ResetPin(_cs);

    // Set  Addr
    GPIO_ResetPin(_rs);

    *_dataPort->GPIO_PDOR = 0;
    GPIO_ResetPin(_wr);
    //__nop();    __nop();     __nop();
    GPIO_SetPin(_wr);

    *_dataPort->GPIO_PDOR = addr & 0xff;
    GPIO_ResetPin(_wr);
    GPIO_SetPin(_wr);

    GPIO_SetPin(_cs);
}

void ILI932x::writeRegister16(uint16_t addr, uint16_t data)
{
    GPIO_ResetPin(_cs);

    // Set  Addr
    GPIO_ResetPin(_rs);

    *_dataPort->GPIO_PDOR = 0;
    GPIO_ResetPin(_wr);
    //__nop();    __nop();     __nop();
    GPIO_SetPin(_wr);

    *_dataPort->GPIO_PDOR = addr & 0xff;
    GPIO_ResetPin(_wr);
    //__nop();    __nop();     __nop();
    GPIO_SetPin(_wr);

    // Set Data
    GPIO_SetPin(_rs);

    *_dataPort->GPIO_PDOR = data >> 8;
    GPIO_ResetPin(_wr);
    //__nop();    __nop();     __nop();
    GPIO_SetPin(_wr);

    *_dataPort->GPIO_PDOR = data;
    GPIO_ResetPin(_wr);
    //__nop();    __nop();     __nop();
    GPIO_SetPin(_wr);

    GPIO_SetPin(_cs);
}

uint16_t ILI932x::readRegister16(uint16_t addr)
{
    GPIO_ResetPin(_cs);

    // Set  Addr
    GPIO_ResetPin(_rs);

    write8(0);
    write8(addr);

    setReadDir();

    // Set Data
    GPIO_SetPin(_rs);

    uint16_t result = read8() << 8;
    result |= read8();

    setWriteDir();
    GPIO_SetPin(_cs);

    return result;
}

uint16_t ILI932x::readID()
{
    return readRegister16(0);
}


void ILI932x::init(void) {

    reset();
    //delay_ms(100); //50
    writeRegister16(ILI932X_START_OSC, 0x0001);
    delay_ms(1); //100

    writeRegister16(ILI932X_DRIV_OUT_CTRL, 0x0100);
    writeRegister16(ILI932X_DRIV_WAV_CTRL, 0x0700);
    writeRegister16(ILI932X_ENTRY_MOD, 0x1030);
    writeRegister16(ILI932X_RESIZE_CTRL, 0x0000);
    writeRegister16(ILI932X_DISP_CTRL2, 0x0202);
    writeRegister16(ILI932X_DISP_CTRL3, 0x0000);
    writeRegister16(ILI932X_DISP_CTRL4, 0x0000);
    writeRegister16(ILI932X_RGB_DISP_IF_CTRL1, 0x0);
    writeRegister16(ILI932X_FRM_MARKER_POS, 0x0);
    writeRegister16(ILI932X_RGB_DISP_IF_CTRL2, 0x0);
    writeRegister16(ILI932X_POW_CTRL1, 0x0000);
    writeRegister16(ILI932X_POW_CTRL2, 0x0007);
    writeRegister16(ILI932X_POW_CTRL3, 0x0000);
    writeRegister16(ILI932X_POW_CTRL4, 0x0000);
    delay_ms(1); //200

    writeRegister16(ILI932X_POW_CTRL1, 0x1690);
    writeRegister16(ILI932X_POW_CTRL2, 0x0227);
    delay_ms(1); //100

    writeRegister16(ILI932X_POW_CTRL3, 0x001A);
    delay_ms(1); //100

    writeRegister16(ILI932X_POW_CTRL4, 0x1800);
    writeRegister16(ILI932X_POW_CTRL7, 0x002A);
    delay_ms(100); //100

    writeRegister16(ILI932X_GAMMA_CTRL1, 0x0000);
    writeRegister16(ILI932X_GAMMA_CTRL2, 0x0000);
    writeRegister16(ILI932X_GAMMA_CTRL3, 0x0000);
    writeRegister16(ILI932X_GAMMA_CTRL4, 0x0206);
    writeRegister16(ILI932X_GAMMA_CTRL5, 0x0808);
    writeRegister16(ILI932X_GAMMA_CTRL6, 0x0007);
    writeRegister16(ILI932X_GAMMA_CTRL7, 0x0201);
    writeRegister16(ILI932X_GAMMA_CTRL8, 0x0000);
    writeRegister16(ILI932X_GAMMA_CTRL9, 0x0000);
    writeRegister16(ILI932X_GAMMA_CTRL10, 0x0000);

    writeRegister16(ILI932X_GRAM_HOR_AD, 0x0000);
    writeRegister16(ILI932X_GRAM_VER_AD, 0x0000);
    writeRegister16(ILI932X_HOR_START_AD, 0x0000);
    writeRegister16(ILI932X_HOR_END_AD, 0x00EF);
    writeRegister16(ILI932X_VER_START_AD, 0X0000);
    writeRegister16(ILI932X_VER_END_AD, 0x013F);
    writeRegister16(ILI932X_GATE_SCAN_CTRL1, 0xA700);
    writeRegister16(ILI932X_GATE_SCAN_CTRL2, 0x0003);
    writeRegister16(ILI932X_GATE_SCAN_CTRL3, 0x0000);
    writeRegister16(ILI932X_PANEL_IF_CTRL1, 0X0010);
    writeRegister16(ILI932X_PANEL_IF_CTRL2, 0X0000);
    writeRegister16(ILI932X_PANEL_IF_CTRL3, 0X0003);
    writeRegister16(ILI932X_PANEL_IF_CTRL4, 0X1100);
    writeRegister16(ILI932X_PANEL_IF_CTRL5, 0X0000);
    writeRegister16(ILI932X_PANEL_IF_CTRL6, 0X0000);
    writeRegister16(ILI932X_DISP_CTRL1, 0x0133);

}


void ILI932x::flood(uint16_t color, uint32_t len)
{
    uint16_t blocks;
    uint8_t i, hi = color >> 8, lo = color;

    GPIO_ResetPin(_cs);

    //command
    GPIO_ResetPin(_rs);

    write8(0x00); // High byte of GRAM register...
    write8(ILI932X_RW_GRAM); // Write data to GRAM

    // Write first pixel normally, decrement counter by 1
    GPIO_SetPin(_rs);
    write8(hi);
    write8(lo);
    len--;

    blocks = (uint16_t) (len / 64); // 64 pixels/block
    if (hi == lo) {
        // High and low bytes are identical.  Leave prior data
        // on the port(s) and just toggle the write strobe.
        while (blocks--) {
            i = 16; // 64 pixels/block / 4 pixels/pass
            do {
                for (uint8_t s = 0; s < 8; ++s) {
                    GPIO_ResetPin(_wr);
                    GPIO_SetPin(_wr);
                }
            } while (--i);
        }
        // Fill any remaining pixels (1 to 64)
        for (i = (uint8_t) len & 63; i--;) {
            for (uint8_t i = 0; i < 2; ++i)
            {
                GPIO_ResetPin(_wr);
                //__nop(); __nop(); __nop();
                GPIO_SetPin(_wr);
            }
        }
    } else {
        while (blocks--) {
            i = 16; // 64 pixels/block / 4 pixels/pass
            do {
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
        for (i = (uint8_t) len & 63; i--;) {
            write8(hi);
            write8(lo);
        }
    }
    GPIO_SetPin(_cs);

}


void ILI932x::reset()
{
    GPIO_SetPin(_cs);
    GPIO_SetPin(_wr);
    GPIO_SetPin(_rd);

    // GPIO_SetPin(_rst);
    // delay_ms(2); // 2

    //GPIO_ResetPin(_rst);
    //delay_ms(1); // 20

    GPIO_SetPin(_rst);
    //delay_ms(1); // 50

    setWriteDir();
}

void ILI932x::setRotation(uint8_t x)
{
    // Call parent rotation func first -- sets up rotation flags, etc.
    Adafruit_GFX::setRotation(x);
    // Then perform hardware-specific rotation operations...

    uint16_t t;
    switch (rotation) {
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
    writeRegister16(ILI932X_ENTRY_MOD, t); // MADCTL
    setAddrWindow(0, 0, _width - 1, _height - 1);
}

void ILI932x::fillScreen(uint16_t color)
{
    uint16_t x, y;
    switch (rotation) {
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
    writeRegister16(ILI932X_GRAM_HOR_AD, x);
    writeRegister16(ILI932X_GRAM_VER_AD, y);
    flood(color, (long) TFTWIDTH * (long) TFTHEIGHT);
}

void ILI932x::drawFastHLine(int16_t x, int16_t y, int16_t length,
        uint16_t color) {
    int16_t x2;

    // Initial off-screen clipping
    if ((length <= 0) || (y < 0) || (y >= _height) || (x >= _width)
            || ((x2 = (x + length - 1)) < 0))
        return;

    if (x < 0) {        // Clip left
        length += x;
        x = 0;
    }
    if (x2 >= _width) { // Clip right
        x2 = _width - 1;
        length = x2 - x + 1;
    }

    setAddrWindow(x, y, x2, y);
    flood(color, length);

    setAddrWindow(0, 0, _width - 1, _height - 1);
}

void ILI932x::drawFastVLine(int16_t x, int16_t y, int16_t length,
        uint16_t color) {
    int16_t y2;

    // Initial off-screen clipping
    if ((length <= 0) || (x < 0) || (x >= _width) || (y >= _height)
            || ((y2 = (y + length - 1)) < 0))
        return;
    if (y < 0) {         // Clip top
        length += y;
        y = 0;
    }
    if (y2 >= _height) { // Clip bottom
        y2 = _height - 1;
        length = y2 - y + 1;
    }

    setAddrWindow(x, y, x, y2);
    flood(color, length);
    setAddrWindow(0, 0, _width - 1, _height - 1);
}

void ILI932x::drawPixel(int16_t x, int16_t y, uint16_t color) {
    // Clip
    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
        return;

    int16_t t;
    switch (rotation) {
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

    writeRegister16(ILI932X_GRAM_HOR_AD, x);
    writeRegister16(ILI932X_GRAM_VER_AD, y);
    writeRegister16(ILI932X_RW_GRAM, color);
}

void ILI932x::fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h,
        uint16_t fillcolor)
{
    int16_t x2, y2;

    // Initial off-screen clipping
    if ((w <= 0) || (h <= 0) || (x1 >= _width) || (y1 >= _height)
            || ((x2 = x1 + w - 1) < 0) || ((y2 = y1 + h - 1) < 0))
        return;
    if (x1 < 0) { // Clip left
        w += x1;
        x1 = 0;
    }
    if (y1 < 0) { // Clip top
        h += y1;
        y1 = 0;
    }
    if (x2 >= _width) { // Clip right
        x2 = _width - 1;
        w = x2 - x1 + 1;
    }
    if (y2 >= _height) { // Clip bottom
        y2 = _height - 1;
        h = y2 - y1 + 1;
    }

    setAddrWindow(x1, y1, x2, y2);
    flood(fillcolor, (uint32_t) w * (uint32_t) h);
    setAddrWindow(0, 0, _width - 1, _height - 1);
}

void ILI932x::setAddrWindow(int x1, int y1, int x2, int y2)
{
    int x, y, t;
    switch (rotation) {
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

    writeRegister16(ILI932X_HOR_START_AD, x1);
    writeRegister16(ILI932X_HOR_END_AD, x2);

    writeRegister16(ILI932X_VER_START_AD, y1);
    writeRegister16(ILI932X_VER_END_AD, y2);

    writeRegister16(ILI932X_GRAM_HOR_AD, x1);
    writeRegister16(ILI932X_GRAM_VER_AD, y1);

 }
