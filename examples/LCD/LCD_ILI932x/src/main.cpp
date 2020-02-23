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
#include "system_time.h"
#include "GPIO_Helper.h"

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

static GPIO_PORT* _dataPort;
static GPIO_PIN *cs, *rs, *rd, *wr, *rst;


//f103, f407 directly to pins, no socket
static void initGPIO()
{
	_dataPort = GPIOA::GetInstance();
    _dataPort->EnablePeripheralClock(true);

    GPIOB* portB = GPIOB::GetInstance();
    portB->EnablePeripheralClock(true);

    rst = portB->GetPin(GPIO_PIN11);
    rst->SetupGPIO_OutPP();
    rst->SetSpeedHigh();

    rst->Set();

    cs = portB->GetPin(GPIO_PIN0);
    cs->SetupGPIO_OutPP();
    cs->SetSpeedHigh();

    rs = portB->GetPin(GPIO_PIN1);
    rs->SetupGPIO_OutPP();
    rs->SetSpeedHigh();

    wr = portB->GetPin(GPIO_PIN10);
    wr->SetupGPIO_OutPP();
    wr->SetSpeedHigh();

    rd = portB->GetPin(GPIO_PIN12);
    rd->SetupGPIO_OutPP();
    rd->SetSpeedHigh();

    delay(500);

}

//411 socket
static void initGPIO_411()
{
	_dataPort = GPIOC::GetInstance();
    _dataPort->EnablePeripheralClock(true);
	GPIOB* portB = GPIOB::GetInstance();
    portB->EnablePeripheralClock(true);

    rst = portB->GetPin(GPIO_PIN5);
    rst->SetupGPIO_OutPP();
    rst->SetSpeedHigh();

    rst->Set();

    cs = portB->GetPin(GPIO_PIN6);
    cs->SetupGPIO_OutPP();
    cs->SetSpeedHigh();

    rs = portB->GetPin(GPIO_PIN7);
    rs->SetupGPIO_OutPP();
    rs->SetSpeedHigh();

    wr = portB->GetPin(GPIO_PIN8);
    wr->SetupGPIO_OutPP();
    wr->SetSpeedHigh();

    rd = portB->GetPin(GPIO_PIN9);
    rd->SetupGPIO_OutPP();
    rd->SetSpeedHigh();

}


/////////////////////////////////////////
static void test_flood() {

	initGPIO();
    ILI932x lcd(cs, rs, wr, rd, rst, _dataPort);
    lcd.init();

    systick_cfg();

    int cnt = 0;
    int _tick;
    while (tick < 1000*60) {
        lcd.fillScreen(0xf0f0);
        lcd.fillScreen(0x0f0f);
        cnt += 2;
    }
    _tick = tick;

    lcd.fillScreen(BLUE);
    lcd.setRotation(1);
    lcd.setFont(&consola);
    lcd.printf(1, 1, "Tick: %d, Cnt: %d, fps: %7.3f", _tick, cnt, (float)cnt/60.0);
    while(1)
        ;

    //f103 debug: 8.6fps, release inline asm: 59 fps, release obj: 33.5
    //f407 240Mhz (4 nops): 182fps (2 bytes different), 182: 2 bytes same
}

static void test_drawPixel() {

	initGPIO();
	ILI932x lcd(cs, rs, wr, rd, rst, _dataPort);
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
	ILI932x lcd(cs, rs, wr, rd, rst, _dataPort);
    lcd.init();
    //lcd.fillScreen(BLUE);
    //int id = lcd.readID();
    //lcd.printf (3, 3, "ID: %x", id);
    //delay(1000);

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
    while (tick < 1000*60) {

        lcd.setAddrWindow(0, 0, TFTWIDTH, TFTHEIGHT);
        lcd.startPushColors();
        for (int i = 0; i < total; ++i)
            lcd.pushColors(red, n);

        lcd.setAddrWindow(0, 0, TFTWIDTH, TFTHEIGHT);
        lcd.startPushColors();
        for (int i = 0; i < total; ++i)
            lcd.pushColors(green, n);

        cnt += 2;
    }

    _tick = tick;


    lcd.setRotation(1);
    lcd.fillScreen(BLUE);
    lcd.setFont(&consola);
    lcd.printf(1, 1, "Tick: %d, Cnt: %d, fps: %f", _tick, cnt, (float)cnt/60.0);

    while (1)
        ;

    //f103: 37 fps, f407 at 168Mhz: 97fps, f411 at 100Mhz: 65fps
}

