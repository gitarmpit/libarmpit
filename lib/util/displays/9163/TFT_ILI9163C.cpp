#include "TFT_ILI9163C.h"
#include "debug.h"
#include <string.h>

//TODO make the display class a singleton to avoid sharing this frambuffer
//had to make it a static buffer outside of class to make the class instances
//smaller so the stack or heap wouldn't overflow
static uint16_t _frameBuffer[_GRAMSIZE];


TFT_ILI9163C::TFT_ILI9163C(SPI* spi, GPIO_PIN* dcPin, GPIO_PIN* rstPin, GPIO_PIN* ssPin) :
        Adafruit_GFX(_TFTWIDTH, _TFTHEIGHT)
{
    _spi = spi;
    _dcPin = dcPin;
    _rstPin = rstPin;
    _ssPin = ssPin;
    bgcolor = BLACK;
    fgcolor = WHITE;

    memset (_frameBuffer, 0, sizeof(_frameBuffer));


    DR = _spi->GetDRAddr();
    SR = _spi->GetSRAddr();
    begin();
}

void TFT_ILI9163C::writecommand(uint8_t cmd)
{
    _dcPin->Reset();
    _ssPin->Reset();
    _spi->TransmitByte(cmd);
    _ssPin->Set();
}

void TFT_ILI9163C::writedata(uint8_t data)
{
    _dcPin->Set();
    _ssPin->Reset();
    _spi->TransmitByte(data);
    _ssPin->Set();
}

void TFT_ILI9163C::writedata16(uint16_t data)
{
    _dcPin->Set();
    _ssPin->Reset();
    _spi->TransmitByte(data >> 8);
    _spi->TransmitByte(data);

//    while (!(*SR & SPI_SR_TXE))
//        ;
//    *DR = data>>8;
//    while(!(*SR & SPI_SR_RXNE))
//        ;
//
//    while (!(*SR & SPI_SR_TXE))
//        ;
//    *DR = data;
//    while(!(*SR & SPI_SR_RXNE))
//        ;

    _ssPin->Set();
}

//called from ctor
void TFT_ILI9163C::begin(void)
{
    sleep = 0;
    _initError = 0;
    _ssPin->Reset();

    if (_rstPin != 0)
    {
        _rstPin->Set();
        delay(5);
        _rstPin->Reset();
        delay(5);
        _rstPin->Set();
        delay(5);
    }

    /*
     7) MY:  1(bottom to top), 0(top to bottom) 	Row Address Order
     6) MX:  1(R to L),        0(L to R)        	Column Address Order
     5) MV:  1(Exchanged),     0(normal)        	Row/Column exchange
     4) ML:  1(bottom to top), 0(top to bottom) 	Vertical Refresh Order
     3) RGB: 1(BGR), 		   0(RGB)           	Color Space
     2) MH:  1(R to L),        0(L to R)        	Horizontal Refresh Order
     1)
     0)

     MY, MX, MV, ML,RGB, MH, D1, D0
     0 | 0 | 0 | 0 | 1 | 0 | 0 | 0	//normal
     1 | 0 | 0 | 0 | 1 | 0 | 0 | 0	//Y-Mirror
     0 | 1 | 0 | 0 | 1 | 0 | 0 | 0	//X-Mirror
     1 | 1 | 0 | 0 | 1 | 0 | 0 | 0	//X-Y-Mirror
     0 | 0 | 1 | 0 | 1 | 0 | 0 | 0	//X-Y Exchange
     1 | 0 | 1 | 0 | 1 | 0 | 0 | 0	//X-Y Exchange, Y-Mirror
     0 | 1 | 1 | 0 | 1 | 0 | 0 | 0	//XY exchange
     1 | 1 | 1 | 0 | 1 | 0 | 0 | 0
     */
    _Mactrl_Data = 0; //0b00000000;
    _colorspaceData = __COLORSPC; //start with default data;
    init();
}

uint8_t TFT_ILI9163C::errorCode(void)
{
    return _initError;
}

