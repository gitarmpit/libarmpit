#include "timer_cpp.h"
#include "rcc.h"
#include "RCC_helper.h"
#include "GPIO_Helper.h"
#include "ESC_PWM_Control.h"

#include "GPIO_Helper.h"
#include "LED_Indicators.h"
#include "RC_PWM_Reader.h"
#include "RF22/si4432.h"

#include "_pins.h"
#include "5110/Adafruit_PCD8544.h"
#include "motionapp_reader.h"
#include "debug.h"


Adafruit_PCD8544* get_LCD()
{
    SPI* lcdSPI = GPIO_Helper::SetupSPI(LCD_RADIO_SPI, true, false, false, SPI_BAUD_RATE_16);

    GPIO_PIN* rstPin = GPIO_Helper::GetPin(LCD_RST_PIN);
    rstPin->SetupGPIO_OutPP();

    GPIO_PIN* dcPin = GPIO_Helper::GetPin(LCD_DC_PIN);
    dcPin->SetupGPIO_OutPP();

    GPIO_PIN* lcd_ssPin = GPIO_Helper::GetPin(LCD_SS_PIN);
    lcd_ssPin->SetupGPIO_OutPP();

    Adafruit_PCD8544* lcd =  new Adafruit_PCD8544(lcdSPI, dcPin, rstPin, lcd_ssPin);
    lcd->Init(0xbc);
    lcd->setRotation(2);
    lcd->clearDisplay();
    lcd->display();

    return lcd;

}

static void test_LCD()
{
//    SPI* lcdSPI = GPIO_Helper::SetupSPI(LCD_RADIO_SPI, true, false, false, SPI_BAUD_RATE_16);
//
//    GPIO_PIN* rstPin = GPIO_Helper::GetPin(LCD_RST_PIN);
//    rstPin->SetupGPIO_OutPP();
//
//    GPIO_PIN* dcPin = GPIO_Helper::GetPin(LCD_DC_PIN);
//    dcPin->SetupGPIO_OutPP();
//
//    GPIO_PIN* lcd_ssPin = GPIO_Helper::GetPin(LCD_SS_PIN);
//    lcd_ssPin->SetupGPIO_OutPP();
//
//    Adafruit_PCD8544 lcd(lcdSPI, dcPin, rstPin, lcd_ssPin);

    Adafruit_PCD8544* lcd = get_LCD();

    lcd->Init(0xbc);
    lcd->setRotation(2);
    lcd->clearDisplay();
    lcd->write("   Team", 0, 0);
    lcd->write("Rote Gefahr", 0, 1);
    lcd->display();

    while(1)
        ;


}


static void test_radio()
{
    SPI* radioSPI = GPIO_Helper::SetupSPI(LCD_RADIO_SPI, true, false, false, SPI_BAUD_RATE_4);

    GPIO_PIN* sdnPin = GPIO_Helper::GetPin(RADIO_SDN_PIN);
    sdnPin->SetupGPIO_OutPP();

    GPIO_PIN* radio_ssPin = GPIO_Helper::GetPin(RADIO_SS_PIN);
    radio_ssPin->SetupGPIO_OutPP();

    //radio and lcd share the same SPI
    Si4432 radio(radioSPI, radio_ssPin, sdnPin, 0);

    LED_Indicators* leds = LED_Indicators::GetInstance();

    bool rc = radio.init();

    if (!rc)
    {
        while(1)
        {
            leds->RGB_ToggleRed();
            delay(1000);
        }
    }

    uint16_t radioRateKbit = 50;
    uint32_t radioFreqMhz = 433;

    radio.setBaudRate(radioRateKbit);
    radio.setFrequency(radioFreqMhz);

    static uint32_t radio_buf[128];

    uint8_t i1 = 0, i2 = 0;

    while (1)
    {
        radio_buf[0] = 111;
        radio_buf[1] = 222;
        radio_buf[2] = 333;
        uint32_t maxRetry = 150;
        int rc  = radio.trySendPacket(12, (uint8_t *) radio_buf, maxRetry);
        if (rc == 0)
        {
            if ((i1++ % 2) == 0)
            {
                leds->RGB_SteadyGreen(0);
            }
            else
            {
                leds->RGB_Off();
            }
        }
        else if (rc == 2)
        {
            if ((i2++ % 2) == 0)
            {
                leds->RGB_SteadyRed(0);
            }
            else
            {
                leds->RGB_Off();
            }
        }

        delay(50);
    }

}

