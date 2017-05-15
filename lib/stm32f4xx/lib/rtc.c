///*
// * The MIT License (MIT)
// *
// * Copyright (c) 2015 igorS
// *
// * Permission is hereby granted, free of charge, to any person obtaining a copy
// * of this software and associated documentation files (the "Software"), to deal
// * in the Software without restriction, including without limitation the rights
// * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// * copies of the Software, and to permit persons to whom the Software is
// * furnished to do so, subject to the following conditions:
// *
// * The above copyright notice and this permission notice shall be included in all
// * copies or substantial portions of the Software.
// *
// * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// * SOFTWARE.
// */
//
//

#include "rtc.h"
#include "pwr.h"

void RTC_EnableWrite(BOOL enable)
{
	PWR_EnablePeripheralClock(TRUE);
	if (enable)
	{
		PWR_DisableWriteProtection(TRUE);
		RTC_WPR = 0xCA;
		RTC_WPR = 0x53;
	}
	else
	{
		PWR_DisableWriteProtection(FALSE);
	}
}

void RTC_GetTime(uint16_t* hour, uint16_t* min, uint16_t* sec, BOOL* isPM)
{
    uint32_t rtc_tr = RTC_TR;

	*hour = ((rtc_tr & RTC_TR_HT_CLEARMASK)>>RTC_TR_HT)*10 + ((rtc_tr & RTC_TR_HU_CLEARMASK)>>RTC_TR_HU);
	*min  = ((rtc_tr & RTC_TR_MNT_CLEARMASK)>>RTC_TR_MNT)*10 + ((rtc_tr & RTC_TR_MNU_CLEARMASK)>>RTC_TR_MNU);
	*sec  = ((rtc_tr & RTC_TR_ST_CLEARMASK)>>RTC_TR_ST)*10 + ((rtc_tr & RTC_TR_SU_CLEARMASK)>>RTC_TR_SU);
	*isPM = rtc_tr & (1<<RTC_TR_PM);
}

void RTC_GetDate(uint16_t* year, uint16_t* month, uint16_t* day)
{
    uint32_t rtc_dr = RTC_DR;

	*year = ((rtc_dr & RTC_DR_YT_CLEARMASK)>>RTC_DR_YT)*10 + ((rtc_dr & RTC_DR_YU_CLEARMASK)>>RTC_DR_YU);
	*month  = ((rtc_dr & RTC_DR_MT_CLEARMASK)>>RTC_DR_MT)*10 + ((rtc_dr & RTC_DR_MU_CLEARMASK)>>RTC_DR_MU);
	*day  = ((rtc_dr & RTC_DR_DT_CLEARMASK)>>RTC_DR_DT)*10 + ((rtc_dr & RTC_DR_DU_CLEARMASK)>>RTC_DR_DU);

}

void RTC_GetCalendar(uint16_t* year, uint16_t* month, uint16_t* day, uint16_t* hour, uint16_t* min, uint16_t* sec, BOOL* isPM)
{
	RTC_GetDate (year, month, day);
	RTC_GetTime (hour, min, sec, isPM);
}


void RTC_Set_BYPSHAD(BOOL set)
{
	if (set)
	{
		RTC_CR |= RTC_CR_BYPSHAD;
	}
	else
	{
		RTC_CR &= ~RTC_CR_BYPSHAD;
	}
}

void RTC_SetCalendar (uint16_t year, uint16_t month, uint16_t day, uint16_t hour, uint16_t min, uint16_t sec, BOOL isPM)
{
	RTC_EnableWrite(TRUE);
	RTC_EnterInit();

    uint32_t rtc_dr = RTC_DR;

    rtc_dr &= ~RTC_DR_DU_CLEARMASK;
    rtc_dr |= (day % 10)<<RTC_DR_DU;
    rtc_dr &= ~RTC_DR_DT_CLEARMASK;
    rtc_dr |= (day / 10)<<RTC_DR_DT;

    rtc_dr &= ~RTC_DR_MU_CLEARMASK;
    rtc_dr |= (month % 10)<<RTC_DR_MU;
    rtc_dr &= ~RTC_DR_MT_CLEARMASK;
    rtc_dr |= (month / 10)<<RTC_DR_DT;

    rtc_dr &= ~RTC_DR_YU_CLEARMASK;
    rtc_dr |= (year % 10)<<RTC_DR_YU;
    rtc_dr &= ~RTC_DR_YT_CLEARMASK;
    rtc_dr |= (year / 10)<<RTC_DR_YT;

    RTC_DR = rtc_dr;


    uint32_t rtc_tr = 0;
    rtc_tr |= isPM<<RTC_TR_PM;

    rtc_tr |= (sec % 10)<<RTC_TR_SU;
    rtc_tr |= (sec / 10)<<RTC_TR_ST;
    rtc_tr |= (min % 10)<<RTC_TR_MNU;
    rtc_tr |= (min / 10)<<RTC_TR_MNT;
    rtc_tr |= (hour % 10)<<RTC_TR_HU;
    rtc_tr |= (hour / 10)<<RTC_TR_HT;


    RTC_TR = rtc_tr;

	RTC_LeaveInit();
	RTC_EnableWrite(FALSE);
}

void RTC_EnterInit()
{
	 RTC_ISR |=  RTC_ISR_INIT;
	 while (!(RTC_ISR & RTC_ISR_INITF))
		 ;
}

void RTC_LeaveInit()
{
	RTC_ISR &=  ~RTC_ISR_INIT;
}

