/*********************************************************************
This is a library for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "Adafruit_GFX.h"
#include "Adafruit_Nokia5110.h"
#include "debug.h"

// the memory buffer for the LCD
uint8_t pcd8544_buffer[LCDWIDTH * LCDHEIGHT / 8] = {0};

// reduces how much is refreshed, which speeds it up!
// originally derived from Steve Evans/JCW's mod but cleaned up and
// optimized
//#define enablePartialUpdate

#ifdef enablePartialUpdate
static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;
#endif



static void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax) {
#ifdef enablePartialUpdate
  if (xmin < xUpdateMin) xUpdateMin = xmin;
  if (xmax > xUpdateMax) xUpdateMax = xmax;
  if (ymin < yUpdateMin) yUpdateMin = ymin;
  if (ymax > yUpdateMax) yUpdateMax = ymax;
#endif
}

Adafruit_Nokia5110::Adafruit_Nokia5110(SPI* spi, GPIO_PIN* dcPin, GPIO_PIN* rstPin, GPIO_PIN* ssPin)
: Adafruit_GFX(LCDWIDTH, LCDHEIGHT)
{
    _spi = spi;
    _dcPin = dcPin;
    _rstPin = rstPin;
    _ssPin = ssPin;
    setTextColor(1, 0);
}



// the most basic function, set a single pixel
void Adafruit_Nokia5110::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    return;

//  if (color == 0)
//  {
//      color = 1;
//  }
//  else
//  {
//      color = 0;
//  }

  int16_t t;
  switch(rotation){
    case 1:
      t = x;
      x = y;
      y =  LCDHEIGHT - 1 - t;
      break;
    case 2:
      x = LCDWIDTH - 1 - x;
      y = LCDHEIGHT - 1 - y;
      break;
    case 3:
      t = x;
      x = LCDWIDTH - 1 - y;
      y = t;
      break;
  }

  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] |= (1<<(y%8));
  else
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] &= ~(1<<(y%8));

  updateBoundingBox(x,y,x,y);
}


// the most basic function, get a single pixel
uint8_t Adafruit_Nokia5110::getPixel(int8_t x, int8_t y) {
  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return 0;

  return (pcd8544_buffer[x+ (y/8)*LCDWIDTH] >> (y%8)) & 0x1;  
}


void Adafruit_Nokia5110::Init(uint8_t contrast, uint8_t bias) {

	// Reset mandatory
	GPIO_ResetPin(_rstPin);
	delay(10);
	GPIO_SetPin(_rstPin);

	command(0x21); //Tell LCD that extended commands follow
    //SendCommand(0xB1); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
    command(contrast); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
    command(0x04); //Set Temp coefficent
    //SendCommand(0x15); //LCD bias mode 1:48: Try 0x13 or 0x14
    command(bias); //LCD bias mode 1:48: Try 0x13 or 0x14

    command(0x20); //We must send 0x20 before modifying the display control mode
    command(0x0C); //Set display control, normal mode. 0x0D for inverse
}


void Adafruit_Nokia5110::command(uint8_t data) {
	GPIO_ResetPin(_dcPin);
	GPIO_ResetPin(_ssPin);
    SPI_TransmitByte(_spi, data);
	GPIO_SetPin(_ssPin);
}

void Adafruit_Nokia5110::data(uint8_t data) {
	GPIO_SetPin(_dcPin);

	GPIO_ResetPin(_ssPin);
    SPI_TransmitByte(_spi, data);
	GPIO_SetPin(_ssPin);
}

void Adafruit_Nokia5110::setContrast(uint8_t val) {
  if (val > 0x7f) {
    val = 0x7f;
  }
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
  command( PCD8544_SETVOP | val); 
  command(PCD8544_FUNCTIONSET);
  
 }



void Adafruit_Nokia5110::display(void) {
  uint8_t col, maxcol, p;
  
  for(p = 0; p < 6; p++) {
#ifdef enablePartialUpdate
    // check if this page is part of update
    if ( yUpdateMin >= ((p+1)*8) ) {
      continue;   // nope, skip it!
    }
    if (yUpdateMax < p*8) {
      break;
    }
#endif

    command(PCD8544_SETYADDR | p);


#ifdef enablePartialUpdate
    col = xUpdateMin;
    maxcol = xUpdateMax;
#else
    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH-1;
#endif

    command(PCD8544_SETXADDR | col);

	GPIO_SetPin(_dcPin);
	GPIO_ResetPin(_ssPin);
    for(; col <= maxcol; col++) {
      SPI_TransmitByte(_spi, pcd8544_buffer[(LCDWIDTH*p)+col]);
    }
	GPIO_SetPin(_ssPin);

  }

  command(PCD8544_SETYADDR );  // no idea why this is necessary but it is to finish the last byte?
#ifdef enablePartialUpdate
  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT-1;
  yUpdateMax = 0;
#endif

}

// clear everything
void Adafruit_Nokia5110::clearDisplay(void) {
  memset(pcd8544_buffer, 0, LCDWIDTH*LCDHEIGHT/8);
  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
  cursor_y = cursor_x = 0;
}

/*
// this doesnt touch the buffer, just clears the display RAM - might be handy
void Adafruit_Nokia5110::clearDisplay(void) {
  
  uint8_t p, c;
  
  for(p = 0; p < 8; p++) {

    st7565_command(CMD_SET_PAGE | p);
    for(c = 0; c < 129; c++) {
      //uart_putw_dec(c);
      //uart_putchar(' ');
      st7565_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      st7565_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      st7565_data(0x0);
    }     
    }

}

*/
