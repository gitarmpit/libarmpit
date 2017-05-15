#include "bmp085.h"
#include "i2c_cpp.h"


Bmp180::Bmp180(I2C* i2c) : Baro_drv (i2c)
{
    _i2c = i2c;
    param_b5 = 0;
    ml_version = 0;
    al_version = 0;
    chip_id = 0;

}

void Bmp180::get_calib_param()
{
    uint8_t cal[22];
    _i2c->MasterRead(BMP180_I2C_ADDR, BMP180_PROM_START__ADDR, BMP180_PROM_DATA__LEN, cal);
    calib_param.ac1 = ((uint16_t)cal[0] << 8) | cal[1];
    calib_param.ac2 = ((uint16_t)cal[2] << 8) | cal[3];
    calib_param.ac3 = ((uint16_t)cal[4] << 8) | cal[5];
    calib_param.ac4 = ((uint16_t)cal[6] << 8) | cal[7];
    calib_param.ac5 = ((uint16_t)cal[8] << 8) | cal[9];
    calib_param.ac6 = ((uint16_t)cal[10] << 8) | cal[11];
    calib_param.b1 = ((uint16_t)cal[12] << 8) | cal[13];
    calib_param.b2 = ((uint16_t)cal[14] << 8) | cal[15];
    calib_param.mb = ((uint16_t)cal[16] << 8) | cal[17];
    calib_param.mc = ((uint16_t)cal[18] << 8) | cal[19];
    calib_param.md = ((uint16_t)cal[20] << 8) | cal[21];
}

bool Bmp180::detect()
{
    uint8_t v = 0;

    bool rc = _i2c->MasterRead(BMP180_I2C_ADDR, BMP180_CHIP_ID__REG, 1, &v);
    if (rc)
    {
        chip_id = BMP180_GET_BITSLICE(v, BMP180_CHIP_ID);
        oversamp_setting = 3;

        rc += _i2c->MasterRead(BMP180_I2C_ADDR, BMP180_VERSION_REG, 1, &v);
        /* read Version reg */

        ml_version = BMP180_GET_BITSLICE(v, BMP180_ML_VERSION);/* get ML Version */
        al_version = BMP180_GET_BITSLICE(v, BMP180_AL_VERSION); /* get AL Version */

        get_calib_param();
    }

    return rc;
}

void Bmp180::temp_start()
{
    _i2c->MasterWrite(BMP180_I2C_ADDR, BMP180_CTRL_MEAS_REG, BMP180_T_MEASURE);
}

//5 msec delay
uint16_t Bmp180::temp_read_raw(void)
{
    uint8_t t[2];
    _i2c->MasterRead(BMP180_I2C_ADDR, BMP180_ADC_OUT_MSB_REG, 2, t);
    return (uint16_t)(t[0] << 8) | (t[1]);
}



int32_t Bmp180::temp_calc(uint16_t raw_tmp)
{
    int32_t x1 = (((int32_t) raw_tmp - (int32_t) calib_param.ac6) * (int32_t) calib_param.ac5) >> 15;

    if (x1 == 0 && calib_param.md == 0)
        return 0;

    /* executed only the divisor is not zero*/
    int32_t x2 = ((int32_t) calib_param.mc << 11) / (x1 + calib_param.md);

    param_b5 = x1 + x2;

    return ((param_b5 + 8) >> 4);
}




void Bmp180::pressure_start(void)
{
    _i2c->MasterWrite(BMP180_I2C_ADDR, BMP180_CTRL_MEAS_REG, BMP180_P_MEASURE + (oversamp_setting << 6));
}

uint32_t Bmp180::pressure_read_raw(void)
{
    uint8_t p[3];

    _i2c->MasterRead(BMP180_I2C_ADDR, BMP180_ADC_OUT_MSB_REG, 3, p);

    return  ( ((uint32_t) p[0] << 16) | ((uint32_t) p[1] << 8) | (uint32_t) p[2] ) >> (8 - oversamp_setting);

}


int32_t Bmp180::pressure_calc(uint32_t raw_pressure)
{
    int32_t b6 = param_b5 - 4000;
    int32_t x1 = (((b6*b6)  >> 12) * calib_param.b2) >> 11;
    int32_t x2 = (calib_param.ac2*b6) >> 11;
    int32_t x3 = x1 + x2;
    int32_t b3 = (((((int32_t)calib_param.ac1)*4 + x3) <<  oversamp_setting) + 2) >> 2;

    x1 = (calib_param.ac3 * b6) >> 13;
    x2 = (calib_param.b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    int32_t b4 = (calib_param.ac4 * (uint32_t)(x3 + 32768)) >> 15;

    uint32_t b7 = ((uint32_t)(raw_pressure - b3) * (50000 >> oversamp_setting));

    int32_t res = 0;
    if (b7 < 0x80000000)
    {
        if (b4 != 0)
            res = (b7 << 1) / b4;
         else
            return 0;
    } else
    {
        if (b4 != 0)
            res = (b7 / b4) << 1;
        else
            return 0;
    }

    x1 = res >> 8;
    x1 *= x1;
    x1 = (x1 * BMP180_PARAM_MG)  >> 16;
    x2 = (res * BMP180_PARAM_MH) >> 16;

    /*pressure in Pa*/
    res += (x1 + x2 + BMP180_PARAM_MI) >> 4;

    return res;
}

void Bmp180::read (int32_t* pressure, int32_t* temp)
{
    *temp = temp_calc(temp_read_raw());
    *pressure = pressure_calc(pressure_read_raw());

    pressure_start();
    temp_start();


}


