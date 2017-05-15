#include "timer_cpp.h"
#include "debug.h"
#include "RCC_helper.h"
#include "GPIO_Helper.h"
#include "ESC_PWM_Control.h"

#include "GPIO_Helper.h"
#include "LED_Indicators.h"
#include "_pins.h"
#include "RF22/si4432.h"
#include "led_pwm.h"

#include "motionapp_reader.h"

static void radio_init_err()
{
    LED_Indicators* leds = LED_Indicators::GetInstance();
    leds->EnablePitchRollLeds(true);
    while(1)
    {
        leds->DisplayPitchRoll(10, 10);
        delay(1000);
        leds->DisplayPitchRoll(0, 0);
        delay(1000);

    }
}

static void test_radio()
{
    SPI* radioSPI = GPIO_Helper::SetupSPI(RADIO_SPI, true, false, false, SPI_BAUD_RATE_2);

    GPIO_PIN* sdnPin = GPIO_Helper::GetPin(RADIO_SDN_PIN);
    sdnPin->SetupGPIO_OutPP();

    GPIO_PIN* radio_ssPin = GPIO_Helper::GetPin(RADIO_SS_PIN);
    radio_ssPin->SetupGPIO_OutPP();

    //radio and lcd share the same SPI
    Si4432 radio(radioSPI, radio_ssPin, sdnPin, 0);

    bool rc = radio.init();

    if (!rc)
    {
        radio_init_err();
    }

    uint16_t radioRateKbit = 50;
    uint32_t radioFreqMhz = 433;

    radio.setBaudRate(radioRateKbit);
    radio.setFrequency(radioFreqMhz);

    static uint32_t radio_buf[128];

//    TIMER_With_Channels* t = GPIO_Helper::SetupTimer(PITCH_ROLL_LED_TIMER);
//    TIMER_Channel* up =   t->GetChannel(1);
//    TIMER_Channel* down = t->GetChannel(3);
//
//    up->EnableChannel(true);
//    up->SetupPWM1(10000, 0);
//    down->EnableChannel(true);
//    down->SetupPWM1(10000, 0);
//
//    t->EnableCounter(true);

    LED_PWM led_up (GPIO_Helper::SetupTimer(PITCH_ROLL_LED_TIMER), 1, 1000);


    uint8_t i = 0;
    while (1)
    {
        radio_buf[0] = 111;
        radio_buf[1] = 222;
        radio_buf[2] = 333;
        uint32_t maxRetry = 150;
        int rc  = radio.trySendPacket(12, (uint8_t *) radio_buf, maxRetry);
        if (rc == 0)
        {
            if ((++i % 2) == 0)
            {
                //up->UpdatePWMDuty(100);
                led_up.On(10);
            }
            else
            {
                //up->UpdatePWMDuty(0);
                led_up.Off();
            }

            delay(20);
        }
        else if (rc == 2)
        {
            delay(20);
        }

    }

}

static void test_single_led (const char* pin_name)
{
    GPIO_PIN* pin = GPIO_Helper::GetPin(pin_name);
    pin->SetupGPIO_OutPP();
    pin->Toggle();
    delay(1000);
    pin->Toggle();
    delay(1000);

}

static void test_led_pwm()
{
    LED_PWM led_pwm (GPIO_Helper::SetupTimer(PITCH_ROLL_LED_TIMER), 1, 1000);
    led_pwm.On();
    led_pwm.Off();
    led_pwm.On();
    led_pwm.On(500);
    led_pwm.On(100);
    led_pwm.On(50);
    led_pwm.On(10);
    led_pwm.On(5);
    led_pwm.On(1);


}

static void test_LED()
{

    test_single_led(LED_U);
    test_single_led(LED_R);
    test_single_led(LED_D);
    test_single_led(LED_L);

    LED_Indicators* leds = LED_Indicators::GetInstance();

//    leds->RGB_SteadyRed(1000);
//    leds->RGB_ToggleRed();
//    leds->RGB_ToggleRed();
//    leds->RGB_ToggleRed();
//
//    leds->RGB_SteadyGreen(1000);
//    leds->RGB_ToggleGreen();
//    leds->RGB_ToggleGreen();
//    leds->RGB_ToggleGreen();
//
//    leds->RGB_SteadyBlue(1000);
//    leds->RGB_ToggleBlue();
//    leds->RGB_ToggleBlue();
//    leds->RGB_ToggleBlue();
//
//    leds->ToggleRGB();
//    leds->ToggleRGB();
//    leds->ToggleRGB();
//
//    leds->RGB_Off();
//
//    leds->RGB_BlinkingRed(100, 50, 1000);
//    leds->RGB_BlinkingGreen(100, 50, 1000);
//    leds->RGB_BlinkingBlue(100, 50, 1000);
//
//    leds->RGB_Off();


    leds->EnablePitchRollLeds(true);

    leds->DisplayPitchRoll(1, 1);
    leds->DisplayPitchRoll(3, 3);
    leds->DisplayPitchRoll(7, 7);
    leds->DisplayPitchRoll(10, 10);
    leds->DisplayPitchRoll(15, 15);
    leds->DisplayPitchRoll(20, 20);
    leds->DisplayPitchRoll(30, 30);

    delay(1000);
    leds->DisplayPitchRoll(-30, -30);
    delay(1000);
    leds->EnablePitchRollLeds(false);



}


static void test_pwm_out()
{
    TIMER_With_Channels* pwmTimer = GPIO_Helper::SetupTimer(MOTOR_TIMER);
    ESC_PWM_Control esc (pwmTimer);
    esc.SetDuty(700, 1000, 1500, 2000);
    while(7)
        ;

}

static void* i2c_callback (void* arg)
{
    while(1)
    {
        test_single_led(LED_U);
        test_single_led(LED_R);
        test_single_led(LED_D);
        test_single_led(LED_L);
    }
}

static void test_motionapp()
{
//    Adafruit_PCD8544* lcd = get_LCD();
//
    uint32_t i2cClockSpeed = 100000;
    static I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    i2c->SetErrorCallback (i2c_callback, i2c);

    int i = 0;
    float y, p, r;
    MotionAppReader read (i2c);
    read.Init();
    while (1)
    {
        //lcd->clearDisplay();

        read.GetYPR(&y, &p, &r);
        if ((i++ % 50) == 0)
        {
//            lcd->printf (0, 0, "y: %5.1f", y);
//            lcd->printf (0, 0, "p: %5.1f", p);
//            lcd->printf (0, 0, "r: %5.1f", r);
//            lcd->display();
        }

    }

}

void tests()
{
    //test_LED();
    //test_motionapp();
    test_radio();
    //test_led_pwm();
}