//called from begin()
void TFT_ILI9163C::init()
{
    uint8_t i;
    writecommand(CMD_SWRESET); //software reset
    delay(5);

    writecommand(CMD_SLPOUT); //exit sleep
    delay(5);

    writecommand(CMD_PIXFMT); //Set Color Format 16bit
    writedata(0x05);
    delay(5);

    writecommand(CMD_GAMMASET); //default gamma curve 3
    writedata(0x04); //0x04
    delay(1);

    writecommand(CMD_GAMRSEL); //Enable Gamma adj
    writedata(0x01);
    delay(1);

    writecommand(CMD_NORML);

    writecommand(CMD_DFUNCTR);
    writedata(0xff); //(0b1111_1111); //
    writedata(0x6);  //(0b00000110); //

    writecommand(CMD_PGAMMAC); //Positive Gamma Correction Setting
    for (i = 0; i < 15; i++)
    {
        writedata(pGammaSet[i]);
    }

    writecommand(CMD_NGAMMAC); //Negative Gamma Correction Setting
    for (i = 0; i < 15; i++)
    {
        writedata(nGammaSet[i]);
    }

    writecommand(CMD_FRMCTR1); //Frame Rate Control (In normal mode/Full colors)
    writedata(0x08); //0x0C//0x08
    writedata(0x02); //0x14//0x08
    delay(1);

    writecommand(CMD_DINVCTR); //display inversion
    writedata(0x07);
    delay(1);

    writecommand(CMD_PWCTR1); //Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
    writedata(0x0A); //4.30 - 0x0A
    writedata(0x02); //0x05
    delay(1);

    writecommand(CMD_PWCTR2); //Set BT[2:0] for AVDD & VCL & VGH & VGL
    writedata(0x02);
    delay(1);

    writecommand(CMD_VCOMCTR1); //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
    writedata(0x50); //0x50
    writedata(99); //0x5b
    delay(1);

    writecommand(CMD_VCOMOFFS);
    writedata(0); //0x40
    delay(1);

    writecommand(CMD_CLMADRS); //Set Column Address
    writedata16(0x00);
    writedata16(_GRAMWIDTH);

    writecommand(CMD_PGEADRS); //Set Page Address
    writedata16(0X00);
    writedata16(_GRAMHEIGH);
    // set scroll area (thanks Masuda)
    writecommand(CMD_VSCLLDEF);
    writedata16(__OFFSET);
    writedata16(_GRAMHEIGH - __OFFSET);
    writedata16(0);

    colorSpace(_colorspaceData);

    setRotation(0);
    writecommand(CMD_DISPON); //display ON
    delay(1);
    writecommand(CMD_RAMWR); //Memory Write

    delay(1);
    clearScreen();
    display();
}

/*
 Colorspace selection:
 0: RGB
 1: GBR
 */
void TFT_ILI9163C::colorSpace(uint8_t cspace)
{
    if (cspace < 1)
    {
        _Mactrl_Data &= ~(1 << 3);
    }
    else
    {
        _Mactrl_Data |= 1 << 3;
    }
}

void TFT_ILI9163C::invertDisplay(bool i)
{
    writecommand(i ? CMD_DINVON : CMD_DINVOF);
}

void TFT_ILI9163C::display(bool onOff)
{
    if (onOff)
    {
        writecommand(CMD_DISPON);
    }
    else
    {
        writecommand(CMD_DISPOFF);
    }
}

void TFT_ILI9163C::idleMode(bool onOff)
{
    if (onOff)
    {
        writecommand(CMD_IDLEON);
    }
    else
    {
        writecommand(CMD_IDLEOF);
    }
}

void TFT_ILI9163C::sleepMode(bool mode)
{
    if (mode)
    {
        if (sleep == 1)
            return; //already sleeping
        sleep = 1;
        writecommand(CMD_SLPIN);
        delay(5); //needed
    }
    else
    {
        if (sleep == 0)
            return; //Already awake
        sleep = 0;
        writecommand(CMD_SLPOUT);
        delay(120); //needed
    }
}

void TFT_ILI9163C::defineScrollArea(uint16_t tfa, uint16_t bfa)
{
    tfa += __OFFSET;
    int16_t vsa = _GRAMHEIGH - tfa - bfa;
    if (vsa >= 0)
    {
        writecommand(CMD_VSCLLDEF);
        writedata16(tfa);
        writedata16(vsa);
        writedata16(bfa);
    }
}

