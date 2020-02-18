#include <gpio.h>
#include <gpio_cpp.h>
#include <rcc.h>
#include <RCC_Helper.h>
#include <stdint-gcc.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "932x/ILI932x.h"
#include "systick.h"
#include "consola24.h"  //xmax=24 ymax=8
//#include "cour20.h"
#include "lucon22.h"   //24 x 10
#include "andalemo24.h"
//#include "fixedsys20.h"
#include "fixedsys1616.h"
#include "8514fix24.h"
#include "coure12.h"
#include "DSEG7ModernMini_BoldItalic36.h"
#include "digital_7_monoitalic72.h"

volatile int tick = 0;
static void systick_handler(void) {
    ++tick;
}

static void systick_cfg() {
    uint32_t ms = 1;
    static volatile uint32_t clock = RCC_GetAHB_CLK();
    static volatile uint32_t count = (uint64_t) clock * (uint64_t) ms / 1000llu;
    SystickEnable(count / 8, true, true, systick_handler);
}

static GPIOA* portA;
static GPIOB* portB;
static GPIO_PIN *cs, *rs, *rd, *wr, *rst;

static void initGPIO()
{
	portA = GPIOA::GetInstance();
	portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    cs = portB->GetPin(GPIO_PIN0);
    cs->SetupGPIO_OutPP();
    cs->SetSpeedHigh();

    rs = portB->GetPin(GPIO_PIN1);
    rs->SetupGPIO_OutPP();
    rs->SetSpeedHigh();

    rd = portB->GetPin(GPIO_PIN12);
    rd->SetupGPIO_OutPP();
    rd->SetSpeedHigh();

    wr = portB->GetPin(GPIO_PIN10);
    wr->SetupGPIO_OutPP();
    wr->SetSpeedHigh();

    rst = portB->GetPin(GPIO_PIN11);
    rst->SetupGPIO_OutPP();
    rst->SetSpeedHigh();

    rst->Set();

    // delay(10);

}

/////////////////////////////////////////
static void test_flood() {

	initGPIO();
    ILI932x lcd(cs, rs, wr, rd, rst, portA, portB);
    lcd.init();

    systick_cfg();

    int cnt = 0;
    int _tick;
    while (tick < 1000*60) {
        lcd.fillScreen(RED);
        lcd.fillScreen(GREEN);
        cnt += 2;
    }
    _tick = tick;

    lcd.setRotation(1);
    lcd.fillScreen(BLUE);
    lcd.setFont(&consola);
    lcd.printf(1, 1, "Tick: %d, Cnt: %d, fps: %f", _tick, cnt, (float)cnt/60.0);
    while(1)
        ;

    //f103 debug: 8.6fps, release inline asm: 59 fps, release obj: 33.5
}

static void test_drawPixel() {

	initGPIO();
	ILI932x lcd(cs, rs, wr, rd, rst, portA, portB);
    lcd.init();

    systick_cfg();

    int cnt = 0;
    int _tick;

    while (tick < 1000 * 60) {

        for (int i = 0; i < TFTWIDTH; ++i) {
            for (int j = 0; j < TFTHEIGHT; ++j) {
                lcd.drawPixel(i, j, RED);
            }
        }

        for (int i = 0; i < TFTWIDTH; ++i) {
            for (int j = 0; j < TFTHEIGHT; ++j) {
                lcd.drawPixel(i, j, GREEN);
            }
        }

        cnt += 2;
    }

    _tick = tick;

    lcd.setRotation(1);
    lcd.fillScreen(BLUE);
    lcd.setFont(&consola);
    lcd.printf(1, 1, "Tick: %d, Cnt: %d, fps: %f", _tick, cnt, (float)cnt/60.0);
    while(1)
        ;

    //f103 release:  6 fps
}

static void testPushColors()
{
	initGPIO();
	ILI932x lcd(cs, rs, wr, rd, rst, portA, portB);
    lcd.init();

    int cnt = 0;
    int _tick;

    const int n = 3200;
    int total = TFTWIDTH * TFTHEIGHT / n;

    uint16_t red[n], green[n];
    for (int i = 0; i < n; ++i) {
        red[i] = RED;
        green[i] = GREEN;
    }

    systick_cfg();
    lcd.setAddrWindow(0, 0, TFTWIDTH, TFTHEIGHT);
    while (tick < 1000*60) {

        lcd.startPushColors();
        for (int i = 0; i < total; ++i)
            lcd.pushColors(red, n);

        lcd.startPushColors();
        for (int i = 0; i < total; ++i)
            lcd.pushColors(green, n);

        cnt += 2;
    }

    _tick = tick;

    lcd.setRotation(1);
    lcd.setFont(&consola);
    lcd.fillScreen(BLUE);

    lcd.printf(1, 1, "Tick: %d, Cnt: %d, fps: %f", _tick, cnt, (float)cnt/60.0);

    while (1)
        ;

    //f103: release:  37 fps
}

