#include <stdbool.h>
#include <stdint.h>

//#include <platform.h>

#include "barometer_bmp085.h"
#include "debug.h"


Bmp085::Bmp085(I2C* i2c) : Baro_drv (i2c)
{
    chip_id = 0;
    ml_version = 0;
    al_version = 0;
    param_b5 = 0;
    oversampling_setting = 3;
    inited = false;
}


bool Bmp085::detect()
{
    uint8_t data;
    bool ack;

    if (inited)
        return true;

    delay(20); // datasheet says 10ms, we'll be careful and do 20.

    ack = _i2c->MasterRead(BMP085_I2C_ADDR, BMP085_CHIP_ID__REG, 1, &data); /* read Chip Id */
    if (ack)
    {
        chip_id = BMP085_GET_BITSLICE(data, BMP085_CHIP_ID);

        if (chip_id == BMP085_CHIP_ID)
        { /* get bitslice */
            _i2c->MasterRead(BMP085_I2C_ADDR, BMP085_VERSION_REG, 1, &data); /* read Version reg */
            ml_version = BMP085_GET_BITSLICE(data, BMP085_ML_VERSION); /* get ML Version */
            al_version = BMP085_GET_BITSLICE(data, BMP085_AL_VERSION); /* get AL Version */
            get_cal_param(); /* readout bmp085 calibparam structure */
            inited = true;

            pressure_start();

            return true;
        }
    }


    return false;
}

void Bmp085::temp_start(void)
{
    _i2c->MasterWrite(BMP085_I2C_ADDR, BMP085_CTRL_MEAS_REG, BMP085_T_MEASURE);
}

uint16_t Bmp085::temp_read_raw(void)
{
    uint8_t data[2];
    _i2c->MasterRead(BMP085_I2C_ADDR, BMP085_ADC_OUT_MSB_REG, 2, data);
    return (data[0] << 8) | data[1];
}

int32_t Bmp085::temp_calc(uint16_t raw_tmp)
{
    int32_t temperature;
    int32_t x1, x2;

    x1 = (((int32_t) raw_tmp - (int32_t) cal_param.ac6) * (int32_t) cal_param.ac5) >> 15;
    x2 = ((int32_t) cal_param.mc << 11) / (x1 + cal_param.md);
    param_b5 = x1 + x2;
    temperature = ((param_b5 * 10 + 8) >> 4);  // temperature in 0.01 C (make same as MS5611)

    return temperature;
}



int32_t Bmp085::pressure_calc(uint32_t raw_pressure)
{
    int32_t pressure, x1, x2, x3, b3, b6;
    uint32_t b4, b7;

    b6 = param_b5 - 4000;
    // *****calculate B3************
    x1 = (b6 * b6) >> 12;
    x1 *= cal_param.b2;
    x1 >>= 11;

    x2 = (cal_param.ac2 * b6);
    x2 >>= 11;

    x3 = x1 + x2;

    b3 = (((((int32_t) cal_param.ac1) * 4 + x3) << oversampling_setting) + 2) >> 2;

    // *****calculate B4************
    x1 = (cal_param.ac3 * b6) >> 13;
    x2 = (cal_param.b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (cal_param.ac4 * (uint32_t)(x3 + 32768)) >> 15;

    b7 = ((uint32_t)(raw_pressure - b3) * (50000 >> oversampling_setting));
    if (b7 < 0x80000000) {
        pressure = (b7 << 1) / b4;
    } else {
        pressure = (b7 / b4) << 1;
    }

    x1 = pressure >> 8;
    x1 *= x1;
    x1 = (x1 * SMD500_PARAM_MG) >> 16;
    x2 = (pressure * SMD500_PARAM_MH) >> 16;
    pressure += (x1 + x2 + SMD500_PARAM_MI) >> 4;   // pressure in Pa

    return pressure;
}


void Bmp085::pressure_start(void)
{
    uint8_t ctrl_reg_data;

    ctrl_reg_data = BMP085_P_MEASURE + (oversampling_setting << 6);

    _i2c->MasterWrite(BMP085_I2C_ADDR, BMP085_CTRL_MEAS_REG, ctrl_reg_data);
}

/** read out up for pressure conversion
 depending on the oversampling ratio setting up can be 16 to 19 bit
 \return up parameter that represents the uncompensated pressure value
 */
uint32_t Bmp085::pressure_read_raw(void)
{
    uint8_t data[3];

    _i2c->MasterRead(BMP085_I2C_ADDR, BMP085_ADC_OUT_MSB_REG, 3, data);
    return (((uint32_t) data[0] << 16) | ((uint32_t) data[1] << 8) | (uint32_t) data[2])
            >> (8 - oversampling_setting);
}


void Bmp085::read (int32_t* pressure, int32_t* temp)
{
    *temp = temp_calc(temp_read_raw());
    *pressure = pressure_calc(pressure_read_raw());

    pressure_start();
    temp_start();


}


void Bmp085::get_cal_param(void)
{
    uint8_t data[22];
    _i2c->MasterRead(BMP085_I2C_ADDR, BMP085_PROM_START__ADDR, BMP085_PROM_DATA__LEN, data);

    /*parameters AC1-AC6*/
    cal_param.ac1 = (data[0] << 8) | data[1];
    cal_param.ac2 = (data[2] << 8) | data[3];
    cal_param.ac3 = (data[4] << 8) | data[5];
    cal_param.ac4 = (data[6] << 8) | data[7];
    cal_param.ac5 = (data[8] << 8) | data[9];
    cal_param.ac6 = (data[10] << 8) | data[11];

    /*parameters B1,B2*/
    cal_param.b1 = (data[12] << 8) | data[13];
    cal_param.b2 = (data[14] << 8) | data[15];

    /*parameters MB,MC,MD*/
    cal_param.mb = (data[16] << 8) | data[17];
    cal_param.mc = (data[18] << 8) | data[19];
    cal_param.md = (data[20] << 8) | data[21];
}

