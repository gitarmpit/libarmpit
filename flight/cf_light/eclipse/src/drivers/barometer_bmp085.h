#ifndef _BMP_085
#define _BMP_085

#define BMP085_I2C_ADDR         0x77
#define BMP085_CHIP_ID          0x55
#define BOSCH_PRESSURE_BMP085   85
#define BMP085_CHIP_ID_REG      0xD0
#define BMP085_VERSION_REG      0xD1
#define E_SENSOR_NOT_DETECTED   (char) 0
#define BMP085_PROM_START__ADDR 0xaa
#define BMP085_PROM_DATA__LEN   22
#define BMP085_T_MEASURE        0x2E                // temperature measurent
#define BMP085_P_MEASURE        0x34                // pressure measurement
#define BMP085_CTRL_MEAS_REG    0xF4
#define BMP085_ADC_OUT_MSB_REG  0xF6
#define BMP085_ADC_OUT_LSB_REG  0xF7
#define BMP085_CHIP_ID__POS     0
#define BMP085_CHIP_ID__MSK     0xFF
#define BMP085_CHIP_ID__LEN     8
#define BMP085_CHIP_ID__REG     BMP085_CHIP_ID_REG

#define BMP085_ML_VERSION__POS      0
#define BMP085_ML_VERSION__LEN      4
#define BMP085_ML_VERSION__MSK      0x0F
#define BMP085_ML_VERSION__REG      BMP085_VERSION_REG

#define BMP085_AL_VERSION__POS      4
#define BMP085_AL_VERSION__LEN      4
#define BMP085_AL_VERSION__MSK      0xF0
#define BMP085_AL_VERSION__REG      BMP085_VERSION_REG

#define BMP085_GET_BITSLICE(regvar, bitname) (regvar & bitname##__MSK) >> bitname##__POS
#define BMP085_SET_BITSLICE(regvar, bitname, val) (regvar & ~bitname##__MSK) | ((val<<bitname##__POS)&bitname##__MSK)

#define SMD500_PARAM_MG      3038        //calibration parameter
#define SMD500_PARAM_MH     -7357        //calibration parameter
#define SMD500_PARAM_MI      3791        //calibration parameter


#define UT_DELAY    6000        // 1.5ms margin according to the spec (4.5ms T conversion time)
#define UP_DELAY    27000       // 6000+21000=27000 1.5ms margin according to the spec (25.5ms P conversion time with OSS=3)



#ifdef BARO_XCLR_PIN
#define BMP085_OFF                  digitalLo(BARO_XCLR_GPIO, BARO_XCLR_PIN);
#define BMP085_ON                   digitalHi(BARO_XCLR_GPIO, BARO_XCLR_PIN);
#else
#define BMP085_OFF
#define BMP085_ON
#endif

#include "baro_drv.h"

class Bmp085 : public Baro_drv
{
private:

    struct bmp085_smd500_calibration_param_t
    {
        int16_t ac1;
        int16_t ac2;
        int16_t ac3;
        uint16_t ac4;
        uint16_t ac5;
        uint16_t ac6;
        int16_t b1;
        int16_t b2;
        int16_t mb;
        int16_t mc;
        int16_t md;
    };

    bmp085_smd500_calibration_param_t cal_param;
    uint8_t chip_id, ml_version, al_version;
    int32_t param_b5;
    int16_t oversampling_setting;

    bool inited = false;

    void get_cal_param(void);

    void     temp_start(void);
    uint16_t temp_read_raw(void);
    int32_t  temp_calc(uint16_t raw_tmp);

    void     pressure_start(void);
    uint32_t pressure_read_raw(void);
    int32_t  pressure_calc(uint32_t raw_pressure);

protected:

    virtual bool detect(void);

public:
    Bmp085(I2C* i2c);

    //25ms or 40HZ
    int16_t get_delay_ms() { return 2 + (3 << oversampling_setting); }

    virtual void read (int32_t* pressure, int32_t* temp);
};



#endif