static void testPushColors_407()
{
	initGPIO();
	ILI932x lcd(cs, rs, wr, rd, rst, _dataPort);
    lcd.init();
    //lcd.fillScreen(BLUE);
    //int id = lcd.readID();
    //lcd.printf (3, 3, "ID: %x", id);
    //delay(1000);

    int cnt = 0;
    int _tick;

    const int n = TFTWIDTH * TFTHEIGHT / 4;
    int total = TFTWIDTH * TFTHEIGHT / n;

    uint16_t red[n], green[n];
    for (int i = 0; i < n; ++i) {
        red[i] = RED;
        green[i] = GREEN;
    }

    lcd.setAddrWindow(0, 0, TFTWIDTH, TFTHEIGHT);
    systick_cfg();
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
    lcd.fillScreen(BLUE);
    lcd.setFont(&consola);
    lcd.printf(1, 1, "Tick: %d, Cnt: %d, fps: %f", _tick, cnt, (float)cnt/60.0);

    while (1)
        ;

    //f103: 37 fps, f407 at 168Mhz: 97fps, f411 at 100Mhz: 65fps
}

static void testPushColors5()
{
	initGPIO();
	ILI932x lcd(cs, rs, wr, rd, rst, _dataPort);
    lcd.init();

    //F1/F411
    const int n = 3200;

    //F407
    //const int n = TFTWIDTH * TFTHEIGHT / 2;

    int total = TFTWIDTH * TFTHEIGHT / n;

    uint16_t color[n];

    for (int j = 0; j < 10; ++j)
    {
        for (int i = 0; i < n; ++i) {
            color[i] = (j % 2) ? 0xf0f0 : 0x0f0f;
        }
    	lcd.startPushColors();
    	for (int i = 0; i < total; ++i)
    		lcd.pushColors(color, n);
    	delay (500);
    }

    for (int i = 0; i < n; ++i) {
        color[i] = 0x0f0f;
    }

    systick_enable(TRUE);
    int m1 = millis();

    int cnt = 100;
	lcd.setAddrWindow(0, 0, TFTWIDTH, TFTHEIGHT);
    for (int j = 0; j < cnt; ++j)
    {
    	lcd.startPushColors();
    	for (int i = 0; i < total; ++i)
    		lcd.pushColors(color, n);
    }
    int m2 = millis() - m1;

    lcd.setRotation(1);
    lcd.setFont(&consola);
    lcd.fillScreen(BLUE);

    lcd.printf(1, 2, "Millis: %7.3f", (float)m2/cnt);
    lcd.printf(1, 3, "fps: %7.3f", 1000.*cnt/(float)m2);

    while (1)
        ;

    //f103: release:  37 fps
}

#ifdef STM32F1
void testPushColors2()
{
	initGPIO();
    ILI932x lcd(cs, rs, wr, rd, rst, _dataPort);
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
    ILI932x lcd(cs, rs, wr, rd, rst, _dataPort);
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

#endif

/////////////////////////////////////////
static void test() {

	initGPIO();

    ILI932x lcd(cs, rs, wr, rd, rst, _dataPort);
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

    ILI932x* lcd =  new ILI932x(cs, rs, wr, rd, rst, _dataPort);
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
    ILI932x lcd(cs, rs, wr, rd, rst, _dataPort);
    lcd.init();
    //delay(500);
    int id = lcd.readID();
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
    ILI932x lcd(cs, rs, wr, rd, rst, _dataPort);
    lcd.init();

    systick_cfg();

    while (1)
    {
    	printf ("%d\n", i++);
    	delay(1000);
    }
}
#endif

#ifdef STM32F1
static void initF1()
{
	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
	//HSE over clock: 128
	//FLASH_SetWaitState(3);
	//RCC_EnableHSE(TRUE);
	//RCC_EnablePLL(16);
}
#endif

#ifdef STM32F4
static void initF407()
{
    //RCC_EnableHSI_168Mhz();

    //f411
    //RCC_EnableHSI_100Mhz();

    /* 411
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(3);

    RCC_EnableHSI(TRUE);
    uint8_t pllm = 4;
    uint16_t plln = 145;
    uint16_t pllp = 4;
    RCC_EnablePLL(pllm, plln, pllp);
    */

    //f407
    //190(3 nops): 110fps
    //>190 requires 4 nops.
    //Max without distortion: 240Mhz(4 nops) 125fps
    //4:
    //200: 105fps (worse than 3 nops at 190)
    //210: 110fps (same as 3 nops at 190)
    //230: 120fps
    //240: 125fps : 8ms

    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(5);

    RCC_EnableHSI(TRUE);
    //F4: HSI=16
    //PLL=16/m/p * n
    //m=16 p=2 :  PLL = n/2
    uint8_t pllm = 16;
    uint16_t pllp = 2; //vco out / pllp = sysclock
    uint16_t plln = MCU_FREQ*2;
    RCC_EnablePLL(pllm, plln, pllp);


    // RCC_SetAHBPrescalerDiv2();
}
#endif

int main() {
#if defined(STM32F1)
	initF1();
#elif defined(STM32F4)
	initF407();
#endif
    Debug_EnableCYCCNT(true);
    testPushColors();
    //testPushColors5();
    //test_flood();
    //test_font();
}
