
#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "debug.h"

#define TFTWIDTH   176
#define TFTHEIGHT  220


class TestLCD// : public Adafruit_GFX
{
private:
    GPIO_PIN* _cs, *_rs, *_wr, *_rd, *_rst;
    GPIO_PORT* _dataPort;

    uint32_t _odr_addr;



public:
    TestLCD(GPIO_PIN* cs, GPIO_PIN* rs, GPIO_PIN* wr, GPIO_PIN* rd, GPIO_PIN* rst, GPIO_PORT* dataPort)
    {
        _cs = cs;
        _rs = rs;
        _wr = wr;
        _rd = rd;
        _rst = rst;
        _dataPort = dataPort;

        _odr_addr = (uint32_t)_dataPort->GetGPIO_ODR();


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
        delay_us(10);
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

        setWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1);

    }


    void setWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
        writeRegister16(0x37, x1);
        writeRegister16(0x36, x2);
        writeRegister16(0x39, y1);
        writeRegister16(0x38, y2);
        writeRegister16(0x20, x1);
        writeRegister16(0x21, y1);
    }

public:

    void drawPixel(int16_t x, int16_t y, uint16_t color) {
        setWindow(x, y, x, y);
        writeRegister16(0x22, color);
    }

    uint16_t readID()
    {
        setWriteDir();

        _cs->Reset();

        _rs->Reset();  //command

        //_rd->Set();
        //_wr->Set();

        write8(0);
        write8(0);

        //_cs->Set();

        /////read data
        setReadDir();

        //_cs->Reset();
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
    TestLCD t(cs, rs, wr, rd, rst, dataPort);


    //t.reset();
    //delay(500);
    //volatile uint16_t id2 = t.readID();

    for (int x = 0; x < 100; ++x)
    {
        for (int y = 0; y < 100; ++y)
        {
            t.drawPixel(x, y, 0x0);
        }
    }

}


int main()
{
    RCC_EnableHSI_100Mhz();
    test();

}
