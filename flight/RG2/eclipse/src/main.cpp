#include <stdlib.h>
#include <stdio.h>
#include "RCC_helper.h"
#include "GPIO_Helper.h"
#include "ESC_PWM_Control.h"
#include "FlightLoop.h"
#include "debug.h"

#include "GPIO_Helper.h"
#include "LED_Indicators.h"
#include "RC_PWM_Reader.h"

#include "_pins.h"
#include "telemetry.h"
#include "system_time.h"
#include "buzzer.h"
#include "main_button_handler.h"
#include "lucon9.h"

volatile void* ptr;

static void* I2C_Error (void* arg)
{
    UNUSED(arg);
    static volatile int i = 0;
    ++i;
//  (i % 2) ? LED_Indicators::GetInstance()->MotorLedOn() :
//      LED_Indicators::GetInstance()->MotorLedOff();

}


static TFT_ILI9163C* get_lcd()
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

    TFT_ILI9163C* lcd = TFT_ILI9163C::GetInstance();
    lcd->init(spi, dcPin, rstPin, ssPin);
    lcd->setFont(&lucon);
    lcd->setTextColor(RED);
    lcd->fillScreen(COLOR565(0,255,255));
    lcd->setTextColor(BLACK);
    lcd->printf(0, 1, "test");

    lcd->display();
    return lcd;

}

static void run2(uint32_t i2cClockSpeed)
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    GPIOC::GetInstance()->EnablePeripheralClock(true);
    GPIOD::GetInstance()->EnablePeripheralClock(true);
    GPIOE::GetInstance()->EnablePeripheralClock(true);

    TFT_ILI9163C* lcd = get_lcd();
    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    Buzzer*  buzzer = Buzzer::GetInstance();
    buzzer->Init(BUZZER_TIMER, BUZZER_PIN, BUZZER_TIMER_CHANNEL);
    buzzer->SetVolume(100);
    buzzer->PlayTone(1000, 100);
    MainButtonHandler appl(TIMER::Get_Instance(BUTTON_TIMER), lcd, i2c, buzzer);
    appl.Run();

}

void debug_init(TFT_ILI9163C* lcd);
void debug (telemetry* telemetry);

static void run(uint32_t i2cClockSpeed)
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    GPIOC::GetInstance()->EnablePeripheralClock(true);
    GPIOD::GetInstance()->EnablePeripheralClock(true);
    GPIOE::GetInstance()->EnablePeripheralClock(true);

    ESC_PWM_Control esc (GPIO_Helper::SetupTimer(MOTOR_TIMER));

    RC_PWM_Reader pwmReader((TIMER_With_Channels*)TIMER::Get_Instance(PWM_CAPTURE_TIMER));


    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    i2c->SetErrorCallback (I2C_Error, &esc);

    TFT_ILI9163C* lcd = get_lcd();
    ptr = (void*)lcd;
    int s = sizeof (*lcd);

    // debug_init(lcd);
    lcd->clearScreen();
    lcd->display();

    FlightLoop* runner = new FlightLoop(i2c, 0, &esc, &pwmReader);
    // runner.SetDebugHook(debug, 20);
    runner->Run(lcd);


    while(1);

}

void HardFault_Handler()
{
	while(1);
}

void BusFault_Handler()
{
	while(1);

}

void MemManage_Handler()
{
	while(1);

}

void UsageFault_Handler()
{
	while(1);

}

int main()
{
    RCC_EnableHSE_168Mhz();
    RCC_EnableHSI(false);
    systick_enable(true);
    Debug_EnableCYCCNT(true);

    uint32_t i2cClockSpeed = 100000;
    run2(i2cClockSpeed);

    while(1);
}
