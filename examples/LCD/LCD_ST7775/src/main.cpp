
#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "debug.h"
#include "Adafruit_GFX.h"

#define TFTWIDTH   176
#define TFTHEIGHT  220


//8-bit parallel interface
class ST7775 //: public Adafruit_GFX
{
private:
    GPIO_PIN* _cs, *_rs, *_wr, *_rd, *_rst;
    GPIO_PORT* _dataPort;
    volatile uint32_t* _odr_addr;
    volatile uint32_t* _bsrr_addr; //set

    uint16_t _csMask, _rsMask, _wrMask, _wrstMask, _rdMask;

    //TODO this is from GFX, remove when done
    int rotation;
    int _width = TFTWIDTH;
    int _height = TFTHEIGHT;

public:
    ST7775(GPIO_PIN* cs, GPIO_PIN* rs, GPIO_PIN* wr, GPIO_PIN* rd, GPIO_PIN* rst, GPIO_PORT* dataPort)
	//: Adafruit_GFX (TFTWIDTH, TFTHEIGHT)
    {
        _cs = cs;
        _rs = rs;
        _wr = wr;
        _rd = rd;
        _rst = rst;
        _dataPort = dataPort;

        _csMask = 1<<_cs->GetPinNumber();
        _rsMask = 1<<_rs->GetPinNumber();
        _wrMask = 1<<_wr->GetPinNumber();
        _wrstMask = 1<<(_wr->GetPinNumber() + 16);
        _rdMask = 1<<_rd->GetPinNumber();


        //GFX, remove when done
        rotation = 0;

        _odr_addr = _dataPort->GetGPIO_ODR();
        _bsrr_addr = _wr->GetPort()->GetGPIO_BSRR();


        _cs->Set(); // Set all control bits to idle state
        _wr->Set();
        _rd->Set();
        _rs->Set();
        if (_rst)
        {
            _rst->Set();
        }

        *_dataPort->GetGPIO_OSPEEDR() |= 0xffff;

        setWriteDir(); // Set up LCD data port(s) for WRITE operations

        _rst->Reset();
        delay(2);
        _rst->Set();

        init();
    }


private:

    void setWriteDir()
    {
       *_dataPort->GetGPIO_MODER() |= 0x5555;
    }

    void setReadDir()
    {
        *_dataPort->GetGPIO_MODER() &= ~0xffff;
    }

    uint8_t read8()
    {
        _rd->Reset();
        uint8_t rc = _dataPort->GetInput();
        _rd->Set();
        return rc;
    }


    inline void write8(uint8_t d)
    {
        _dataPort->SetOutput(d);
        _wr->Reset();
        //may need delay
        _wr->Set();

    	return;

        __asm volatile(

                " str  %[data], [%[odr]]          \n\t"
                " str  %[wrst],    [%[bsrr]]    \n\t"
              //  "nop\n\t; nop\n\t; nop\n\t"
                " str  %[wr],    [%[bsrr]]      \n\t"

                :: [bsrr]  "r" ((uint32_t)_bsrr_addr),
                [odr]     "r" ((uint32_t)_odr_addr),
                [wr]       "r" (_wrMask),
                [wrst]       "r" (_wrstMask),
                [data]    "r" (d)
           );


    }


    void writeRegister16(uint16_t addr, uint16_t data)
    {
        _cs->Reset();

        //cmd
        _rs->Reset();
        write8(addr>>8);
        write8(addr & 0xff);

        //data
        _rs->Set();
        write8(data>>8);
        write8(data & 0xff);

        _cs->Set();
    }

    uint16_t readRegister16(uint16_t addr)
    {
        uint16_t data;
        _cs->Reset();

        //cmd
        _rs->Reset();
        write8(addr>>8);
        write8(addr & 0xff);

        setReadDir();
        //data
        _rs->Set();

        data = read8() << 8;
        data |= read8();

        _cs->Set();

        setWriteDir();

        return data;
    }



