#include <gpio.h>
#include <gpio_cpp.h>
#include <rcc.h>
#include <RCC_Helper.h>
#include <stdint-gcc.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "ILI932x.h"



ILI932x::ILI932x(GPIO_PIN* cs, GPIO_PIN* rs, GPIO_PIN* wr, GPIO_PIN* rd,
		GPIO_PIN* rst, GPIO_PORT* dataPort) :
		Adafruit_GFX(TFTWIDTH, TFTHEIGHT) {

	// For STM32F1 all 5 pins must be on the same port
	_cs = cs;
	_rs = rs;
	_wr = wr;
	_rd = rd;
	_rst = rst;

	_dataPort = dataPort;

	_odr_addr = _dataPort->GetGPIO_ODR();
	_bsrr_addr = _wr->GetPort()->GetGPIO_BSRR();
#ifdef STM32F1
	_brr_addr = _cs->GetPort()->GetGPIO_BRR();
#endif
	_csMask = 1 << _cs->GetPinNumber();
	_rsMask = 1 << _rs->GetPinNumber();
	_wrMask = 1 << _wr->GetPinNumber();
	_wrstMask = (1 << _wr->GetPinNumber()) << 16;

	_cs->Set(); // Set all control bits to idle state
	_wr->Set();
	_rd->Set();
	_rs->Set();
	_rst->Set();

	setWriteDir(); // Set up LCD data port(s) for WRITE operations
}

void ILI932x::init(void) {
	uint16_t a, d;

    reset();

	uint8_t i = 0;
	while (i < sizeof(ILI932x_regValues) / sizeof(uint16_t)) {
		a = ILI932x_regValues[i++];
		d = ILI932x_regValues[i++];
		if (a == TFTLCD_DELAY) {
			delay(d);
		} else
			writeRegister16(a, d);
	}

	setRotation(rotation);
}

uint8_t ILI932x::read8() {
	_rd->Reset();
	delay_us(10);
	uint8_t rc = _dataPort->GetInput() & 0xff;
	_rd->Set();
	return rc;
}

void ILI932x::writeRegister16(uint16_t addr, uint16_t data) {
//#ifdef STM32F1
#if 0
	__asm volatile(

			// cs Low
			" str  %[cs],    [%[brr]]    \n\t"

			//rs Low : address mode
			" str  %[rs],    [%[brr]]    \n\t"

			// Store 0 (low addr)
			" str  %[a_hi], [%[odr]]          \n\t"
			// Trigger ws low/high
			" str  %[wr],    [%[brr]]    \n\t"
			//  "nop\n\t; nop\n\t; nop\n\t"
			" str  %[wr],    [%[bsrr]]      \n\t"

			// Store addr
			" str  %[a_lo], [%[odr]]          \n\t"
			// Trigger ws low/high
			" str  %[wr],    [%[brr]]    \n\t"
			//  "nop\n\t; nop\n\t; nop\n\t"
			" str  %[wr],    [%[bsrr]]      \n\t"

			//rs High : data mode
			" str  %[rs],    [%[bsrr]]    \n\t"

			//Store high data
			" str  %[d_hi], [%[odr]]          \n\t"
			// Trigger ws low/high
			" str  %[wr],    [%[brr]]    \n\t"
			//  "nop\n\t; nop\n\t; nop\n\t"
			" str  %[wr],    [%[bsrr]]      \n\t"

			//Store low data
			" str  %[d_lo], [%[odr]]          \n\t"
			// Trigger ws low/high
			" str  %[wr],    [%[brr]]    \n\t"
			//  "nop\n\t; nop\n\t; nop\n\t"
			" str  %[wr],    [%[bsrr]]      \n\t"

			// cs High
			" str  %[cs],    [%[bsrr]]    \n\t"

			:: [brr] "r" (_brr_addr),
			[bsrr] "r" (_bsrr_addr),
			[odr] "r" (_odr_addr),
			[cs] "r" (_csMask),
			[wr] "r" (_wrMask),
			[rs] "r" (_rsMask),
			[a_hi] "r" (0),
			[a_lo] "r" (addr),
			[d_hi] "r" (data>>8),
			[d_lo] "r" (data)
	);
#else
	_cs->Reset();

	// Set  Addr
	_rs->Reset();

	_dataPort->SetOutput(0);
	_wr->Reset();
	_wr->Set();

	_dataPort->SetOutput(addr);
	_wr->Reset();
	_wr->Set();

	// Set Data
	_rs->Set();

	_dataPort->SetOutput(data >> 8);
	_wr->Reset();
	_wr->Set();

	_dataPort->SetOutput(data);
	_wr->Reset();
	_wr->Set();

	_cs->Set();

#endif
}

