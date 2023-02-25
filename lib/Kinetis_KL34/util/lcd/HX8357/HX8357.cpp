#include "HX8357.h"

HX8357::HX8357(GPIO_PIN* cs, GPIO_PIN* rs, GPIO_PIN* wr, GPIO_PIN* rd,
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
    setWriteDir();
}


uint8_t HX8357::read8() {
    GPIO_ResetPin(_rd);
    __nop();    __nop();     __nop();
    uint8_t rc = *_dataPort->GPIO_PDIR & 0xff;
    GPIO_SetPin(_rd);
    return rc;
}

void HX8357::writeCommand(uint8_t addr)
{
    GPIO_ResetPin(_cs);

    // Set  Addr
    GPIO_ResetPin(_rs);

    *_dataPort->GPIO_PDOR = addr;
    GPIO_ResetPin(_wr);
    __nop();    __nop();     __nop();
    GPIO_SetPin(_wr);


    GPIO_SetPin(_cs);
}

void HX8357::writeData(uint8_t data)
{
    GPIO_ResetPin(_cs);

    // Set data
    GPIO_SetPin(_rs);

    *_dataPort->GPIO_PDOR = data;
    GPIO_ResetPin(_wr);
    __nop();    __nop();     __nop();
    GPIO_SetPin(_wr);


    GPIO_SetPin(_cs);

}

uint8_t HX8357::readData()
{
    GPIO_ResetPin(_cs);

    // Set Data
    GPIO_SetPin(_rs);

    GPIO_ResetPin(_rd);
    __nop();    __nop();     __nop();
    uint8_t rc = *_dataPort->GPIO_PDIR & 0xff;
    GPIO_SetPin(_rd);

    GPIO_SetPin(_cs);

    return rc;

}

uint16_t HX8357::readID()
{
    GPIO_ResetPin(_cs);

    // Set  Addr
    GPIO_ResetPin(_rs);

    write8(0xa);

    setReadDir();

    // Set Data
    GPIO_SetPin(_rs);

	//uint8_t b1 = read8();
	//uint8_t b2 = read8();

	GPIO_ResetPin(_cs);

	setWriteDir();
	return 0;
}


void HX8357::init(void) {

    reset();
    /*
    writeCommand(0xB9); // Enable extension command
    writeData(0xFF);
    writeData(0x83);
    writeData(0x57);
    delay_ms(50);

    writeCommand(0xB6); //Set VCOM voltage
    writeData(0x2C);    //0x52 for HSD 3.0"

    writeCommand(0x11); // Sleep off
    delay_ms(200);

    writeCommand(0x35); // Tearing effect on
    writeData(0x00);    // Added parameter

    writeCommand(0x3A); // Interface pixel format
    writeData(0x55);    // 16 bits per pixel

    //writeCommand(0xCC); // Set panel characteristic
    //writeData(0x09);    // S960>S1, G1>G480, R-G-B, normally black

    //writeCommand(0xB3); // RGB interface
    //writeData(0x43);
    //writeData(0x00);
    //writeData(0x06);
    //writeData(0x06);

    writeCommand(0xB1); // Power control
    writeData(0x00);
    writeData(0x15);
    writeData(0x0D);
    writeData(0x0D);
    writeData(0x83);
    writeData(0x48);


    writeCommand(0xC0); // Does this do anything?
    writeData(0x24);
    writeData(0x24);
    writeData(0x01);
    writeData(0x3C);
    writeData(0xC8);
    writeData(0x08);

    writeCommand(0xB4); // Display cycle
    writeData(0x02);
    writeData(0x40);
    writeData(0x00);
    writeData(0x2A);
    writeData(0x2A);
    writeData(0x0D);
    writeData(0x4F);

    writeCommand(0xE0); // Gamma curve
    writeData(0x00);
    writeData(0x15);
    writeData(0x1D);
    writeData(0x2A);
    writeData(0x31);
    writeData(0x42);
    writeData(0x4C);
    writeData(0x53);
    writeData(0x45);
    writeData(0x40);
    writeData(0x3B);
    writeData(0x32);
    writeData(0x2E);
    writeData(0x28);

    writeData(0x24);
    writeData(0x03);
    writeData(0x00);
    writeData(0x15);
    writeData(0x1D);
    writeData(0x2A);
    writeData(0x31);
    writeData(0x42);
    writeData(0x4C);
    writeData(0x53);
    writeData(0x45);
    writeData(0x40);
    writeData(0x3B);
    writeData(0x32);

    writeData(0x2E);
    writeData(0x28);
    writeData(0x24);
    writeData(0x03);
    writeData(0x00);
    writeData(0x01);

    writeCommand(0x36); // MADCTL Memory access control
    writeData(0x48);
    delay_ms(20);

    writeCommand(0x21); //Display inversion on
    delay_ms(20);

    writeCommand(0x29); // Display on

    delay_ms(120);
	*/

    writeCommand(0xd0);
    writeData(0x44);
    writeData(0x41);
    writeData(0x6);

    writeCommand(0xd1);
    writeData(0x40);
    writeData(0x10);

    writeCommand(0xd2);
    writeData(0x05);
    writeData(0x12);

    writeCommand(0xc0);
    writeData(0x14);
    writeData(0x3b);
    writeData(0x00);
    writeData(0x02);
    writeData(0x11);

    writeCommand(0xc5);
    writeData(0x0c);

    writeCommand(0xe9);
    writeData(0x01);

    writeCommand(0xEA);
    writeData(0x03);
    writeData(0x00);
    writeData(0x00);

    writeCommand(0xEB);
    writeData(0x40);
    writeData(0x54);
    writeData(0x26);
    writeData(0xdb);

    writeCommand(0xc8);
    writeData(0x00);
    writeData(0x15);
    writeData(0x00);
    writeData(0x22);
    writeData(0x00);
    writeData(0x08);
    writeData(0x77);
    writeData(0x26);
    writeData(0x66);
    writeData(0x22);
    writeData(0x04);
    writeData(0x00);

    writeCommand(HX8357_MADCTL);
    writeData(0xC0);

    writeCommand(HX8357_COLMOD);
    writeData(0x55);

    writeCommand(HX8357_PASET);
    writeData(0x00);
    writeData(0x00);
    writeData(0x01);
    writeData(0xDF);

    writeCommand(HX8357_CASET);
    writeData(0x00);
    writeData(0x00);
    writeData(0x01);
    writeData(0x3F);

    writeCommand(HX8357B_SETDISPMODE);
    writeData(0x00);

    writeCommand(HX8357_SLPOUT);
    delay_ms(120);

    writeCommand(HX8357_DISPON);
    delay_ms(10);

}


