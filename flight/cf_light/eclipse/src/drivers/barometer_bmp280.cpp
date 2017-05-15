#include "barometer_bmp280.h"
//#include "platform.h"
#include "debug.h"


// BMP280, address 0x76


Bmp280::Bmp280(I2C* i2c) : Baro_drv (i2c)
{
    bmp280_chip_id = 0;
    bmp280InitDone = false;
    // uncompensated pressure and temperature
    raw_pressure = 0;
    raw_temp = 0;
}

bool Bmp280::detect()
{
    if (bmp280InitDone)
        return true;

    delay(20);

    _i2c->MasterRead(BMP280_I2C_ADDR, BMP280_CHIP_ID_REG, 1, &bmp280_chip_id);

    if (bmp280_chip_id != BMP280_DEFAULT_CHIP_ID)
        return false;

    // read calibration
    _i2c->MasterRead(BMP280_I2C_ADDR, BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG, 24, (uint8_t *)&bmp280_cal);
    // set oversampling + power mode (forced), and start sampling
    _i2c->MasterWrite(BMP280_I2C_ADDR, BMP280_CTRL_MEAS_REG, BMP280_MODE);

    bmp280InitDone = true;

    return true;
}


void Bmp280::trigger_measurement(void)
{
    // start measurement
    // set oversampling + power mode (forced), and start sampling
    uint8_t mode = BMP280_OVERSAMP_8X << 2 | BMP280_OVERSAMP_1X << 5 | BMP280_FORCED_MODE;
    _i2c->MasterWrite(BMP280_I2C_ADDR, BMP280_CTRL_MEAS_REG, mode);
}



void Bmp280::get_raw(void)
{
    uint8_t data[BMP280_DATA_FRAME_SIZE];

    // read data from sensor
    _i2c->MasterRead(BMP280_I2C_ADDR, BMP280_PRESSURE_MSB_REG, BMP280_DATA_FRAME_SIZE, data);

    raw_pressure = (int32_t)((((uint32_t)(data[0])) << 12) | (((uint32_t)(data[1])) << 4) | ((uint32_t)data[2] >> 4));
    raw_temp = (int32_t)((((uint32_t)(data[3])) << 12) | (((uint32_t)(data[4])) << 4) | ((uint32_t)data[5] >> 4));
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of "5123" equals 51.23 DegC
// t_fine carries fine temperature as global value
int32_t Bmp280::compensate_temp(int32_t adc_T)
{
    int32_t var1, var2, T;

    var1 = ((((adc_T >> 3) - ((int32_t)bmp280_cal.dig_T1 << 1))) * ((int32_t)bmp280_cal.dig_T2)) >> 11;
    var2  = (((((adc_T >> 4) - ((int32_t)bmp280_cal.dig_T1)) * ((adc_T >> 4) - ((int32_t)bmp280_cal.dig_T1))) >> 12) * ((int32_t)bmp280_cal.dig_T3)) >> 14;
    bmp280_cal.t_fine = var1 + var2;
    T = (bmp280_cal.t_fine * 5 + 128) >> 8;

    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa
uint32_t Bmp280::compensate_pressure(int32_t adc_P)
{
    int64_t var1, var2, p;
    var1 = ((int64_t)bmp280_cal.t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bmp280_cal.dig_P6;
    var2 = var2 + ((var1*(int64_t)bmp280_cal.dig_P5) << 17);
    var2 = var2 + (((int64_t)bmp280_cal.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)bmp280_cal.dig_P3) >> 8) + ((var1 * (int64_t)bmp280_cal.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)bmp280_cal.dig_P1) >> 33;
    if (var1 == 0)
        return 0;
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)bmp280_cal.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)bmp280_cal.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280_cal.dig_P7) << 4);
    return (uint32_t)p;
}

void Bmp280::read(int32_t *pressure, int32_t *temperature)
{
	trigger_measurement();
	get_raw();

    // calculate
    int32_t t;
    uint32_t p;
    t = compensate_temp(raw_temp);
    p = compensate_pressure(raw_pressure);

    if (pressure)
        *pressure = (int32_t)(p / 256);
    if (temperature)
        *temperature = t;
}

