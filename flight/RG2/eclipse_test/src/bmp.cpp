
//#include "bmp.h"
#include <stdint.h>
#include "9163/TFT_ILI9163C.h"
#include "bitmaps/menu5_16.cpp"
#include "_func.h"
#include "debug.h"

void bmp_test()
{
    TFT_ILI9163C* lcd = get_lcd();

    lcd->setTextSize(2);
    lcd->setBgColor(WHITE);
    lcd->setTextColor(BLACK);
    lcd->fillScreen(YELLOW);

    //lcd->write("8", 0, 0);

    lcd->drawBmp(0, 0, menu5_16);
    lcd->drawRect(2, 2, 35, 36, BLUE);
    lcd->drawRect(2, 46, 35, 36, BLUE);
    lcd->drawRect(2, 87, 35, 36, BLUE);

    lcd->drawRect(43, 2, 36, 36, BLUE);
    lcd->drawRect(43, 46, 36, 36, BLUE);

    lcd->drawRect(85, 2, 36, 36, BLUE);
    lcd->drawRect(85, 46, 36, 36, BLUE);
    lcd->drawRect(44, 87, 128-44-2, 128-87-2, BLUE);
    lcd->display();

//  lcd->drawBmp16(0, 0, java);
//    lcd->display();
//    delay(2000);
//    lcd->fillScreen(WHITE);
//    lcd->drawBmp16(30, 0, wave);
//  lcd->display();
//    delay(2000);
//    lcd->fillScreen(WHITE);
//    lcd->drawBmp16(0, 0, jboss);
//    lcd->write("JBoss Fuse loading", 0, 6);
//    //lcd->write("       is loading...", 0, 6);
//    lcd->display();

/*
    WINBMPFILEHEADER* hdr1 = (WINBMPFILEHEADER*)bmp;
    WIN3XBITMAPHEADER*  hdr2 = (WIN3XBITMAPHEADER*)&bmp[sizeof(WINBMPFILEHEADER)];

    const uint8_t* bitmap = &bmp[hdr1->BitmapOffset];
    uint8_t wread = hdr2->Width/2;

    if (hdr2->Width / 4 * 4 != hdr2->Width)
    {
        int w = (hdr2->Width / 4 + 1) * 4;
        wread  = w /2;
    }

    uint8_t b;
    for (uint32_t j = 0; j < hdr2->Height; ++j)
    {
        lcd->setCursor(0, j);
        uint32_t wtotal = 0;
        for (int i = 0; i < wread; ++i)
        {
            b = bitmap[(hdr2->Height-j-1)*wread + i];
            if (++wtotal <= hdr2->Width)
            {
                lcd->printf ("%x",  b>>4);
            }
            if (++wtotal <= hdr2->Width)
            {
                lcd->printf ("%x",  b&0xf);
            }
            //lcd->printf ("%x%x",  b>>4, (b&0xf));
        }
    }
    lcd->display();
*/
    while(1)
        ;

}



