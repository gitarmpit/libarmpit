/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <assert.h>
#include <utility>

#include <AP_Math/AP_Math.h>

#include "AP_Compass_AK8963.h"
#include "AP_Compass.h"
#include <AP_InertialSensor/AP_InertialSensor_MPU9250.h>

#define AK8963_I2C_ADDR                                 0x0c

#define AK8963_WIA                                      0x00
#        define AK8963_Device_ID                        0x48

#define AK8963_HXL                                      0x03

/* bit definitions for AK8963 CNTL1 */
#define AK8963_CNTL1                                    0x0A
#        define    AK8963_CONTINUOUS_MODE1              0x02
#        define    AK8963_CONTINUOUS_MODE2              0x06
#        define    AK8963_SELFTEST_MODE                 0x08
#        define    AK8963_POWERDOWN_MODE                0x00
#        define    AK8963_FUSE_MODE                     0x0f
#        define    AK8963_16BIT_ADC                     0x10
#        define    AK8963_14BIT_ADC                     0x00

#define AK8963_CNTL2                                    0x0B
#        define AK8963_RESET                            0x01

#define AK8963_ASAX                                     0x10

#define AK8963_MILLIGAUSS_SCALE 10.0f

struct PACKED sample_regs {
    int16_t val[3];
    uint8_t st2;
};


AP_Compass_AK8963::AP_Compass_AK8963(Compass* compass)
{
	_compass = compass;
	_dev_id = AP_COMPASS_TYPE_AK8963_I2C;
    _reset_filter();
    init();
}



bool AP_Compass_AK8963::init()
{

    if (!_check_id()) {
        goto fail;
    }

    if (!_calibrate()) {
        goto fail;
    }

    //setup mode
    if (!_register_write(AK8963_CNTL1, AK8963_CONTINUOUS_MODE2 | AK8963_16BIT_ADC))
        goto fail;

    if (!start_measurements()) {
        goto fail;
    }

    _initialized = true;


    /* timer needs to be called every 10ms so set the freq_div to 10 */
	//TODO scheduling
    //_timesliced = hal.scheduler->register_timer_process(FUNCTOR_BIND_MEMBER(&AP_Compass_AK8963::_update, void), 10);


    return true;

fail:
fail_sem:

    return false;
}

void AP_Compass_AK8963::read()
{
    if (!_initialized) {
        return;
    }

    if (_accum_count == 0) {
        /* We're not ready to publish*/
        return;
    }

    //hal.scheduler->suspend_timer_procs();  TODO
    Vector3f field(_mag_x_accum, _mag_y_accum, _mag_z_accum);
    field /= _accum_count;

    _mag_x_accum = _mag_y_accum = _mag_z_accum = 0;
    _accum_count = 0;

    //hal.scheduler->resume_timer_procs();
    _compass->publish_filtered_field(field);
}


void AP_Compass_AK8963::_make_adc_sensitivity_adjustment(Vector3f& field) const
{
    static const float ADC_16BIT_RESOLUTION = 0.15f;

    field *= ADC_16BIT_RESOLUTION;
}

void AP_Compass_AK8963::_make_factory_sensitivity_adjustment(Vector3f& field) const
{
    field.x *= _magnetometer_ASA[0];
    field.y *= _magnetometer_ASA[1];
    field.z *= _magnetometer_ASA[2];
}

void AP_Compass_AK8963::_update()
{
    struct sample_regs regs;
    Vector3f raw_field;
    uint32_t time_us = micros();

    if (!_timesliced &&
        micros() - _last_update_timestamp < 10000) {
        goto end;
    }


    if (!_block_read(AK8963_HXL, (uint8_t *) &regs, sizeof(regs))) {
        goto fail;
    }

    /* Check for overflow. See AK8963's datasheet, section
     * 6.4.3.6 - Magnetic Sensor Overflow. */
    if ((regs.st2 & 0x08)) {
        goto fail;
    }

    raw_field = Vector3f(regs.val[0], regs.val[1], regs.val[2]);

    if (is_zero(raw_field.x) && is_zero(raw_field.y) && is_zero(raw_field.z)) {
        goto fail;
    }

    _make_factory_sensitivity_adjustment(raw_field);
    _make_adc_sensitivity_adjustment(raw_field);
    raw_field *= AK8963_MILLIGAUSS_SCALE;

    // rotate raw_field from sensor frame to body frame
    _compass->rotate_field(raw_field);

    // publish raw_field (uncorrected point sample) for calibration use
    _compass->publish_raw_field(raw_field, time_us);

    // correct raw_field for known errors
    _compass->correct_field(raw_field);

    _mag_x_accum += raw_field.x;
    _mag_y_accum += raw_field.y;
    _mag_z_accum += raw_field.z;
    _accum_count++;
    if (_accum_count == 10) {
        _mag_x_accum /= 2;
        _mag_y_accum /= 2;
        _mag_z_accum /= 2;
        _accum_count = 5;
    }

    _last_update_timestamp = micros();
fail:
end:
    return;
}

bool AP_Compass_AK8963::_check_id()
{
    for (int i = 0; i < 5; i++) {
        uint8_t deviceid = 0;

        /* Read AK8963's id */
        if (_register_read(AK8963_WIA, &deviceid) &&
            deviceid == AK8963_Device_ID) {
            return true;
        }
    }

    return false;
}


bool AP_Compass_AK8963::_reset()
{
    return _register_write(AK8963_CNTL2, AK8963_RESET);
}


bool AP_Compass_AK8963::_calibrate()
{
    /* Enable FUSE-mode in order to be able to read calibration data */
    _register_write(AK8963_CNTL1, AK8963_FUSE_MODE | AK8963_16BIT_ADC);

    uint8_t response[3];

    _block_read(AK8963_ASAX, response, 3);

    for (int i = 0; i < 3; i++) {
        float data = response[i];
        _magnetometer_ASA[i] = ((data - 128) / 256 + 1);
    }

    return true;
}

bool AP_Compass_AK8963::start_measurements()
{
//    if (_register_periodic_read(_slave, AK8963_HXL, sizeof(sample_regs)) < 0) {
//        return false;
//    }

    _started = true;

    return true;
}



bool AP_Compass_AK8963::_block_read(uint8_t reg, uint8_t *buf, uint32_t size)
{
    if (_started) {
        /*
         * We can only read a block when reading the block of sample values -
         * calling with any other value is a mistake
         */
        assert(reg == AK8963_HXL);

		int n = 0;  // = _slave->read(buf);  //TODO
        return n == static_cast<int>(size);
    }

	int r = 0; // _slave->passthrough_read(reg, buf, size);  //TODO

    return r > 0 && static_cast<uint32_t>(r) == size;
}

bool AP_Compass_AK8963::_register_read(uint8_t reg, uint8_t *val)
{
	//TODO
    //return _slave->passthrough_read(reg, val, 1) == 1;
	return true;
}

bool AP_Compass_AK8963::_register_write(uint8_t reg, uint8_t val)
{
	//TODO
	//return _slave->passthrough_write(reg, val) == 1;
	return true;
}




