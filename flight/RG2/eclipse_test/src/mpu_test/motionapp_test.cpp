#include "GPIO_Helper.h"
#include "9163/TFT_ILI9163C.h"
#include "_pins.h"
#include "LED_Indicators.h"
#include "system_time.h"
#include "motionapp_reader.h"
#include "buzzer.h"
#include  "button_handler/button.h"
#include "bitmaps/menu5_16.cpp"
#include "debug.h"

 TFT_ILI9163C* get_lcd();
static void* I2C_Error (void* arg)
{
    UNUSED(arg);
    static volatile int i = 0;
    static LED_Indicators* led = LED_Indicators::GetInstance();
    ++i;

    //((I2C*)arg)->Reinitialize();

    return 0;

}

static void draw_hairline(TFT_ILI9163C* lcd)
{
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

}


void motionapp_test()
{

    TFT_ILI9163C* lcd = get_lcd();

    systick_enable(true);

    Buzzer*  buzzer = Buzzer::GetInstance();
    buzzer->Init(BUZZER_TIMER, BUZZER_PIN, BUZZER_TIMER_CHANNEL);
    buzzer->SetVolume(2);
    //const char *t = "r1:d=8,o=7,b=160:c,eb,g,eb,4c,4p,c,eb,g,eb,4g,4p";
    //buzzer->PlayTuneAsync(t, TIMER::Get_Instance(TIM6), true);

    lcd->setBgColor(0);
    lcd->setTextColor(CYAN);
    lcd->setTextSize(2);
    lcd->clearScreen();
    lcd->setTextColor(YELLOW);
    lcd->printf (0, 0, "Calibration mode");

    lcd->setTextColor(CYAN);
    lcd->printf (0, 2, "Initializing sensors...");
    lcd->display();
//    lcd->drawEllipse(lcd->width()/2, lcd->height()/2, 13, 12, CYAN);


    uint32_t i2cClockSpeed = 100000;
    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    i2c->SetErrorCallback (I2C_Error, i2c);

    int i = 0;
    float y, p, r;
    MotionAppReader read (i2c);
    read.Init();
    char buf[64];
    float y0 = 0;
    bool stabilized = false;
    int stable_count = 0;
    bool buzzer_on = false;
    uint32_t avg_t = 0;
    bool bulleye = false;
    int16_t circle_color;
    while (1)
    {
        uint32_t t0 = micros();
        read.GetYPR(&y, &p, &r);
        uint32_t t1 = micros();
        avg_t += (t1 - t0);
        if (fabs(y - y0) < 0.01)
        {
            ++stable_count;
        }
        else
        {
            stable_count = 0;
        }
        y0 = y;
        if ((++i % 50) == 0)
        {
            if (buzzer_on)
            {
                buzzer->Stop();
                buzzer_on = false;
            }
            if (fabs(p) < 0.9 && fabs(r) < 0.9)
            {
                circle_color = RED;
                if (!bulleye)
                {
                    bulleye = true;
                    buzzer->PlayTone (2000);
                    buzzer_on = true;
                }
            }
            else
            {
                circle_color = YELLOW;
                bulleye = false;
            }
            lcd->clearScreen();
            lcd->setTextColor(YELLOW);
            lcd->printf (0, 0, "Calibration mode");
            lcd->drawFastHLine(0, 14, 128, YELLOW);
            lcd->setTextColor(CYAN);
            lcd->printf (0, 1, "p: %4.2f", p);
            lcd->printf (0, 2, "r: %4.2f", r);
            lcd->printf (10, 1, "y: %4.2f", y);
            lcd->printf (10, 2, "bull: %d", bulleye);
//            if (stable_count > 10000)
//            {
//                lcd->setTextColor(YELLOW);
//                lcd->write("yaw stabilized", 0, 3);
//                lcd->setTextColor(CYAN);
//            }
//            lcd->printf (0, 5, "telapsed: %u", t1);
//            lcd->printf (0, 6, "avg: %u", avg_t / i);

            //draw_hairline(lcd);
            lcd->drawFastHLine(0, 64, 128, CYAN);
            lcd->drawFastVLine(64, 14, 128, CYAN);
            lcd->drawEllipse(64 + (int)(r*5.), 64 + (int)(p*5.), 13, 12, circle_color);
            if (bulleye)
            {
                lcd->drawEllipse(64 + (int)(r*5.), 64 + (int)(p*5.), 12, 11, circle_color);
            }
            lcd->drawPixel(64 + (int)(r*5.), 64 + (int)(p*5.), circle_color);
            lcd->displayFast();
            //LED_Indicators::GetInstance()->ToggleRGB();
        }

   }
   delay(100);
}


void motionapp_test2()
{
    TFT_ILI9163C* lcd = get_lcd();

    lcd->setBgColor(0);
    lcd->setTextColor(CYAN);
    lcd->setTextSize(2);
    lcd->clearScreen();
    lcd->setTextColor(YELLOW);

    uint32_t i2cClockSpeed = 100000;
    static I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    i2c->SetErrorCallback (I2C_Error, i2c);

    int i = 0;
    float y, p, r;
    MotionAppReader read (i2c);
    read.Init();
    while (1)
    {
        lcd->clearScreen();
        read.GetYPR(&y, &p, &r);
        if ((i++ % 50) == 0)
        {
//            lcd->drawBmp(0, 0, menu5_16);
            lcd->printf (0, 0, "%5.1f", p);
            lcd->display();
        }

    }

    delay(100);
}
