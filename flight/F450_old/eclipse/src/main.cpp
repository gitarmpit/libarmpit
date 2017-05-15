#include "timer_cpp.h"
#include "rcc.h"
#include "RCC_helper.h"
#include "GPIO_Helper.h"
#include "ESC_PWM_Control.h"
#include "FlightLoop.h"

#include "GPIO_Helper.h"
#include "LED_Indicators.h"
#include "RC_PWM_Reader.h"

#include "_pins.h"
#include "debug.h"


static void* I2C_Error (void* arg)
{
    UNUSED(arg);
    static volatile int i = 0;
    ++i;

    if (i % 2)
    {
        LED_Indicators::GetInstance()->MotorLedOff();
    }
    else
    {
        LED_Indicators::GetInstance()->MotorLedOn();
    }

}

static void run(uint32_t i2cClockSpeed, uint16_t radioRateKbit, uint32_t radioFreqMhz)
{

#ifdef USE_LCD

    //LCD
    SPI* lcdSPI = GPIO_Helper::SetupSPI(LCD_RADIO_SPI, true, false, false, SPI_BAUD_RATE_2);

    GPIO_PIN* rstPin = GPIO_Helper::GetPin(LCD_RST_PIN);
    rstPin->SetupGPIO_OutPP();

    GPIO_PIN* dcPin = GPIO_Helper::GetPin(LCD_DC_PIN);
    dcPin->SetupGPIO_OutPP();

    GPIO_PIN* lcd_ssPin = GPIO_Helper::GetPin(LCD_SS_PIN);
    lcd_ssPin->SetupGPIO_OutPP();

    Adafruit_PCD8544 lcd(lcdSPI, dcPin, rstPin, lcd_ssPin);
    lcd.Init(0xbc);
    lcd.setRotation(2);
    lcd.clearDisplay();
    lcd.write("   Team", 0, 0);
    lcd.write("Rote Gefahr", 0, 1);
    lcd.display();
    delay(2000);

#endif

#ifdef USE_RADIO

    GPIO_PIN* sdnPin = GPIO_Helper::GetPin(RADIO_SDN_PIN);
    sdnPin->SetupGPIO_OutPP();

    GPIO_PIN* radio_ssPin = GPIO_Helper::GetPin(RADIO_SS_PIN);
    radio_ssPin->SetupGPIO_OutPP();

    //radio and lcd share the same SPI
    Si4432 radio(lcdSPI, radio_ssPin, sdnPin, 0);

    radio.init();
    radio.setBaudRate(radioRateKbit);
    radio.setFrequency(radioFreqMhz);


#endif


    ESC_PWM_Control esc (GPIO_Helper::SetupTimer(MOTOR_TIMER));

    RC_PWM_Reader pwmReader((TIMER_With_Channels*)TIMER::Get_Instance(PWM_CAPTURE_TIMER));


    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    i2c->SetErrorCallback (I2C_Error, &esc);


    SPI* proximitySPI = GPIO_Helper::SetupSPI(PROXIMITY_SPI, true, false, false, SPI_BAUD_RATE_2);

    FlightLoop runner (i2c, proximitySPI, &esc, &pwmReader, &lcd, &radio);
    runner.Run();

    LED_Indicators::GetInstance()->RGB_BlinkingBlue(1000, 500, 2000);

    while(1);

}


void tests();

int main()
{
    RCC_EnableHSI_168Mhz();
    RCC_SetAHBPrescalerDiv2();

    //tests();

    uint32_t i2cClockSpeed = 100000;

    uint16_t radioRateKbit = 50;
    uint32_t radioFreqMhz = 433;
    run(i2cClockSpeed, radioRateKbit, radioFreqMhz);

    while(1);
}
