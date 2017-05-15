#ifndef _MPU9250_H
#define _MPU9250_H

#include "regs.h"
#include "_pins.h"
#include "GPIO_Helper.h"
#include "i2c_cpp.h"
#include "9163/TFT_ILI9163C.h"
#include "system_time.h"
#include <stdio.h>
#include <math.h>



#define delay delay


class MPU9250_Drv
{
private:
    I2C* _i2c;

    enum Ascale
    {
        AFS_2G = 0, AFS_4G, AFS_8G, AFS_16G
    };

    enum Gscale
    {
        GFS_250DPS = 0, GFS_500DPS, GFS_1000DPS, GFS_2000DPS
    };

    enum Mscale
    {
        MFS_14BITS = 0, // 0.6 mG per LSB
        MFS_16BITS      // 0.15 mG per LSB
    };

    // Specify sensor full scale
    uint8_t Gscale = GFS_250DPS;
    uint8_t Ascale = AFS_2G;
    uint8_t Mscale = MFS_16BITS; // Choose either 14-bit or 16-bit magnetometer resolution
    uint8_t Mmode = 0x02; // 2 for 8 Hz, 6 for 100 Hz continuous magnetometer data read
    float aRes, gRes, mRes;      // scale resolutions per LSB for the sensors


//
    float magGain[3] =   { 0, 0, 0 };
    float magbias[3] =  { 0, 0, 0 };
//    float gyroBias[3] =     { 0, 0, 0 };
//    float accelBias[3] =    { 0, 0, 0 };
//
//    int16_t tempCount;      // temperature raw count output
//    float temperature; // Stores the real internal chip temperature in degrees Celsius
//    float SelfTest[6];    // holds results of gyro and accelerometer self test

    void writeByte(uint8_t slave_address, uint8_t reg_address, uint8_t data);
    uint8_t readByte(uint8_t slave_address, uint8_t reg_address);
    void readBytes(uint8_t slave_address, uint8_t reg_address, uint8_t count, uint8_t *data);


public:
    MPU9250_Drv (I2C* i2c);

    void initMPU9250();
    void initAK8963();
    void calibrateMPU9250(float * dest1_gyro, float * dest2_acc);
    void MPU9250SelfTest(float * destination);

    int16_t readTempData();
    void    readMagData(int16_t * destination);
    void    readGyroData(int16_t * destination);
    void    readAccelData(int16_t * destination);


    bool isDataReady();

    void read(float* a, float* g, float* m);


    void loop();

};


#endif
