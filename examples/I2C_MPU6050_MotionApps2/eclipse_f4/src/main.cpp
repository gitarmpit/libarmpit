#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "i2c_cpp.h"
#include "_pins.h"
#include "LED_Indicators.h"
#include "5110/Adafruit_PCD8544.h"
#include <stdio.h>
#include "fp2.h"
#include <math.h>
#include "systick.h"
#include "debug.h"
#include "system_time.h"
#include "timer_cpp.h"
#include "motionapp_reader.h"

static void* I2C_Error (void* arg)
{
    UNUSED(arg);
    static volatile int i = 0;
    static LED_Indicators* led = LED_Indicators::GetInstance();
    ++i;

    if (i % 2)
    {
        led->MotorLedOff();
    }
    else
    {
        led->MotorLedOn();
    }

    ((I2C*)arg)->Reinitialize();

    return 0;

}


static Adafruit_PCD8544 get_lcd(SPI* spi)
{
    delay(10);
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);


    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN1);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN0);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = portB->GetPin(GPIO_PIN2);
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

    Adafruit_PCD8544 lcd(spi, dcPin, rstPin, ssPin);
    lcd.Init(0xbc);
    lcd.setRotation(2);
    lcd.clearDisplay();

    return lcd;
}


static void test()
{
    LED_Indicators::GetInstance()->MotorLedOn();
    LED_Indicators::GetInstance()->MotorLedOff();

    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_16);
    Adafruit_PCD8544 lcd = get_lcd(spi);
    lcd.write ("MotionApps2");
    lcd.display();
    delay(1500);

    uint32_t i2cClockSpeed = 100000;
    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    i2c->SetErrorCallback (I2C_Error, i2c);
    delay(500);
    LED_Indicators::GetInstance()->EnablePitchRollLeds(true);

    int i = 0;
    float y, p, r;
    MotionAppReader read (i2c);
    read.Init();
    while (1)
    {
        read.GetYPR(&y, &p, &r);
        if ((++i % 50) == 0)
        {
            lcd.clearDisplay();
            lcd.printf(2, 0, 0, "%6.2f", y);
            lcd.printf(2, 0, 1, "%6.2f", p);
            lcd.printf(2, 0, 2, "%6.2f", r);
            lcd.display();
            LED_Indicators::GetInstance()->ToggleRGB();
        }

        LED_Indicators::GetInstance()->DisplayPitchRoll((int8_t) p, (int8_t) r);
   }
}


int main()
{
    RCC_EnableHSI_168Mhz();
    test();
    while(1);

    return 0;
}