void TFT_ILI9163C::scroll(uint16_t adrs)
{
    if (adrs <= _GRAMHEIGH)
    {
        writecommand(CMD_VSSTADRS);
        writedata16(adrs + __OFFSET);
    }
}


#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif
void TFT_ILI9163C::displayFast()
{
    setAddrWindow(0x00, 0x00, _GRAMWIDTH, _GRAMHEIGH); //go home
    _dcPin->Set();

    _ssPin->Reset();

#if 0 //defined(STM32F4) TODO occasional hiccups maybe due to FIFO overflow
    //requires inverting hi and lo color bytes
    _spi->SendDMA(_frameBuffer, sizeof(_frameBuffer));
#else

    //TODO SPI DMA for F1
    for (int px = 0; px < _GRAMSIZE; px++)
    {
        while (!(*SR & SPI_SR_TXE))
            ;

#ifdef INVERTED_565
        *DR = _frameBuffer[px];

        while (!(*SR & SPI_SR_TXE))
            ;

        *DR = _frameBuffer[px]>>8;
#else
        *DR = _frameBuffer[px]>>8;
        while (!(*SR & SPI_SR_TXE))
            ;
        *DR = _frameBuffer[px];

#endif
    }
#endif

    _ssPin->Set();

}
#ifdef __GNUC__
#pragma GCC pop_options
#endif



#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif
void TFT_ILI9163C::display()
{
    setAddrWindow(0x00, 0x00, _GRAMWIDTH, _GRAMHEIGH); //go home
    _dcPin->Set();

    _ssPin->Reset();
    volatile uint16_t tmp;
    for (int px = 0; px < _GRAMSIZE; px++)
    {
    	//_spi->TransmitByte(_frameBuffer[px]>>8);
        while (!(*SR & SPI_SR_TXE))
            ;
        *DR = _frameBuffer[px]>>8;

        while(!(*SR & SPI_SR_RXNE))
            ;

        tmp  = *DR;

        while (!(*SR & SPI_SR_TXE))
            ;
        *DR = _frameBuffer[px];

        while(!(*SR & SPI_SR_RXNE))
            ;
        tmp  = *DR;


    	//_spi->TransmitByte(_frameBuffer[px]);
    }
    _ssPin->Set();


}
#ifdef __GNUC__
#pragma GCC pop_options
#endif

void TFT_ILI9163C::clearScreen()
{
    for (uint32_t i = 0; i < _GRAMSIZE/4; ++i)
    {
        ((uint64_t*)_frameBuffer)[i] = 0;
    }
}


void TFT_ILI9163C::writeScreen24(const uint32_t *bitmap, uint16_t size)
{
    uint16_t color;
    uint16_t px;
    writecommand(CMD_RAMWR);
    for (px = 0; px < size; px++)
    {
        color = Color24To565(bitmap[px]);
        writedata16(color);
    }
    homeAddress();
}

void TFT_ILI9163C::homeAddress()
{
    setAddrWindow(0x00, 0x00, _GRAMWIDTH, _GRAMHEIGH);
}

//void TFT_ILI9163C::setCursor(int16_t x, int16_t y)
//{
//    if (boundaryCheck(x, y))
//        return;
//
//    Adafruit_GFX::setCursor(x, y);
//
//    setAddrWindow(0x00, 0x00, cursor_x, cursor_y);
//}

void TFT_ILI9163C::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if (boundaryCheck(x, y))
        return;
    if ((x < 0) || (y < 0))
        return;

    _frameBuffer[y*_GRAMWIDTH + x] = color;
}

void TFT_ILI9163C::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    // Rudimentary clipping
    if (boundaryCheck(x, y))
        return;
    if (((y + h) - 1) >= _height)
        h = _height - y;

    for (int16_t i = 0; i < h; ++i)
    {
        _frameBuffer[(y+i)*_GRAMWIDTH + x] = color;
    }
}

bool TFT_ILI9163C::boundaryCheck(int16_t x, int16_t y)
{
    if ((x >= _width) || (y >= _height))
        return true;
    return false;
}