// Fast block fill operation for fillScreen, fillRect, H/V line, etc.
// Requires setAddrWindow() has previously been called to set the fill
// bounds.  'len' is inclusive, MUST be >= 1.
void ILI932x::flood(uint16_t color, uint32_t len) {
	uint16_t blocks;
	uint8_t i, hi = color >> 8, lo = color;

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
	if (hi == lo) {
		// High and low bytes are identical.  Leave prior data
		// on the port(s) and just toggle the write strobe.
		while (blocks--) {
			i = 16; // 64 pixels/block / 4 pixels/pass
			do {
				for (uint8_t s = 0; s < 8; ++s) {
					*_brr_addr = 1 << _wrMask;
					*_bsrr_addr = 1 << _wrMask;
					// new:
					//*_bsrr_addr = _wrstMask;
					//*_bsrr_addr = _wrMask;

					/*
					__asm volatile(
							"str  %[wrst],   [%[bsrr]]   \n\t"
							__DELAY__
							"str  %[wr], [%[bsrr]]   \n\t"
							"nop \n\t"
							"nop \n\t"
							::
							[bsrr] "r" (_bsrr_addr),
							[wr]   "r" (_wrMask),
							[wrst] "r" (_wrstMask)
					);
*/
				}
			} while (--i);
		}
		// Fill any remaining pixels (1 to 64)
		for (i = (uint8_t) len & 63; i--;) {
			/*
			*_brr_addr = 1 << _wrMask;
			*_bsrr_addr = 1 << _wrMask;
			*_brr_addr = 1 << _wrMask;
			*_bsrr_addr = 1 << _wrMask;
			*/
			//new:
/*
			*_bsrr_addr = _wrstMask;
			*_bsrr_addr = _wrMask;
			*_bsrr_addr = _wrstMask;
			*_bsrr_addr = _wrMask;
*/
			for (uint8_t i = 0; i < 2; ++i)
			{
				__asm volatile(
						"str  %[wrst],   [%[bsrr]]   \n\t"
						__DELAY__
						"str  %[wr], [%[bsrr]]   \n\t"
						::
						[bsrr] "r" (_bsrr_addr),
						[wr]   "r" (_wrMask),
						[wrst] "r" (_wrstMask)
				);

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
	_cs->Set();
}

uint16_t ILI932x::readID() {
	setWriteDir();
	_cs->Reset();

	_rs->Reset();  //command

	write8(0);
	write8(0);

	_cs->Set();

	/////read data
	setReadDir();

	_cs->Reset();
	_rs->Set();  //data

	uint16_t result = read8() << 8;
	result |= read8();

	_cs->Set();
	setWriteDir();

	return result;

}

void ILI932x::reset() {

	_cs->Set();
	_wr->Set();
	_rd->Set();

	_rst->Reset();
	delay(2);
	_rst->Set();

	setWriteDir();

	// Data transfer sync
	_cs->Reset();
	_rs->Reset();
	write8(0x00);
	// Three extra 0x00s
	for (uint8_t i = 0; i < 3; i++) {
		_wr->Reset();
		_wr->Set();
	}

	_cs->Set();
	delay(200);
}

void ILI932x::setRotation(uint8_t x) {
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
	writeRegister16(0x0003, t); // MADCTL
	// For 932X, init default full-screen address window:
	setAddrWindow(0, 0, _width - 1, _height - 1);
}

void ILI932x::fillScreen(uint16_t color) {
	// For the 932X, a full-screen address window is already the default
	// state, just need to set the address pointer to the top-left corner.
	// Although we could fill in any direction, the code uses the current
	// screen rotation because some users find it disconcerting when a
	// fill does not occur top-to-bottom.
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
	writeRegister16(0x0020, x);
	writeRegister16(0x0021, y);
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

	writeRegister16(0x0020, x);
	writeRegister16(0x0021, y);
	writeRegister16(0x0022, color);
}

void ILI932x::fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h,
		uint16_t fillcolor) {
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

//TODO: private
// Issues 'raw' an array of 16-bit color values to the LCD; used
// externally by BMP examples.  Assumes that setWindowAddr() has
// previously been set to define the bounds.  Max 255 pixels at
// a time (BMP examples read in small chunks due to limited RAM).
void ILI932x::pushColors(uint16_t *data, int len) {
	uint16_t color;
	uint8_t hi, lo;

	_cs->Reset();

	while (len--) {
		color = *data++;
		hi = color >> 8;
		lo = color;
		write8(hi);
		write8(lo);
	}

	_cs->Set();
}

#ifdef STM32F1
void ILI932x::pushColors2(uint16_t *data, int len) {
	static int _len;
	static uint16_t *_data;

	_len = len;
	_data = data;

	__asm volatile(

			// cs Low
			" str  %[cs],    [%[brr]]    \n\t"

			"loop_%=:                       \n\t"
			"ldrb r3, [%[data]], #1      \n\t"//low
			"ldrb r4, [%[data]], #1      \n\t"//high

			//Store high data
			" str  r4, [%[odr]]          \n\t"
			// Trigger ws low/high
			" str  %[wr],    [%[brr]]    \n\t"
			//  "nop\n\t; nop\n\t; nop\n\t"
			" str  %[wr],    [%[bsrr]]      \n\t"

			//Store low data
			" str  r3, [%[odr]]          \n\t"
			// Trigger ws low/high
			" str  %[wr],    [%[brr]]    \n\t"
			//  "nop\n\t; nop\n\t; nop\n\t"
			" str  %[wr],    [%[bsrr]]      \n\t"

			"subs %[len], #1             \n\t"
			"bne loop_%=                  \n\t"

			// cs High
			" str  %[cs],    [%[bsrr]]    \n\t"

			:
			[len] "+r" (_len),
			[data] "+r" (_data)
			:
			[brr] "r" (_brr_addr),
			[bsrr] "r" (_bsrr_addr),
			[odr] "r" (_odr_addr),
			[cs] "r" (_csMask),
			[wr] "r" (_wrMask)
			:"r3", "r4", "cc"
	);

}
#endif

void ILI932x::startPushColors() {
	_cs->Reset();
	_rs->Reset();
	write8(0x00);
	write8(0x22);
	_rs->Set();
	_cs->Set();

}
//TODO: private
// Sets the LCD address window (and address counter, on 932X).
// Relevant to rect/screen fills and H/V lines.  Input coordinates are
// assumed pre-sorted (e.g. x2 >= x1).
void ILI932x::setAddrWindow(int x1, int y1, int x2, int y2) {
	// Values passed are in current (possibly rotated) coordinate
	// system.  932X requires hardware-native coords regardless of
	// MADCTL, so rotate inputs as needed.  The address counter is
	// set to the top-left corner -- although fill operations can be
	// done in any direction, the current screen rotation is applied
	// because some users find it disconcerting when a fill does not
	// occur top-to-bottom.
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
	writeRegister16(0x0050, x1); // Set address window
	writeRegister16(0x0051, x2);
	writeRegister16(0x0052, y1);
	writeRegister16(0x0053, y2);
	writeRegister16(0x0020, x); // Set address counter to top left
	writeRegister16(0x0021, y);

}
