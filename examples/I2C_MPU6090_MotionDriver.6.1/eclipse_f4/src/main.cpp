#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "i2c_cpp.h"
#include "invensense/inv_mpu.h"
#include "invensense/inv_mpu_dmp_motion_driver.h"
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
#include "invensense/helper_3dmath.h"


void setI2C(I2C* i2c);
void basic_AHRS(Adafruit_PCD8544* lcd);
void basic_AHRS2(Adafruit_PCD8544* lcd);


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

class Update_Handler: public TIMER_Interrupt_Handler
{
public:

    virtual void HandleInterrupt(bool isUpdate, bool isTrigger)
    {
        static volatile int i = 0;
        static LED_Indicators* led = LED_Indicators::GetInstance();
        if (isUpdate)
        {
            led->RGB_ToggleRed();
            ++i;
        }
    }
};



static void systick_test2()
{
    SCB_EnableDWT(true);
    Debug_EnableCYCCNT(true);
    systick_enable(true);

    systick_timer_start();
    delay_us(7);
    volatile uint32_t dur1 = systick_timer_stop();

    systick_timer_start();
    delay_us(137);
    volatile uint32_t dur2 = systick_timer_stop();


    systick_timer_start();
    delay_us(724567);
    volatile uint32_t dur3 = systick_timer_stop();


    systick_timer_start();
    delay_us(7);
    volatile uint32_t dur4 = systick_timer_stop();

    systick_timer_start();
    delay_us(1234567);
    volatile uint32_t dur5 = systick_timer_stop();

    systick_timer_start();
    delay_us(127);
    volatile uint32_t dur6 = systick_timer_stop();

    systick_timer_start();
    delay_us(6234567);
    volatile uint32_t dur7 = systick_timer_stop();

    systick_timer_start();
    delay_us(3);
    volatile uint32_t dur8 = systick_timer_stop();

    systick_timer_start();
    delay_us(12345678);
    volatile uint32_t dur9 = systick_timer_stop();

    static volatile int i;
    ++i;
    while (1)
    {
    }
}


static void systick_test()
{
    SCB_EnableDWT(true);
    Debug_EnableCYCCNT(true);

    systick_enable(true);
    micros();

    uint32_t us0 = micros();
    delay_us(7);
    volatile uint32_t dur = micros() - us0;
    us0 = micros();
    delay_us(7);
    volatile uint32_t dur2 = micros() - us0;
    us0 = micros();
    delay_us(27463);
    volatile uint32_t dur3 = micros() - us0;
    us0 = micros();
    delay_us(1275678);
    volatile uint32_t dur4 = micros() - us0;
    us0 = micros();
    delay_us(7);
    volatile uint32_t dur5 = micros() - us0;

    us0 = micros();
    delay_us(12000000);
    volatile uint32_t dur6 = micros() - us0;

    us0 = micros();
    delay_us(123);
    volatile uint32_t dur7 = micros() - us0;

    us0 = micros();
    delay_us(321);
    volatile uint32_t dur8 = micros() - us0;

    static volatile int i;
    ++i;
    while (1)
    {
    }
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


static int init_mpu2()
{
    if (mpu_init())
    {
        return -1;
    }

    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

    dmp_load_motion_driver_firmware();
    uint16_t dmp_features = DMP_FEATURE_6X_LP_QUAT |
       DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO;
       //DMP_FEATURE_TAP |
//     DMP_FEATURE_GYRO_CAL;

    dmp_enable_feature (dmp_features);
    dmp_set_fifo_rate(100);
    mpu_set_dmp_state(1);

}
static int init_mpu()
{
    if (mpu_init())
    {
        return -1;
    }

    //this is done in init
    //mpu_set_gyro_fsr (2000);
    //mpu_set_accel_fsr(2);
    //mpu_set_lpf(42)
    //mpu_set_sample_rate(50)

    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

    //not needed if dsp is on
//  if (mpu_set_sample_rate(100))
//  {
//      return;
//  }

    mpu_set_compass_sample_rate(25);


//  if (mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
//  {
//      return;
//  }


    dmp_load_motion_driver_firmware();

    //dmp_set_orientation(gyro_orients[orientation]);

    uint16_t dmp_features = DMP_FEATURE_6X_LP_QUAT |
       DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO;
       //DMP_FEATURE_TAP |
//     DMP_FEATURE_GYRO_CAL;

    dmp_enable_feature (dmp_features);

    dmp_set_fifo_rate(100);

    //dmp_set_interrupt_mode (DMP_INT_CONTINUOUS);

    //also enables DMP interrupt
    mpu_set_dmp_state(1);

    //volatile long gyrob = 0, accelb = 0;
    //volatile int rc = mpu_run_self_test((long*)&gyrob, (long*)&accelb);

}

static void test_3rd_party()
{
    Debug_EnableCYCCNT(true);
    systick_enable(true);
    uint32_t m0 = millis();
    //delay_us(3000000);
    volatile uint32_t dur = millis() - m0;


    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_16);
    Adafruit_PCD8544 lcd = get_lcd(spi);
    lcd.write ("3rd party test");
    lcd.display();


    uint32_t i2cClockSpeed = 100000;
    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    i2c->SetErrorCallback (I2C_Error, i2c);
    setI2C(i2c);
    delay(500);
    basic_AHRS(&lcd);
}

static void test_3rd_party2()
{
    Debug_EnableCYCCNT(true);
    systick_enable(true);

    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_16);
    Adafruit_PCD8544 lcd = get_lcd(spi);
    lcd.write ("3rd party test2");
    lcd.display();

    uint32_t i2cClockSpeed = 100000;
    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    i2c->SetErrorCallback (I2C_Error, i2c);
    i2c_init(i2c);

    setI2C(i2c);
    delay(500);
    init_mpu2();
    basic_AHRS2(&lcd);
}

