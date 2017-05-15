#include "i2c_cpp.h"
#include "sensors/sensors.h"
#include "compass_ak8963.h"
#include "debug.h"

Ak8963::Ak8963(I2C*  i2c) : Compass_drv(i2c)
{
}



bool Ak8963::detect()
{
    bool ack = false;
    uint8_t sig = 0;

    // check for AK8963
    ack = _i2c->MasterRead(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_WHO_AM_I, 1, &sig);
    if (ack && sig == AK8963_Device_ID) // 0x48 / 01001000 / 'H'
    {
        return true;
    }
    return false;
}

void Ak8963::init()
{
    bool ack;
    uint8_t calibration[3];
    uint8_t status;

    ack = _i2c->MasterWrite(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_CNTL, CNTL_MODE_POWER_DOWN); // power down before entering fuse mode
    delay(20);

    ack = _i2c->MasterWrite(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_CNTL, CNTL_MODE_FUSE_ROM); // Enter Fuse ROM access mode
    delay(10);

    ack = _i2c->MasterRead(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_ASAX, sizeof(calibration), calibration); // Read the x-, y-, and z-axis calibration values
    delay(10);

    //TODO why multiply by 30?
    magGain[0] = (((((float)(int8_t)calibration[0] - 128) / 256) + 1) * 30);
    magGain[1] = (((((float)(int8_t)calibration[1] - 128) / 256) + 1) * 30);
    magGain[2] = (((((float)(int8_t)calibration[2] - 128) / 256) + 1) * 30);

    ack = _i2c->MasterWrite(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_CNTL, CNTL_MODE_POWER_DOWN); // power down after reading.
    delay(10);

    // Clear status registers
    ack = _i2c->MasterRead(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_STATUS1, 1, &status);
    ack = _i2c->MasterRead(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_STATUS2, 1, &status);

    // Trigger first measurement  (single measurement mode)
    ack = _i2c->MasterWrite(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_CNTL, CNTL_MODE_ONCE);
}

bool Ak8963::read(int16_t *magData)
{
    bool ack = false;
    uint8_t buf[7];

    ack = _i2c->MasterRead(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_STATUS1, 1, &buf[0]);

    uint8_t status = buf[0];

    if (!ack || (status & STATUS1_DATA_READY) == 0) {
        return false;
    }

    ack = _i2c->MasterRead(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_HXL, 7, &buf[0]);
    uint8_t status2 = buf[6];
    if (!ack || (status2 & STATUS2_DATA_ERROR) || (status2 & STATUS2_MAG_SENSOR_OVERFLOW)) {
        return false;
    }

    magData[0] = (int16_t)(buf[1] << 8 | buf[0]) * magGain[0];
    magData[1] = (int16_t)(buf[3] << 8 | buf[2]) * magGain[1];
    magData[2] = (int16_t)(buf[5] << 8 | buf[4]) * magGain[2];

    return _i2c->MasterWrite(AK8963_MAG_I2C_ADDRESS, AK8963_MAG_REG_CNTL, CNTL_MODE_ONCE); // start reading again
}
