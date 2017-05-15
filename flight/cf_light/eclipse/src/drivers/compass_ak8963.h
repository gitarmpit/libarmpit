#ifndef AK8963_H
#define AK8963_H

#include "compass_drv.h"

// AK8963, mag sensor address
#define AK8963_MAG_I2C_ADDRESS          0x0C
#define AK8963_Device_ID                0x48

// Registers
#define AK8963_MAG_REG_WHO_AM_I         0x00
#define AK8963_MAG_REG_INFO             0x01
#define AK8963_MAG_REG_STATUS1          0x02
#define AK8963_MAG_REG_HXL              0x03
#define AK8963_MAG_REG_HXH              0x04
#define AK8963_MAG_REG_HYL              0x05
#define AK8963_MAG_REG_HYH              0x06
#define AK8963_MAG_REG_HZL              0x07
#define AK8963_MAG_REG_HZH              0x08
#define AK8963_MAG_REG_STATUS2          0x09
#define AK8963_MAG_REG_CNTL             0x0a
#define AK8963_MAG_REG_ASCT             0x0c // self test
#define AK8963_MAG_REG_ASAX             0x10 // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_MAG_REG_ASAY             0x11 // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_MAG_REG_ASAZ             0x12 // Fuse ROM z-axis sensitivity adjustment value

#define READ_FLAG                       0x80

#define STATUS1_DATA_READY              0x01
#define STATUS1_DATA_OVERRUN            0x02

#define STATUS2_DATA_ERROR              0x02
#define STATUS2_MAG_SENSOR_OVERFLOW     0x03

#define CNTL_MODE_POWER_DOWN            0x00
#define CNTL_MODE_ONCE                  0x01
#define CNTL_MODE_CONT1                 0x02
#define CNTL_MODE_CONT2                 0x06
#define CNTL_MODE_SELF_TEST             0x08
#define CNTL_MODE_FUSE_ROM              0x0F


class I2C;

class Ak8963 : public Compass_drv
{

public:
    Ak8963 (I2C*  i2c);

    virtual bool detect();
    virtual void init();
    virtual bool read(int16_t *magData);
};



#endif
