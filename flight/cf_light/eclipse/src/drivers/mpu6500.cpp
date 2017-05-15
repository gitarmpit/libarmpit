#include "mpu6500.h"
#include "i2c_cpp.h"
#include "maths.h"
#include "filter.h"
#include "sensors.h"
#include "debug.h"

//TODO
static volatile bool mpuDataReady;


MPU::MPU(I2C* i2c)
{
	_i2c = i2c;
    //i2c->SetErrorCallback (I2C_Error, i2c);
   _mpuIntDenominator = 0;

}

bool MPU::Detect()
{
    bool rc = false;
    uint8_t sig;

    // MPU datasheet specifies 30ms.
    delay(35);

    bool ack = _i2c->MasterRead(MPU_ADDRESS, MPU_RA_WHO_AM_I, 1, &sig);

    if (ack && ((sig & MPU_INQUIRY_MASK) == MPU6500_WHO_AM_I_CONST)) 
    {
        rc = true;
    }
    return rc;
}

void MPU::Init(uint16_t* sampleFreq, uint8_t  lpf)
{
    uint16_t gyroOutputRate;

    //Gyroscope Output Rate = 8kHz when the DLPF is disabled (DLPF_CFG = 0 or 7),
    //                    and 1kHz when the DLPF is enabled

    switch(lpf)
    {
        case 0:
            gyroOutputRate = 8000;
        break;
        default:
            gyroOutputRate = 1000;
        break;
    }

    //default: lpf = 0 (256Hz) so Gyro out rate = 8K
    //sample Freq = 4K,  denominator = 2 (means process 8K int every other time)
    _mpuIntDenominator = gyroOutputRate / *sampleFreq;
    // handle cases where the refresh period was set higher than the LPF allows
    if (_mpuIntDenominator == 0) {
        _mpuIntDenominator = 1;
        *sampleFreq = gyroOutputRate;
    }

    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_PWR_MGMT_1, MPU6500_BIT_RESET);
    delay(100);
    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_SIGNAL_PATH_RESET, 0x07);
    delay(100);
    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_PWR_MGMT_1, 0);
    delay(100);
    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_PWR_MGMT_1, INV_CLK_PLL);
    delay(15);
    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_GYRO_CONFIG, INV_FSR_2000DPS << 3);
    delay(15);
    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_ACCEL_CONFIG, INV_FSR_8G << 3);
    delay(15);

    //Bandwidth  0=256Hz,
    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_CONFIG, lpf);
    delay(15);
    //Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_SMPLRT_DIV, 0);

    // Data ready interrupt configuration
//#ifdef USE_MPU9250_MAG
    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_INT_PIN_CFG, 0 << 7 | 0 << 6 | 0 << 5 | 1 << 4 | 0 << 3 | 0 << 2 | 1 << 1 | 0 << 0);  // INT_ANYRD_2CLEAR, BYPASS_EN
//#else
//    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_INT_PIN_CFG, 0 << 7 | 0 << 6 | 0 << 5 | 1 << 4 | 0 << 3 | 0 << 2 | 0 << 1 | 0 << 0);  // INT_ANYRD_2CLEAR, BYPASS_EN
//#endif

//    #ifdef USE_MPU_DATA_READY_SIGNAL
    _i2c->MasterWrite(MPU_ADDRESS, MPU_RA_INT_ENABLE, 0x01); // RAW_RDY_EN interrupt enable
//#endif

}


void MPU_DATA_READY_EXTI_Handler(void)
{
    mpuDataReady = true;
}


bool MPU::AccRead(int32_t *accADC)
{
    uint8_t data[6];

    bool ack = _i2c->MasterRead(MPU_ADDRESS, MPU_RA_ACCEL_XOUT_H, 6, data);
    if (!ack)
    {
        return false;
    }

    accADC[0] = (int16_t)((data[0] << 8) | data[1]);
    accADC[1] = (int16_t)((data[2] << 8) | data[3]);
    accADC[2] = (int16_t)((data[4] << 8) | data[5]);

    return true;
}

bool MPU::GyroRead(int32_t *gyroADC)
{
    uint8_t data[6];

    bool ack = _i2c->MasterRead(MPU_ADDRESS, MPU_RA_GYRO_XOUT_H, 6, data);
    if (!ack)
    {
        return false;
    }

    gyroADC[0] = (int16_t)((data[0] << 8) | data[1]);
    gyroADC[1] = (int16_t)((data[2] << 8) | data[3]);
    gyroADC[2] = (int16_t)((data[4] << 8) | data[5]);

    return true;
}

//TODO
bool MPU::IsDataReady(void)
{
    if (mpuDataReady) {
        mpuDataReady = false;
        return true;
    }

    return false;
}







