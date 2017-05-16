#include "RF22/si4432.h"

#include <stdio.h>

#include "adc_reader.h"
#include "button_handler.h"
#include "timer_cpp.h"
#include "afio.h"
#include "flash.h"
//#include "lcd5110.h"
#include "GPIO_Helper.h"
#include "5110/Adafruit_Nokia5110.h"
#include "main_loop.h"
#include "spi_lcd_setup.h"
#include "timer_ch_setup.h"
#include "debug.h"


static void start()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    GPIOC::GetInstance()->EnablePeripheralClock(true);


//    GPIO_PIN* g = GPIOB::GetInstance()->GetPin(GPIO_PIN9);
//    g->SetupGPIO_OutPP();
//
//    while(1)
//    {
//        g->Set();
//        delay(200);
//        g->Reset();
//        delay(200);
//
//    }


    //ADC: Throttle Yaw Pitch Roll, Keypad YAXB, L/R Trigger: 10 total A0 - B1

    RCC_EnableAFIO(true);
    AFIO_Remap_SWJ_CFG(false, true, false);
    AFIO_Remap_SPI1(true);

    SPI_LCD_Setup lcdSetup;

    Adafruit_Nokia5110 display(lcdSetup.GetSPI(), lcdSetup.GetDcPin(), lcdSetup.GetRstPin(), lcdSetup.GetSsPin());

    //bool reverse = true;
    //LCD5110 display(spiLCD, dcPin, rstPin, ssPin, reverse);
    //display.Init();
    //_display->setTextColor(1, 1);
    //display.Clear();
    //display.WriteLine(0, 0, "Test");

    Timer_Ch_Setup  t_ch;
    t_ch.Get_BL()->UpdatePWMDuty(100);

   display.write("test");
   display.display();
//    display.drawString("test", 0, 0, 4);
//    display.display();

    //while(1)
    //    ;


    //Radio 22 25 26 27 28
    //25 26 27 28 : SPI2 radio
    SPI* spiRadio = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_32);
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    GPIO_PIN* ssPinRadio = GPIOB::GetInstance()->GetPin(GPIO_PIN12);
    GPIO_PIN* sdnPin = GPIOB::GetInstance()->GetPin(GPIO_PIN11);
    Si4432 radio(spiRadio, ssPinRadio, sdnPin, 0);
    radio.init();
    radio.setBaudRate(70);
    radio.setFrequency(433);
    radio.readAll();
//
    //20 Photo
    //21 Infra LED
    MainLoop mainLoop (&radio, &display, t_ch.Get_BL(), t_ch.Get_Sound(), t_ch.Get_Red(), t_ch.Get_Green());
    mainLoop.Run();


}

extern "C" void initialise_monitor_handles(void);

int main()
{
#ifdef SEMIHOSTING
    initialise_monitor_handles();
#endif
    //RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();

    FLASH_SetWaitState(2);

    //RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
    RCC_EnableHSI(TRUE);
    RCC_EnablePLL(16);
    RCC_SetAPB1PrescalerDiv2();
    //RCC_SetAHBPrescalerDiv4();

    GPIOB::GetInstance()->EnablePeripheralClock(true);

    Debug_EnableCYCCNT(true);


    start();
}