static void test_LED()
{
    LED_Indicators* leds = LED_Indicators::GetInstance();

    leds->RGB_SteadyRed(1000);
    leds->RGB_ToggleRed();
    leds->RGB_ToggleRed();
    leds->RGB_ToggleRed();

    leds->RGB_SteadyGreen(1000);
    leds->RGB_ToggleGreen();
    leds->RGB_ToggleGreen();
    leds->RGB_ToggleGreen();

    leds->RGB_SteadyBlue(1000);
    leds->RGB_ToggleBlue();
    leds->RGB_ToggleBlue();
    leds->RGB_ToggleBlue();

    leds->ToggleRGB();
    leds->ToggleRGB();
    leds->ToggleRGB();

    leds->RGB_Off();

    leds->RGB_BlinkingRed(100, 50, 1000);
    leds->RGB_BlinkingGreen(100, 50, 1000);
    leds->RGB_BlinkingBlue(100, 50, 1000);

    leds->RGB_Off();


    leds->MotorLedOn();
    leds->MotorLedOff();

    leds->EnablePitchRollLeds(true);

    leds->DisplayPitchRoll(30, 30);
    delay(1000);
    leds->DisplayPitchRoll(-30, -30);
    delay(1000);
    leds->EnablePitchRollLeds(false);

    leds->EnableThrottleLevelLeds(true);
    leds->DisplayThrottleLevel(2000, 2000, 1700, 1700);
    leds->EnableThrottleLevelLeds(false);


}


static void test_pwm_out()
{
    TIMER_With_Channels* pwmTimer = GPIO_Helper::SetupTimer(MOTOR_TIMER);
    ESC_PWM_Control esc (pwmTimer);
    esc.SetDuty(700, 1000, 1500, 2000);
    while(7)
        ;

}

static void test_rc_pwm_in()
{
    Adafruit_PCD8544* lcd = get_LCD();


    uint16_t thr;
    RC_PWM_Reader pwmReader ((TIMER_With_Channels*)TIMER::Get_Instance(PWM_CAPTURE_TIMER));
    pwmReader.Start();

    TIMER_With_Channels* pwmTimer = GPIO_Helper::SetupTimer(MOTOR_TIMER);
    ESC_PWM_Control esc (pwmTimer);

    while(1)
    {
        lcd->clearDisplay();
        lcd->printf(0, 0, "t: %d", pwmReader.GetThrottle());
        lcd->printf(0, 1, "r: %d", pwmReader.GetRoll());
        lcd->printf(0, 2, "p: %d", pwmReader.GetPitch());
        lcd->display();
        esc.SetDuty(thr);
        delay(100);
    }
}

static void test_pwm_capture()
{
    Adafruit_PCD8544* lcd = get_LCD();
    TIMER_With_Channels* timer_in = (TIMER_With_Channels*)TIMER::Get_Instance(PWM_CAPTURE_TIMER);
    PWM_Capture_EXTI handler (timer_in, PWM_INPUT_THROTTLE, 0);
    volatile uint32_t period, duty;

    while(1)
    {
        lcd->clearDisplay();
        duty = handler.GetDuty_us();
        period = handler.GetPeriod_us();
        lcd->printf(0, 0, "period: %d", period);
        lcd->printf(0, 1, "duty: %d", duty);
        lcd->display();
        delay(100);
    }

}

static void* I2C_Error (void* arg)
{

    while(1)
        ;
}

static void test_motionapp()
{
    Adafruit_PCD8544* lcd = get_LCD();

    uint32_t i2cClockSpeed = 100000;
    static I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    i2c->SetErrorCallback (I2C_Error, i2c);

    int i = 0;
    float y, p, r;
    MotionAppReader read (i2c);
    read.Init();
    while (1)
    {
        lcd->clearDisplay();

        read.GetYPR(&y, &p, &r);
        if ((i++ % 50) == 0)
        {
            lcd->printf (0, 0, "y: %5.1f", y);
            lcd->printf (0, 1, "p: %5.1f", p);
            lcd->printf (0, 2, "r: %5.1f", r);
            lcd->display();
        }

    }

}

void tests()
{
    test_radio();
    //test_LED();
    //test_LCD();
    //test_motionapp();
}
