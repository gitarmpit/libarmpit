#include "ILI9225.h"

ILI9225::ILI9225(GPIO_PIN* cs, GPIO_PIN* rs, GPIO_PIN* wr, GPIO_PIN* rd,
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

uint8_t ILI9225::read8() {
    GPIO_ResetPin(_rd);
    //__nop();    __nop();     __nop();
    uint8_t rc = *_dataPort->GPIO_PDIR & 0xff;
    GPIO_SetPin(_rd);
    return rc;
}

void ILI9225::writeCommand(uint16_t addr)
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

void ILI9225::writeRegister16(uint16_t addr, uint16_t data)
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

uint16_t ILI9225::readRegister16(uint16_t addr)
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

uint16_t ILI9225::readID()
{
	return readRegister16(0);
}


void ILI9225::init(void) {

    reset();
    //delay_ms(100); //50
    writeRegister16(ILI9225_POWER_CTRL1, 0x0000); // Set SAP,DSTB,STB
    writeRegister16(ILI9225_POWER_CTRL2, 0x0000); // Set APON,PON,AON,VCI1EN,VC
    writeRegister16(ILI9225_POWER_CTRL3, 0x0000); // Set BT,DC1,DC2,DC3
    writeRegister16(ILI9225_POWER_CTRL4, 0x0000); // Set GVDD
    writeRegister16(ILI9225_POWER_CTRL5, 0x0000); // Set VCOMH/VCOML voltage
    delay_ms(1); //40

    // Power-on sequence
    writeRegister16(ILI9225_POWER_CTRL2, 0x0018); // Set APON,PON,AON,VCI1EN,VC
    writeRegister16(ILI9225_POWER_CTRL3, 0x6121); // Set BT,DC1,DC2,DC3
    writeRegister16(ILI9225_POWER_CTRL4, 0x006F); // Set GVDD   /*007F 0088 */
    writeRegister16(ILI9225_POWER_CTRL5, 0x495F); // Set VCOMH/VCOML voltage
    writeRegister16(ILI9225_POWER_CTRL1, 0x0800); // Set SAP,DSTB,STB
    delay_ms(1); //10

    writeRegister16(ILI9225_POWER_CTRL2, 0x103B); // Set APON,PON,AON,VCI1EN,VC
    delay_ms(1); //50

    writeRegister16(ILI9225_DRIVER_OUTPUT_CTRL, 0x011C); // set the display line number and display direction
    writeRegister16(ILI9225_LCD_AC_DRIVING_CTRL, 0x0100); // set 1 line inversion
    writeRegister16(ILI9225_DISP_CTRL1, 0x0000); // Display off
    writeRegister16(ILI9225_BLANK_PERIOD_CTRL1, 0x0808); // set the back porch and front porch
    writeRegister16(ILI9225_FRAME_CYCLE_CTRL, 0x1100); // set the clocks number per line
    writeRegister16(ILI9225_INTERFACE_CTRL, 0x0000); // CPU interface
    writeRegister16(ILI9225_OSC_CTRL, 0x0E01); // Set Osc  /*0e01*/
    writeRegister16(ILI9225_VCI_RECYCLING, 0x0020); // Set VCI recycling


    /* Set GRAM area */
    writeRegister16(ILI9225_GATE_SCAN_CTRL, 0x0000);         // 0x30
    writeRegister16(ILI9225_VERTICAL_SCROLL_CTRL1, TFTHEIGHT-1);  //0xDB 220-1
    writeRegister16(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);
    writeRegister16(ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000);

    //writeRegister16(ILI9225_PARTIAL_DRIVING_POS1, TFTHEIGHT-1);  //0xDB 220-1
    //writeRegister16(ILI9225_PARTIAL_DRIVING_POS2, 0x0000);

    // Write direction: Horizontal
    writeRegister16(ILI9225_ENTRY_MODE, 0x1030);
    //writeRegister16(ILI9225_ENTRY_MODE, 0x1028);
    //writeRegister16(ILI9225_ENTRY_MODE, 0x1000);
    //writeRegister16(ILI9225_ENTRY_MODE, 0x1018);

    writeRegister16(ILI9225_RAM_ADDR_SET1, 0x0000); // RAM Address 0x20
    writeRegister16(ILI9225_RAM_ADDR_SET2, 0x0000); // RAM Address 0x21

    // 0x36 0x37 0x38 0x39
    writeRegister16(ILI9225_HORIZONTAL_WINDOW_START, 0x0000);
    writeRegister16(ILI9225_HORIZONTAL_WINDOW_END, TFTWIDTH-1);
    writeRegister16(ILI9225_VERTICAL_WINDOW_START, 0x0000);
    writeRegister16(ILI9225_VERTICAL_WINDOW_END, TFTHEIGHT-1);

    /* Set GAMMA curve */
    writeRegister16(ILI9225_GAMMA_CTRL1, 0x0000);
    writeRegister16(ILI9225_GAMMA_CTRL2, 0x0808);
    writeRegister16(ILI9225_GAMMA_CTRL3, 0x080A);
    writeRegister16(ILI9225_GAMMA_CTRL4, 0x000A);
    writeRegister16(ILI9225_GAMMA_CTRL5, 0x0A08);
    writeRegister16(ILI9225_GAMMA_CTRL6, 0x0808);
    writeRegister16(ILI9225_GAMMA_CTRL7, 0x0000);
    writeRegister16(ILI9225_GAMMA_CTRL8, 0x0A00);
    writeRegister16(ILI9225_GAMMA_CTRL9, 0x0710);
    writeRegister16(ILI9225_GAMMA_CTRL10, 0x0710);

    //writeRegister16(ILI9225_DISP_CTRL1, 0x0012);
    //delay_ms(10); //50

    writeRegister16(ILI9225_DISP_CTRL1, 0x1017);
    // setRotation();

}


void ILI9225::flood(uint16_t color, uint32_t len)
{
    uint16_t blocks;
    uint8_t i, hi = color >> 8, lo = color;

    GPIO_ResetPin(_cs);

    //command
    GPIO_ResetPin(_rs);

    write8(0x00); // High byte of GRAM register...
    write8(ILI9225_GRAM_DATA_REG); // Write data to GRAM

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


void ILI9225::reset()
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

void ILI9225::setRotation(uint8_t x)
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
    writeRegister16(ILI9225_ENTRY_MODE, t); // MADCTL
    setAddrWindow(0, 0, _width - 1, _height - 1);
}

void ILI9225::fillScreen(uint16_t color)
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
    writeRegister16(ILI9225_RAM_ADDR_SET1, x);
    writeRegister16(ILI9225_RAM_ADDR_SET2, y);
    flood(color, (long) TFTWIDTH * (long) TFTHEIGHT);
}

void ILI9225::drawFastHLine(int16_t x, int16_t y, int16_t length,
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

void ILI9225::drawFastVLine(int16_t x, int16_t y, int16_t length,
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

void ILI9225::drawPixel(int16_t x, int16_t y, uint16_t color) {
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

    writeRegister16(ILI9225_RAM_ADDR_SET1, x);
    writeRegister16(ILI9225_RAM_ADDR_SET2, y);
    writeRegister16(ILI9225_GRAM_DATA_REG, color);
}

void ILI9225::fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h,
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

void ILI9225::setAddrWindow(int x1, int y1, int x2, int y2)
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

    writeRegister16(ILI9225_HORIZONTAL_WINDOW_START,x1);
    writeRegister16(ILI9225_HORIZONTAL_WINDOW_END,x2);

    writeRegister16(ILI9225_VERTICAL_WINDOW_START,y1);
    writeRegister16(ILI9225_VERTICAL_WINDOW_END,y2);

    writeRegister16(ILI9225_RAM_ADDR_SET1,x1);
    writeRegister16(ILI9225_RAM_ADDR_SET2,y1);

 }