void TFT_ILI9163C::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    // Rudimentary clipping
    if (boundaryCheck(x, y))
        return;
    if (((x + w) - 1) >= _width)
        w = _width - x;

    for (int16_t i = 0; i < w; ++i)
    {
        _frameBuffer[y * _GRAMWIDTH + x + i] = color;
    }
}

void TFT_ILI9163C::fillScreen(uint16_t color)
{
    for (uint16_t i = 0; i < _GRAMSIZE; ++i)
    {
        _frameBuffer[i] = color;
    }
    //setAddrWindow(0x00, 0x00, _GRAMWIDTH, _GRAMHEIGH); //go home
}

// fill a rectangle
void TFT_ILI9163C::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    if (boundaryCheck(x, y))
        return;
    if (((x + w) - 1) >= _width)
        w = _width - x;
    if (((y + h) - 1) >= _height)
        h = _height - y;


    for (int16_t j = 0; j < h; ++j)
    {
        for (int16_t i = 0; i < w; ++i)
        {
            _frameBuffer[(y+j) * _GRAMWIDTH + x + i] = color;
        }
    }
}


void TFT_ILI9163C::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    writecommand(CMD_CLMADRS); // Column
    if (rotation == 0 || rotation > 1)
    {
        writedata16(x0);
        writedata16(x1);
    }
    else
    {
        writedata16(x0 + __OFFSET);
        writedata16(x1 + __OFFSET);
    }

    writecommand(CMD_PGEADRS); // Page
    if (rotation == 0)
    {
        writedata16(y0 + __OFFSET);
        writedata16(y1 + __OFFSET);
    }
    else
    {
        writedata16(y0);
        writedata16(y1);
    }
    writecommand(CMD_RAMWR); //Into RAM
}

//0: default vertical, connector on top
//1: landscape, connector on left
//2: portrait, connector at bottom
//3: landscape, connector on right
void TFT_ILI9163C::setRotation(uint8_t m)
{

    rotation = m % 4; // can't be higher than 3
    switch (rotation)
    {
    case 0:
        _Mactrl_Data = 8; //0b00001000;
        _width = _TFTWIDTH;
        _height = _TFTHEIGHT; //-__OFFSET;
        break;
    case 1:
        _Mactrl_Data = 0x68; //0b0110_1000;
        _width = _TFTHEIGHT; //-__OFFSET;
        _height = _TFTWIDTH;
        break;
    case 2:
        _Mactrl_Data = 0xc8; //0b1100_1000;
        _width = _TFTWIDTH;
        _height = _TFTHEIGHT; //-__OFFSET;
        break;
    case 3:
        _Mactrl_Data = 0xa8; //0b1010_1000;
        _width = _TFTWIDTH;
        _height = _TFTHEIGHT; //-__OFFSET;
        break;
    }
    colorSpace(_colorspaceData);
    writecommand(CMD_MADCTL);
    writedata(_Mactrl_Data);
}


void TFT_ILI9163C::drawBmp2(int16_t x, int16_t y, const uint8_t *bmp)
{
    WINBMPFILEHEADER* hdr1 = (WINBMPFILEHEADER*)bmp;
    WIN3XBITMAPHEADER*  hdr2 = (WIN3XBITMAPHEADER*)&bmp[sizeof(WINBMPFILEHEADER)];

    const uint16_t* bitmap16 = (uint16_t*)&bmp[hdr1->BitmapOffset];

    uint16_t wread = hdr2->Width;
    uint16_t h = hdr2->Height;

    if (hdr2->Width / 2 * 2 != hdr2->Width)
    {
        wread = (hdr2->Width / 2 + 1) * 2;
    }


    uint16_t c = 0;
    for (uint32_t j = 0; j < h; ++j)
    {
        for (uint32_t i = 0; i < wread; ++i)
        {

        	c = bitmap16[(hdr2->Height-j-1)*wread + i];
        	if (i < hdr2->Width)
        	{
        		if (x + i >= 0 && y + j >= 0 && (x + i < _width) && (y + j  < _height))
        		//drawPixel(x + i, y + j, c);
        			_frameBuffer[(y + j)*_GRAMWIDTH + x + i] = c;
        	}
        }
    }

}

