#ifndef __BMP180_H__
#define __BMP180_H__


#define BMP180_I2C_ADDR         0x77
#define BMP180_CHIP_ID          0x55

//#define   BMP180_INIT_VALUE                     ((uint8_t)0)
//#define   BMP180_INITIALIZE_OVERSAMP_SETTING_U8X    ((uint8_t)0)
//#define   BMP180_INITIALIZE_SW_OVERSAMP_U8X         ((uint8_t)0)
//#define   BMP180_INITIALIZE_NUMBER_OF_SAMPLES_U8X   ((uint8_t)1)
//#define   BMP180_GEN_READ_WRITE_DATA_LENGTH         ((uint8_t)1)
//#define   BMP180_TEMPERATURE_DATA_LENGTH            ((uint8_t)2)
//#define   BMP180_PRESSURE_DATA_LENGTH               ((uint8_t)3)
//#define   BMP180_SW_OVERSAMP_U8X                    ((uint8_t)1)
//#define   BMP180_OVERSAMP_SETTING_U8X               ((uint8_t)3)
//#define   BMP180_2MS_DELAY_U8X          (2)
//#define   BMP180_3MS_DELAY_U8X          (3)
//#define   BMP180_AVERAGE_U8X            (3)
//#define   BMP180_INVALID_DATA           (0)
//#define   BMP180_CHECK_DIVISOR          (0)
//#define   BMP180_DATA_MEASURE           (3)
//#define   BMP180_CALCULATE_TRUE_PRESSURE        (8)
//#define   BMP180_CALCULATE_TRUE_TEMPERATURE     (8)

#define BMP180_PROM_START__ADDR     (0xAA)
#define BMP180_PROM_DATA__LEN       (22)

#define BMP180_CHIP_ID_REG          (0xD0)
#define BMP180_VERSION_REG          (0xD1)

#define BMP180_CTRL_MEAS_REG        (0xF4)
#define BMP180_ADC_OUT_MSB_REG      (0xF6)
#define BMP180_ADC_OUT_LSB_REG      (0xF7)

#define BMP180_SOFT_RESET_REG       (0xE0)

#define BMP180_T_MEASURE            (0x2E)
#define BMP180_P_MEASURE            (0x34)
#define BMP180_TEMP_CONVERSION_TIME  (5)

#define BMP180_PARAM_MG     (3038)
#define BMP180_PARAM_MH     (-7357)
#define BMP180_PARAM_MI     (3791)


#define BMP180_CALIB_DATA_SIZE          (22)


#define BMP180_CHIP_ID__POS             (0)
#define BMP180_CHIP_ID__MSK             (0xFF)
#define BMP180_CHIP_ID__LEN             (8)
#define BMP180_CHIP_ID__REG             (BMP180_CHIP_ID_REG)
#define BMP180_ML_VERSION__POS          (0)
#define BMP180_ML_VERSION__LEN          (4)
#define BMP180_ML_VERSION__MSK          (0x0F)
#define BMP180_ML_VERSION__REG          (BMP180_VERSION_REG)

#define BMP180_AL_VERSION__POS          (4)
#define BMP180_AL_VERSION__LEN          (4)
#define BMP180_AL_VERSION__MSK          (0xF0)
#define BMP180_AL_VERSION__REG          (BMP180_VERSION_REG)

#define BMP180_GET_BITSLICE(regvar, bitname)\
((regvar & bitname##__MSK) >> (bitname##__POS))

#define BMP180_SET_BITSLICE(regvar, bitname, val)\
((regvar & ~bitname##__MSK) | ((val<<bitname##__POS)&bitname##__MSK))

#include <stdint.h>
#include "baro_drv.h"

class I2C;

class Bmp180 : public Baro_drv
{
private:
    struct bmp180_calib_param_t
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

    I2C* _i2c;

    struct bmp180_calib_param_t calib_param;
    uint8_t chip_id;
    uint8_t ml_version;
    uint8_t al_version;
    uint8_t sensortype;

    int32_t param_b5;
    int16_t oversamp_setting;


    //P units 1Pa  (= 0.01hPa  =  0.01mbar)


    void get_calib_param(void);


    void     temp_start(void);
    uint16_t temp_read_raw(void);
    int32_t  temp_calc(uint16_t raw_tmp);

    void     pressure_start(void);
    uint32_t pressure_read_raw(void);
    int32_t  pressure_calc(uint32_t raw_pressure);

    virtual bool detect();

public:
    Bmp180 (I2C* i2c);

    int16_t get_delay_ms() { return 2 + (3 << oversamp_setting); }

    virtual void read (int32_t* pressure, int32_t* temp);


};

#endif