void testPushColors2()
{
	initGPIO();
    ILI932x lcd(cs, rs, wr, rd, rst, portA, portB);
    lcd.init();

    const int n = 3000;
    int total = TFTWIDTH * TFTHEIGHT / n;

    uint16_t red[n], green[n];
    for (int i = 0; i < n; ++i) {
        red[i] = RED;
        green[i] = GREEN;
    }

    int cnt = 0;
    int _tick;

    systick_cfg();

    while (tick < 1000*60) {

        lcd.setAddrWindow(0, 0, TFTWIDTH, TFTHEIGHT);
        lcd.startPushColors();
        for (int i = 0; i < total; ++i)
            lcd.pushColors2(red, n);

        lcd.setAddrWindow(0, 0, TFTWIDTH, TFTHEIGHT);
        lcd.startPushColors();
        for (int i = 0; i < total; ++i)
            lcd.pushColors2(green, n);

        cnt += 2;
    }

    _tick = tick;

    lcd.setRotation(1);
    lcd.setFont(&consola);
    lcd.fillScreen(BLUE);

    lcd.printf(1, 1, "Tick: %d, Cnt: %d, fps: %f", _tick, cnt, (float)cnt/60.0);

    while (1)
        ;

    //f103: release:  36 fps
}

static void testPushColorsAsm()
{
	initGPIO();
    ILI932x lcd(cs, rs, wr, rd, rst, portA, portB);
    lcd.init();

    const int n = 3000;
    uint16_t red2[n], green2[n];
    for (int i = 0; i < n; ++i) {
        red2[i] = RED;
        green2[i] = GREEN;
    }

    int cnt = 0;
    int _tick;
    systick_cfg();
    while (tick < 1000*60) {

        lcd.setAddrWindow(0, 0, TFTWIDTH, TFTHEIGHT);
        lcd.startPushColors();
        for (int i = 0; i < 25; ++i)
            lcd.pushColors3(red2, n);

        lcd.setAddrWindow(0, 0, TFTWIDTH, TFTHEIGHT);
        lcd.startPushColors();
        for (int i = 0; i < 25; ++i)
            lcd.pushColors3(green2, n);

        cnt += 2;
    }

    _tick = tick;

    lcd.setRotation(1);
    lcd.setFont(&consola);
    lcd.fillScreen(BLUE);

    lcd.printf(1, 1, "Tick: %d, Cnt: %d, fps: %f", _tick, cnt, (float)cnt/60.0);

    while (1)
        ;

    //f103: debug: ,  release: 37
}

/////////////////////////////////////////
static void test() {

	initGPIO();

    ILI932x lcd(cs, rs, wr, rd, rst, portA, portB);
    lcd.init();

    uint16_t id2 = lcd.readID();
    //lcd.setTextColor(0x9393, 0xE0E0);

    lcd.drawFastHLine(0, 10, TFTHEIGHT, RED);
    //while(1)
    //  ;

    lcd.fillRect(0, 0, TFTWIDTH, TFTHEIGHT, BLUE);
    delay(3000);

}

static void test2() {

	initGPIO();

    ILI932x* lcd =  new ILI932x(cs, rs, wr, rd, rst, portA, portB);
    lcd->init();

    uint16_t id2 = lcd->readID();
    //lcd.setTextColor(0x9393, 0xE0E0);

    lcd->drawFastHLine(0, 10, TFTHEIGHT, RED);
    delay(3000);

    lcd->fillRect(0, 0, TFTWIDTH, TFTHEIGHT, BLUE);
    while(1)
      ;

}

static void test_font() {
	initGPIO();
    ILI932x lcd(cs, rs, wr, rd, rst, portA, portB);
    lcd.init();
    lcd.setRotation(1);
    lcd.setTextSize(2);
    lcd.fillScreen(BLUE);
    lcd.setTextColor(YELLOW);
    lcd.fillScreen(BLUE);
    /*
    lcd.setFont(&coure);
	lcd.printf(0, 0, "123456789012345678901234");
	lcd.printf(14, 1, "2AbcdEF");
	lcd.printf(0, 2, "3AbcdEF lucon");
	lcd.printf(0, 3, "4AbcdEF");
	lcd.printf(0, 4, "5AbcdEF");
	lcd.printf(0, 5, "6AbcdEF");
	lcd.printf(0, 6, "7AbcdEF");
	lcd.printf(0, 7, "8AbcdEF");
	lcd.printf(0, 8, "9AbcdEF");
	lcd.printf(0, 9, "0AbcdEF");
     */
    lcd.setFont(&digital_7_monoitalic);
    lcd.printf(2,2, "11:40");

    while(1)
      ;

}

#if 0
extern "C" void initialise_monitor_handles(void);
#include <stdio.h>

static void test_semihosting()
{

    //initialise_monitor_handles();
    int i = 0;
	initGPIO();
    ILI932x lcd(cs, rs, wr, rd, rst, portA, portB);
    lcd.init();

    systick_cfg();

    while (1)
    {
    	printf ("%d\n", i++);
    	delay(1000);
    }
}
#endif

int main() {
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
    Debug_EnableCYCCNT(true);
    test_font();
}
