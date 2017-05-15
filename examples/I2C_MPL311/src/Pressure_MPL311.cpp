#include "Pressure_MPL311.h"
#include "debug.h"


Pressure_MPL311::Pressure_MPL311(I2C* i2c, bool isAltimeterMode, bool isAutoAquisition, OSR osr)
{
    _i2c = i2c;
    _isAltimeterMode = isAltimeterMode;
    _isAutoAquisition = isAutoAquisition;
    InitializeSensor(isAltimeterMode, isAutoAquisition, osr);
}


void Pressure_MPL311::InitializeSensor(bool isAltimeterMode, bool isAutoAquisition, OSR osr)
{

    uint8_t ctrl1 = CTRL_REG1_RST;
    //Reset
    _i2c->MasterWrite(MPL311_SLAVE_ADDR, CTRL_REG1, ctrl1);
    //delay(10);
    while (1)
    {
        _i2c->MasterRead(MPL311_SLAVE_ADDR, CTRL_REG1, 1, &ctrl1);
        if (!(ctrl1 & CTRL_REG1_RST))
        {
            break;
        }
    }

    if (isAltimeterMode)
    {
        ctrl1 = CTRL_REG1_ALT;
    }
    else
    {
        ctrl1 = 0;
    }

    ctrl1 |= (osr<<3);

    if (isAutoAquisition)
    {
        ctrl1 |= CTRL_REG1_SBYB;
    }

    _i2c->MasterWrite(MPL311_SLAVE_ADDR, CTRL_REG1, ctrl1);

    if (isAutoAquisition)
    {
        uint8_t data_cfg = PT_DATA_CFG_DREM | PT_DATA_CFG_PDEFE | PT_DATA_CFG_TDEFE;
        _i2c->MasterWrite(MPL311_SLAVE_ADDR, PT_DATA_CFG, data_cfg);
    }

}

void Pressure_MPL311::WaitForData(uint8_t status_flag)
{
    uint8_t byte;

    if (!_isAutoAquisition)
    {
        _i2c->MasterRead(MPL311_SLAVE_ADDR, CTRL_REG1, 1, &byte);
        byte |= CTRL_REG1_OST;
        _i2c->MasterWrite(MPL311_SLAVE_ADDR, CTRL_REG1, byte);
    }

    while (1)
    {

        if (_isAutoAquisition)
        {
            _i2c->MasterRead(MPL311_SLAVE_ADDR, DR_STATUS, 1, &byte);
            if (byte & status_flag)
            {
                break;
            }
        }
        else
        {
            _i2c->MasterRead(MPL311_SLAVE_ADDR, CTRL_REG1, 1, &byte);
            if (!(byte & CTRL_REG1_OST))
            {
                break;
            }

        }

    }

}

float Pressure_MPL311::GetPressureAltitude()
{
    WaitForData(DR_STATUS_PDR);

    uint8_t data[3];

    _i2c->MasterRead(MPL311_SLAVE_ADDR, OUT_P_MSB, 3, data);

    float res;

    if (_isAltimeterMode)
    {
        res = (float)(((uint32_t)data[0]<<24) | ((uint32_t)data[1]<<16) | ((uint32_t)data[2]<<8)) / 65536;
    }
    else
    {
        res = (float) (((data[0] << 16) | (data[1] << 8) | (data[2] & 0xC0))
                >> 6) + (float) ((data[2] & 0x30) >> 4) * (float) 0.25;
    }


    //volatile float res2 = (float) ((short) ((data[0] << 8) | data[1])) + (float) (data[2] >> 4) * 0.0625;
    return res;

}

float Pressure_MPL311::GetTemperature()
{
    WaitForData(DR_STATUS_TDR);

    uint8_t data[5];

    _i2c->MasterRead(MPL311_SLAVE_ADDR, OUT_P_MSB, 5, data);
    return (float) ((short)((data[3] << 8) | (data[4] & 0xF0)) >> 4) * 0.0625;
}

uint8_t Pressure_MPL311::GetDeviceID()
{
    static uint8_t byte = 0;
    _i2c->MasterRead(MPL311_SLAVE_ADDR, WHO_AM_I, 1, &byte);
    return byte;

}