void HX8357::flood(uint16_t color, uint32_t len)
{
    uint16_t blocks;
    uint8_t i, hi = color >> 8, lo = color;

    writeCommand(HX8357_RAMWR);


    GPIO_ResetPin(_cs);

    //write8(0x00); // High byte of GRAM register...
    //write8(HX8357_GRAM_DATA_REG); // Write data to GRAM

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


void HX8357::reset()
{
    GPIO_SetPin(_cs);
    GPIO_SetPin(_wr);
    GPIO_SetPin(_rd);

    GPIO_SetPin(_rst);
    delay_ms(20);

    GPIO_ResetPin(_rst);
    delay_ms(10);

    GPIO_SetPin(_rst);
    delay_ms(10);

    setWriteDir();
}

void HX8357::setRotation(uint8_t x)
{
    // Call parent rotation func first -- sets up rotation flags, etc.
    Adafruit_GFX::setRotation(x);
    // Then perform hardware-specific rotation operations...

    uint16_t t;

    switch (rotation) {
      case 1:
        t = MADCTL_MY | MADCTL_MV | MADCTL_RGB;
        break;
      case 2:
        t = MADCTL_RGB;
        break;
      case 3:
        t = MADCTL_MX | MADCTL_MV | MADCTL_RGB;
        break;
      case 0:
      default:
        t = MADCTL_MX | MADCTL_MY | MADCTL_RGB;
        break;
    }
    writeCommand(HX8357_MADCTL);
    writeData(t);
    setAddrWindow(0, 0, _width - 1, _height - 1);
}

void HX8357::fillScreen(uint16_t color)
{
	setAddrWindow(0, 0, _width - 1, _height - 1);
	flood(color, (long) TFTWIDTH * (long) TFTHEIGHT);
}

void HX8357::drawFastHLine(int16_t x, int16_t y, int16_t length,
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

void HX8357::drawFastVLine(int16_t x, int16_t y, int16_t length,
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

void HX8357::drawPixel(int16_t x, int16_t y, uint16_t color) {
    // Clip
    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
        return;

    setAddrWindow(x, y, x+1, y+1);
    writeCommand(HX8357_RAMWR);
    writeData (color >> 8);
    writeData(color);
}

void HX8357::fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h,
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

void HX8357::setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    writeCommand(HX8357_CASET);
    writeData((x1 >> 8) & 0xff);
    writeData(x1 & 0xff);
    writeData((x2 >> 8) & 0xff);
    writeData(x2 & 0xff);

    writeCommand(HX8357_PASET);
    writeData((y1 >> 8) & 0xff);
    writeData(y1 & 0xff);
    writeData((y2 >> 8) & 0xff);
    writeData(y2 & 0xff);

}
