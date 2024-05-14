/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
#pragma once

#include <AP_Common/AP_Common.h>
#include <AP_Math/AP_Math.h>


class AuxiliaryBus;
class AuxiliaryBusSlave;
class AP_InertialSensor;
class Compass;

class AP_Compass_AK8963
{
public:
    AP_Compass_AK8963(Compass* compass);

    bool init();
    void read();

private:

    bool _configure();


    void _make_factory_sensitivity_adjustment(Vector3f &field) const;
    void _make_adc_sensitivity_adjustment(Vector3f &field) const;
    Vector3f _get_filtered_field() const;

    void _reset_filter();
    bool _reset();
    bool _setup_mode();
    bool _check_id();
    bool _calibrate();

    void _update();

    bool _block_read(uint8_t reg, uint8_t *buf, uint32_t size);
    bool _register_read(uint8_t reg, uint8_t *val);
    bool _register_write(uint8_t reg, uint8_t val);
    bool start_measurements();

    Compass* _compass;

    float _magnetometer_ASA[3] {0, 0, 0};
    float _mag_x_accum;
    float _mag_y_accum;
    float _mag_z_accum;
    uint32_t _accum_count;
    uint32_t _last_update_timestamp;
    uint32_t _dev_id;

    uint8_t _compass_instance;
    bool _initialized;
    bool _timesliced;

    bool _started;


};

//class AP_AK8963_BusDriver
//{
//public:
//    virtual ~AP_AK8963_BusDriver() { }
//
//    virtual bool block_read(uint8_t reg, uint8_t *buf, uint32_t size) = 0;
//    virtual bool register_read(uint8_t reg, uint8_t *val) = 0;
//    virtual bool register_write(uint8_t reg, uint8_t val) = 0;
//
//    //virtual AP_HAL::Semaphore  *get_semaphore() = 0;
//
//    virtual bool configure() { return true; }
//    virtual bool start_measurements() { return true; }
//};
//
//class AP_AK8963_BusDriver_HALDevice: public AP_AK8963_BusDriver
//{
//public:
//	AP_AK8963_BusDriver_HALDevice();// TODO I2C AP_HAL::OwnPtr<AP_HAL::I2CDevice> dev);
//
//    virtual bool block_read(uint8_t reg, uint8_t *buf, uint32_t size) override;
//    virtual bool register_read(uint8_t reg, uint8_t *val) override;
//    virtual bool register_write(uint8_t reg, uint8_t val) override;
//
//    //virtual AP_HAL::Semaphore  *get_semaphore() override;
//
//private:
//    //AP_HAL::OwnPtr<AP_HAL::I2CDevice> _dev;  TODO I2C
//};

//class AP_AK8963_BusDriver_Auxiliary : public AP_AK8963_BusDriver
//{
//public:
//    AP_AK8963_BusDriver_Auxiliary(AP_InertialSensor &ins, uint8_t backend_id,
//                                  uint8_t backend_instance, uint8_t addr);
//    ~AP_AK8963_BusDriver_Auxiliary();
//
//    bool block_read(uint8_t reg, uint8_t *buf, uint32_t size) override;
//    bool register_read(uint8_t reg, uint8_t *val) override;
//    bool register_write(uint8_t reg, uint8_t val) override;
//
//    //AP_HAL::Semaphore  *get_semaphore() override;
//
//    bool start_measurements();
//
//private:
//    AuxiliaryBus *_bus;
//    AuxiliaryBusSlave *_slave;
//    bool _started;
//};
