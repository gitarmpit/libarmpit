#include "rtc.h"
#include "rcc.h"
#include "pwr.h"
#include "9163/TFT_ILI9163C.h"
#include "debug.h"
#include "system_time.h"
#include "_func.h"
#include "debug.h"

static void init_rtc()
{
    RCC_EnablePWR(TRUE);
    PWR_DisableWriteProtection(TRUE);
    RCC_ResetBKP(TRUE);
    RCC_ResetBKP(FALSE);
    RCC_EnableRTC(FALSE);
    RCC_EnableLSE();
    RCC_SetRTCClockSourceLSE();
    RCC_EnableRTC(TRUE);
    RTC_EnableWrite(TRUE);
    RTC_SetCalendar(16, 8, 12, 11, 59, 00, FALSE);
    RTC_EnableWrite(FALSE);

    //PWR_DisableWriteProtection(FALSE);

}

void rtc_test()
{
    TFT_ILI9163C* lcd = get_lcd();

    //RTC_Set_BYPSHAD(TRUE);

    //init_rtc();
    //Debug_EnableCYCCNT(TRUE);
    //systick_enable(TRUE);

    lcd->setBgColor(0);
    lcd->setTextColor(YELLOW);
    lcd->setTextSize(1);


    uint16_t year, month, day, hour, min, sec;
    BOOL isPM;
    while (1)
    {
        lcd->fillScreen(0);
        RTC_GetCalendar(&year, &month, &day, &hour, &min, &sec, &isPM);
        lcd->printf (0, 0, "%d:%02d:%02d %s %d/%d/20%d", isPM ? hour : hour, min, sec, isPM ? "PM" : "AM", month, day, year);
        lcd->display();
        delay(1000);
    }


    while(1);

}