    void init()
    {
        writeRegister16(0x01, 0x011C);
        writeRegister16(0x02, 0x0100);
        writeRegister16(0x03, 0x1030);
        writeRegister16(0x08, 0x0808);
        writeRegister16(0x0C, 0x0000);
        writeRegister16(0x0F, 0x0E01);

        writeRegister16(0x10, 0x0A00);
        writeRegister16(0x11, 0x1038);
        writeRegister16(0xFF, 0x0003);
        writeRegister16(0xB0, 0x1411);
        writeRegister16(0xB1, 0x0202);
        writeRegister16(0xB2, 0x0313);

        writeRegister16(0x30, 0x0000);
        writeRegister16(0x31, 0x00db);
        writeRegister16(0x32, 0x0000);
        writeRegister16(0x33, 0x0000);
        writeRegister16(0x34, 0x00db);
        writeRegister16(0x35, 0x0000);
        writeRegister16(0x36, 0x00AF);
        writeRegister16(0x37, 0x0000);
        writeRegister16(0x38, 0x00DB);
        writeRegister16(0x39, 0x0000);

        writeRegister16(0xFF, 0x0003);
        writeRegister16(0x50, 0x0000);
        writeRegister16(0x51, 0x0300);
        writeRegister16(0x52, 0x0103);
        writeRegister16(0x53, 0x2011);
        writeRegister16(0x54, 0x0703);
        writeRegister16(0x55, 0x0000);
        writeRegister16(0x56, 0x0400);
        writeRegister16(0x57, 0x0107);
        writeRegister16(0x58, 0x2011);
        writeRegister16(0x59, 0x0703);
        writeRegister16(0x20, 0x0000);
        writeRegister16(0x21, 0x0000);
        writeRegister16(0x07, 0x1017);

        setAddrWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1);

    }


    void setAddrWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
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

    	writeRegister16(0x37, x1);
        writeRegister16(0x36, x2);
        writeRegister16(0x39, y1);
        writeRegister16(0x38, y2);
        writeRegister16(0x20, x1);
        writeRegister16(0x21, y1);
    }

public:

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) {
        setAddrWindow(x, y, x, y);
        writeRegister16(0x22, color);
    }

    uint16_t readID()
    {
        setWriteDir();

        _cs->Reset();

        _rs->Reset();  //command

        write8(0);
        //write8(0);
        write8(0xbf);

        setReadDir();

        _rs->Set();  //data

//        uint16_t  result = read8() << 8;
//        result |= read8();

        uint8_t b1 = read8();
        uint8_t b2 = read8();
        uint8_t b3 = read8();
        uint8_t b4 = read8();
        uint8_t b5 = read8();
        uint8_t b6 = read8();

        _cs->Set();
        setWriteDir();

        return 0;

    }

    uint16_t readID2()
    {
        return readRegister16(0x0);
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
//        _cs->Reset();
//        _rs->Reset();
//        write8(0x00);
//        // Three extra 0x00s
//        for (uint8_t i = 0; i < 3; i++)
//        {
//            _wr->Reset();
//            _wr->Set();
//        }
//
//        _cs->Set();
//        delay(200);

    }

    void flood(uint16_t color, uint32_t len)
     {
         uint16_t blocks;
         uint8_t i, hi = color >> 8, lo = color;

         /*
         if (bsrr == 0)
         {
             // brr = _wr->GetPort()->GetGPIO_BRR(); STM32F1 only
        	 //brr = _wr->GetPort()->GetGPIO_BSRR();  //bits 16-31
        	 bsrr = _wr->GetPort()->GetGPIO_BSRR();
             w_pin = _wr->GetPinNumber();
         }
*/
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
                         *_bsrr_addr = 1 << _wrstMask;

                         //_wr->Set();
                         *_bsrr_addr = 1 << _wrMask;
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

                 *_bsrr_addr = 1 << _wrstMask;  //reset
                 *_bsrr_addr = 1 << _wrMask;         //set
                 *_bsrr_addr = 1 << _wrstMask;
                 *_bsrr_addr = 1 << _wrMask;


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


};

/////////////////////////////////////////

void test()
{
    GPIO_PORT* dataPort = GPIOC::GetInstance();
    dataPort->EnablePeripheralClock(true);

    GPIO_PIN* rst = GPIO_Helper::GetPin("E2");
    rst->SetupGPIO_OutPP();
    rst->SetSpeedHigh();

    GPIO_PIN* rd = GPIO_Helper::GetPin("E3");
    rd->SetupGPIO_OutPP();
    rd->SetSpeedHigh();

    GPIO_PIN* wr = GPIO_Helper::GetPin("E4");
    wr->SetupGPIO_OutPP();
    wr->SetSpeedHigh();

    GPIO_PIN* rs = GPIO_Helper::GetPin("E5");
    rs->SetupGPIO_OutPP();
    rs->SetSpeedHigh();

    GPIO_PIN* cs = GPIO_Helper::GetPin("E6");
    cs->SetupGPIO_OutPP();
    cs->SetSpeedHigh();


    rst->Set();

    delay(500);
    ST7775 t(cs, rs, wr, rd, rst, dataPort);


    //t.reset();
    //delay(500);
    volatile uint16_t id2 = t.readID();

    id2 = t.readID2();

    /*
    for (int x = 0; x < 100; ++x)
    {
        for (int y = 0; y < 100; ++y)
        {
            t.drawPixel(x, y, 0x0);
        }
    }
*/
    t.fillRect(0,0,TFTWIDTH,TFTHEIGHT,0xF80);
    while(1)
        ;

}


int main()
{
    RCC_EnableHSI_100Mhz();
    test();

}
