#include "MPU9250.h"
#include "GPIO_Helper.h"
#include "_pins.h"
#include "_func.h"
#include "AHRS.h"
#include "debug.h"

void kwiner_raw()
{
    TFT_ILI9163C* lcd = get_lcd();

    lcd->setBgColor(0);
    lcd->setTextColor(YELLOW);

    uint32_t i2cClockSpeed = 100000;
    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    MPU9250_Drv mpu(i2c);
    mpu.initMPU9250();
    //float a_bias, g_bias;
    //mpu.calibrateMPU9250(&g_bias, &a_bias);
    mpu.initAK8963();

    int16_t a[3], g[3];

    while(1)
    {
        lcd->clearScreen();
        mpu.readAccelData(a);
        lcd->printf (0, 0, "%d %d %d", a[0], a[1], a[2]);
        mpu.readGyroData(g);
        lcd->printf (0, 1, "%d %d %d", g[0], g[1], g[2]);
        lcd->display();
        delay(100);

    }
}


void kwiner_ahrs_loop()
{
    TFT_ILI9163C* lcd = get_lcd();
    lcd->setBgColor(0);
    lcd->setTextColor(YELLOW);

    systick_enable(true);
    uint32_t i2cClockSpeed = 100000;
    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    MPU9250_Drv mpu(i2c);
    mpu.initMPU9250();
    //float a_bias, g_bias;
    //mpu.calibrateMPU9250(&g_bias, &a_bias);
    mpu.initAK8963();

    uint32_t display_rate_ms = 500;

    AHRS ahrs;

    float a[3] =    { 0, 0, 0 };
    float g[3] =    { 0, 0, 0 };
    float m[3] =    { 0, 0, 0 };

    // Sensors x (y)-axis of the acc is aligned with the y (x)-axis of the mag;
    // the magnetometer z-axis (+ down) is opposite to z-axis (+ up) of accelerometer and gyro!
    // For the MPU-9250, we have chosen a magnetic rotation that keeps the sensor forward along the x-axis just like
    // in the LSM9DS0 sensor. This rotation can be modified to allow any convenient orientation convention.


    float deltat = 0.0f;
    uint32_t now = 0, lastUpdate = 0;
    uint32_t total_ms = 0, sumCount = 0;
    uint32_t delt_t = 0, lastDisplayUpdate = 0;

    while (1)
    {
        if (mpu.isDataReady())  // If intPin goes high, all data registers have new data
        {  // On interrupt, check if data ready interrupt
            mpu.read(a, g, m);
        }

        now = micros();
        deltat = ((now - lastUpdate) / 1000000.0f);
        lastUpdate = now;

        total_ms += deltat;
        sumCount++;

        ahrs.mahonyQuaternionUpdate(deltat, a, g, m);

        delt_t = millis() - lastDisplayUpdate;

        if (delt_t > display_rate_ms)
        {
            float y, p, r;
            ahrs.getEuler(&y, &p, &r);


            //  Magwich ~145 Hz rate, Mahony > 200Hz, on-board DMP = 200Hz
            float rateHz = (float) sumCount / total_ms;

            lcd->printf (0, 0, "y: %4.2f", y);
            lcd->printf (0, 1, "p: %4.2f", p);
            lcd->printf (0, 2, "r: %4.2f", r);
            lcd->printf (0, 5, "rate: %4.2f", rateHz);


            lastDisplayUpdate = millis();
            sumCount = 0;
            total_ms = 0;
        }
    }

}
