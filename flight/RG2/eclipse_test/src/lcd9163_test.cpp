#include "9163/TFT_ILI9163C.h"
#include "_pins.h"
#include "GPIO_Helper.h"
#include "system_time.h"
#include "bitmaps/abu_no_eyes_16.cpp"
#include "debug.h"

TFT_ILI9163C* get_lcd()
{
    SPI* spi = GPIO_Helper::SetupSPI(LCD_PROXIMITY_SPI, true, false, false, SPI_BAUD_RATE_2);

    DMA1* dma1 = DMA1::GetInstance();
    dma1->EnablePeripheralClock(true);
    DMA_Stream* txStream = dma1->GetStream(DMA_STREAM_5);
    txStream->SetChannel(DMA_CHANNEL_0);
    spi->SetDMA_TxStream(txStream);

    GPIO_PIN* rstPin = GPIO_Helper::GetPin(LCD_RST_PIN);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = GPIO_Helper::GetPin(LCD_DC_PIN);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = GPIO_Helper::GetPin(LCD_SS_PIN);
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

    return new TFT_ILI9163C(spi, dcPin, rstPin, ssPin);

}


extern "C" void hsv_to_rgb(int h, float s, float v, int* r, int* g, int* b);
void lcd_hue()
{
    TFT_ILI9163C* lcd = get_lcd();
    int r, g, b;
    for (int i = 0; i < 100; ++i)
    {
        hsv_to_rgb (i, 1., 1., &r, &g, &b);
        lcd->fillScreen(COLOR565(r,g,b));
        lcd->display();
        delay (50);
    }

    while(1)
        ;
}

void lcd_speed_test()
{
    TFT_ILI9163C* lcd = get_lcd();

    lcd->setBgColor(0);
    lcd->setTextColor(CYAN);
    lcd->setTextSize(2);
    lcd->clearScreen();
    lcd->setTextColor(YELLOW);
    systick_enable(TRUE);
    uint32_t t0 = millis();
    for (int32_t i = 0; i < 1000; ++i)
    {
        lcd->clearScreen();
        //lcd->drawBmp2(0, 0, menu5_16);
        lcd->display();
    }
    volatile uint32_t dur = millis() - t0;
    --dur;
    ++dur;

    while(1);

}

void lcd_test()
{
    systick_enable(true);
    TFT_ILI9163C* lcd = get_lcd();
    //lcd->fillScreen(RED);

    uint32_t t0 = millis();

    for (int i = 0; i < 1000; ++i)
    {
        //lcd->fillScreen(COLOR565(16,31,28));
        lcd->drawBmp2 (0, 0, abu_no_eyes_16);
        lcd->displayFast();
    }

    volatile uint32_t d = millis() - t0;
    ++d;
    --d;


    //lcd->setBgColor(0);
    lcd->write ("Test", 0, 0, 2);
    lcd->invertBgFg();
    lcd->write ("Test", 0, 1, 2);
    lcd->invertBgFg();
    lcd->write ("Test", 0, 2, 2);

//    lcd->drawEllipse(lcd->width()/2, lcd->height()/2, 50, 48);
    lcd->drawEllipse(lcd->width()/2, lcd->height()/2, 13, 12, CYAN);

    for (int i = 0; i < 128; i += 4)
    {
        if ((i % 16) != 0)
        {
            lcd->drawFastHLine(i - 1, 64, 4, YELLOW);
        }
    }
    for (int i = 0; i < 128; i += 4)
    {
        if ((i % 16) != 0)
        {
            lcd->drawFastVLine(64, i - 1, 4, YELLOW);
        }
    }

    lcd->fillCircle (64, 64, 2, YELLOW);
    lcd->display();



//    lcd->setTextColor(COLOR656(0, 63, 31));
//    for (int i = 0; i < 32; ++i)
//    {
//        lcd->fillScreen(COLOR656(i, 0, 31-i));
//        delay(100);
//        lcd->display();
//    }


    while(1);
}

