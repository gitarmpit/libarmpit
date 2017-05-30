
#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "debug.h"



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






public:


    uint16_t readID()
    {
        setWriteDir();

        _cs->Reset();

        _rs->Reset();  //command

        //_rd->Set();
        //_wr->Set();

        write8(0);
        write8(0x22);

        //_cs->Set();

        /////read data
        setReadDir();

        //_cs->Reset();
        //
        _rs->Set();  //data


        uint16_t  result = read8() << 8;
        //  //delay_us(10);
        result |= read8();

        uint16_t  result2 = read8() << 8;
        //  //delay_us(10);
        result2 |= read8();
        +result2;

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
        _cs->Reset();
        _rs->Reset();
        write8(0x00);
        // Three extra 0x00s
        for (uint8_t i = 0; i < 3; i++)
        {
            _wr->Reset();
            _wr->Set();
        }

        _cs->Set();
        delay(200);

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
    t.reset();
    //delay(500);
    volatile uint16_t id2 = t.readID();

    id2++;
}


int main()
{
    RCC_EnableHSI_100Mhz();
    test();

}