static uint8_t dmpGetGravity(VectorFloat *v, Quaternion *q) {
    v->x = 2 * (q->x * q->z - q->w * q->y);
    v->y = 2 * (q->w * q->x + q->y * q->z);
    v->z = q->w * q->w - q->x * q->x - q->y * q->y + q->z * q->z;
    return 0;
}

static uint8_t dmpGetEuler(float *data, Quaternion *q) {
    data[0] = atan2(2 * q->x * q->y - 2 * q->w * q->z,
            2 * q->w * q->w + 2 * q->x * q->x - 1);   // psi
    data[1] = -asin(2 * q->x * q->z + 2 * q->w * q->y);                 // theta
    data[2] = atan2(2 * q->y * q->z - 2 * q->w * q->x,
            2 * q->w * q->w + 2 * q->z * q->z - 1);   // phi
    return 0;
}

static int8_t dmpGetYawPitchRoll(float *data, Quaternion *q,
        VectorFloat *gravity) {
    // yaw: (about Z axis)
    data[0] = atan2(2 * q->x * q->y - 2 * q->w * q->z,
            2 * q->w * q->w + 2 * q->x * q->x - 1);
    // pitch: (nose up/down, about Y axis)
    data[1] = atan(
            gravity->x
                    / sqrt(gravity->y * gravity->y + gravity->z * gravity->z));
    // roll: (tilt left/right, about X axis)
    data[2] = atan(
            gravity->y
                    / sqrt(gravity->x * gravity->x + gravity->z * gravity->z));
    return 0;
}


static void test_invensense()
{
    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_16);
    Adafruit_PCD8544 lcd = get_lcd(spi);
    lcd.clearDisplay();
    lcd.write ("test inv");
    lcd.display();

    uint32_t i2cClockSpeed = 100000;
    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    i2c->SetErrorCallback (I2C_Error, i2c);

    i2c_init(i2c);
    uint8_t d = 0x80;

    volatile int rc2 = 3;

    rc2 = i2c_read(0x68, 0x6b, 1, &d);

    init_mpu2();

    int rc;
    short gyro[3], a[3], c[3], sensors;
    unsigned char more;
    long  q[4];
    uint32_t ts;
    char buf[64];
    int16_t int_status = 0;
    uint32_t i = 0;
    uint32_t err = 0, good = 0, empty=0;
    VectorFloat gravity;
    float ypr[3];
    int us = 100;
    bool set = false;

    while(1)
    {
        mpu_get_int_status(&int_status);
        rc = dmp_read_fifo (gyro, a, q, &ts, &sensors, &more);
        if (rc == 0)
        {
            ++good;
            if (sensors & INV_WXYZ_QUAT)
            {
                Quaternion qq;
                qq.w = (float) q[0] / 16384.0f;
                qq.x = (float) q[1] / 16384.0f;
                qq.y = (float) q[2] / 16384.0f;
                qq.z = (float) q[3] / 16384.0f;

                dmpGetGravity(&gravity, &qq);
                dmpGetYawPitchRoll(ypr, &qq, &gravity);

                ypr[0] *= 180. / M_PI;
                ypr[1] *= 180. / M_PI;
                ypr[2] *= 180. / M_PI;

                if (++i % 10 == 0)
                {
                    lcd.clearDisplay();
                    lcd.printf(1, 0, 0, "%6.2f", ypr[0]);
                    lcd.printf(1, 0, 1, "%6.2f", ypr[1]);
                    lcd.printf(1, 0, 2, "%6.2f", ypr[2]);

//                  lcd.write_int(0, 0, q[0]);                                                , 
//                  lcd.write_int(0, 1, q[1]);
//                  lcd.write_int(0, 2, q[2]);
//                  lcd.write_int(0, 3, q[3]);
                    lcd.printf(0, 4, "%5d", us);
                    lcd.printf(0, 5, "%2d", good);
                    lcd.printf(7, 5, "%2d", empty);
                    lcd.printf(11, 5, "%2d", err);
                    lcd.display();
                }
            }
        }
        else if (rc == -2 || rc == -1)
        {
            ++err;
        }
        else if (rc == -3)
        {
            ++empty;
        }

        if (!set)
        {
            if (empty == 10)
            {
                empty = 0;
                us += 5;
            }

            if (err == 1)
            {
                //delay_us -= 10;
                err = 0;
                us -= 5;
                set = true;
            }
        }

        delay_us(us);



        //         if (!mpu_get_compass_reg (c, 0) )
//         {
//              float heading = atan2(c[1], c[0]);
//              float headingDegrees = heading * 180/M_PI;
//              headingDegrees -= 4.39;
////              if(headingDegrees < 0)
////              {
////                  headingDegrees += 360.;
////              }
////              if (headingDegrees > 360)
////              {
////                  headingDegrees -= 360;
////              }
//
//              lcd.write_float(0, 0, headingDegrees, 2);
////              fix16_to_str(fix16_from_int(compass[1]), buf, 0);
////              lcd.write(5, 0, buf);
////              fix16_to_str(fix16_from_int(compass[2]), buf, 0);
////              lcd.write(10, 0, buf);
//
//              lcd.write_int(0, 1, a[0]);
//              lcd.write_int(5, 1, a[1]);
//              lcd.write_int(10, 1, a[2]);
//              more==0 ? lcd.write(0, 4, "no more") : lcd.write(0, 4, "more   ");
//              lcd.display();
//         }


    }

}


int main()
{
    RCC_EnableHSI_168Mhz();
    //Debug_EnableCYCCNT(TRUE);
    //test_clock();
    test_invensense();
    //systick_test();
    //test_3rd_party();
    while(1);
    return 0;
}
