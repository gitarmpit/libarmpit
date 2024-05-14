// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
#pragma once

#include <inttypes.h>

#include <AP_Common/AP_Common.h>
#include <AP_Declination/AP_Declination.h>
#include <AP_Math/AP_Math.h>

#include "CompassCalibrator.h"
#include "AP_Compass_AK8963.h"


// compass product id
#define AP_COMPASS_TYPE_UNKNOWN         0x00
#define AP_COMPASS_TYPE_HIL             0x01
#define AP_COMPASS_TYPE_HMC5843         0x02
#define AP_COMPASS_TYPE_HMC5883L        0x03
#define AP_COMPASS_TYPE_PX4             0x04
#define AP_COMPASS_TYPE_VRBRAIN         0x05
#define AP_COMPASS_TYPE_AK8963_MPU9250  0x06
#define AP_COMPASS_TYPE_AK8963_I2C      0x07
#define AP_COMPASS_TYPE_LSM303D         0x08
#define AP_COMPASS_TYPE_LSM9DS1         0x09

// motor compensation types (for use with motor_comp_enabled)
#define AP_COMPASS_MOT_COMP_DISABLED    0x00
#define AP_COMPASS_MOT_COMP_THROTTLE    0x01
#define AP_COMPASS_MOT_COMP_CURRENT     0x02

# define MAG_BOARD_ORIENTATION ROTATION_ROLL_180
//# define MAG_BOARD_ORIENTATION ROTATION_YAW_90
//# define MAG_BOARD_ORIENTATION ROTATION_YAW_270
//# define MAG_BOARD_ORIENTATION ROTATION_NONE


//MAXIMUM COMPASS REPORTS
#define MAX_CAL_REPORTS 10
#define CONTINUOUS_REPORTS 0
#define AP_COMPASS_MAX_XYZ_ANG_DIFF radians(50.0f)
#define AP_COMPASS_MAX_XY_ANG_DIFF radians(30.0f)
#define AP_COMPASS_MAX_XY_LENGTH_DIFF 100.0f

class Compass
{
public:
    Compass();
    bool init();

    /// Read the compass and update the mag_ variables.
    bool read();

    /// use spare CPU cycles to accumulate values from the compass if
    /// possible (this method should also be implemented in the backends)
    void accumulate();

    /// Calculate the tilt-compensated heading_ variables.
    /// @returns heading in radians
    float calculate_heading(const Matrix3f &dcm_matrix) const;

    /// Sets offset x/y/z values.
    /// @param  offsets             Offsets to the raw mag_ values in milligauss.
    void set_offsets(const Vector3f &offsets);

    void set_and_save_offsets(const Vector3f &offsets);
    void set_and_save_diagonals(const Vector3f &diagonals);
    void set_and_save_offdiagonals(const Vector3f &diagonals);

    /// Saves the current offset x/y/z values for one or all compasses
    ///
    /// @param  i                   compass instance
    ///
    /// This should be invoked periodically to save the offset values maintained by
    /// ::learn_offsets.
    ///
    void save_offsets(uint8_t i);
    void save_offsets(void);


    /// Return the current field as a Vector3f in milligauss
    const Vector3f &get_field() const { return _state.field; }

    // compass calibrator interface
    void compass_cal_update();

    bool start_calibration(bool retry=false, bool autosave=false, float delay_sec=0.0f, bool autoreboot = false);

    void cancel_calibration();

    bool accept_calibration();

    bool compass_cal_requires_reboot() { return _cal_complete_requires_reboot; }
    bool auto_reboot() { return _compass_cal_autoreboot; }
    bool is_calibrating() const;

    /*
      handle an incoming MAG_CAL command
    */
    //uint8_t handle_mag_cal_command(const mavlink_command_long_t &packet);

    //void send_mag_cal_progress(mavlink_channel_t chan);
    //void send_mag_cal_report(mavlink_channel_t chan);

    // check if the compasses are pointing in the same direction
    bool consistent() const;

    /// Return the health of a compass
    bool healthy() const { return _state.healthy; }

    /// @returns                    The current compass offsets in milligauss.
    const Vector3f &get_offsets() const { return _state.offset; }

    /// Sets the initial location used to get declination
    ///
    /// @param  latitude             GPS Latitude.
    /// @param  longitude            GPS Longitude.
    ///
    void set_initial_location(int32_t latitude, int32_t longitude);

    /// Program new offset values.
    ///
    /// @param  i                   compass instance
    /// @param  x                   Offset to the raw mag_x value in milligauss.
    /// @param  y                   Offset to the raw mag_y value in milligauss.
    /// @param  z                   Offset to the raw mag_z value in milligauss.
    ///
    void set_and_save_offsets(int x, int y, int z) {
        set_and_save_offsets(Vector3f(x, y, z));
    }

    // learn offsets accessor
    bool learn_offsets_enabled() const { return _learn; }

    /// Perform automatic offset updates
    ///
    void learn_offsets(void);

    /// return true if the compass should be used for yaw calculations
    bool use_for_yaw(void) const;

    /// Sets the local magnetic field declination.
    ///
    /// @param  radians             Local field declination.
    /// @param save_to_eeprom       true to save to eeprom (false saves only to memory)
    ///
    void set_declination(float radians, bool save_to_eeprom = true);
    float get_declination() const;

    // set overall board orientation
    void set_board_orientation(enum Rotation orientation) {
        _board_orientation = orientation;
    }

    /// Set the motor compensation type
    ///
    /// @param  comp_type           0 = disabled, 1 = enabled use throttle, 2 = enabled use current
    ///
    void motor_compensation_type(const uint8_t comp_type);

    /// get the motor compensation value.
    uint8_t get_motor_compensation_type() const {
        return _motor_comp_type;
    }

    /// Set the motor compensation factor x/y/z values.
    ///
    /// @param  i                   instance of compass
    /// @param  offsets             Offsets multiplied by the throttle value and added to the raw mag_ values.
    ///
    void set_motor_compensation(const Vector3f &motor_comp_factor);

    /// get motor compensation factors as a vector
    const Vector3f& get_motor_compensation() const { return _state.motor_compensation; }

    /// Saves the current motor compensation x/y/z values.
    ///
    /// This should be invoked periodically to save the offset values calculated by the motor compensation auto learning
    ///
    void save_motor_compensation();

    /// Returns the current motor compensation offset values
    ///
    /// @returns                    The current compass offsets in milligauss.
    ///
    const Vector3f &get_motor_offsets() const { return _state.motor_offset; }

    /// Set the throttle as a percentage from 0.0 to 1.0
    /// @param thr_pct              throttle expressed as a percentage from 0 to 1.0
    void set_throttle(float thr_pct) {
        if (_motor_comp_type == AP_COMPASS_MOT_COMP_THROTTLE) {
            _thr_or_curr = thr_pct;
        }
    }

    /// Set the current used by system in amps
    /// @param amps                 current flowing to the motors expressed in amps
    void set_current(float amps) {
        if (_motor_comp_type == AP_COMPASS_MOT_COMP_CURRENT) {
            _thr_or_curr = amps;
        }
    }

    bool configured(void);


    void        _setup_earth_field();


    // return last update time in microseconds
    uint32_t last_update_usec() const { return _state.last_update_usec; }



    enum LearnType {
        LEARN_NONE=0,
        LEARN_INTERNAL=1,
        LEARN_EKF=2
    };

    // return the chosen learning type
    enum LearnType get_learn_type(void) const {
        return (enum LearnType)_learn;
    }
    

    /*
     * A compass measurement is expected to pass through the following functions:
     * 1. rotate_field - this rotates the measurement in-place from sensor frame
     *      to body frame
     * 2. publish_raw_field - this provides an uncorrected point-sample for
     *      calibration libraries
     * 3. correct_field - this corrects the measurement in-place for hard iron,
     *      soft iron, motor interference, and non-orthagonality errors
     * 4. publish_filtered_field - legacy filtered magnetic field
     *
     * All those functions expect the mag field to be in milligauss.
     */


    ////moved from backend
    void rotate_field(Vector3f &mag);
    void publish_raw_field(const Vector3f &mag, uint32_t time_us);
    void correct_field(Vector3f &mag);
    void publish_filtered_field(const Vector3f &mag);
    void set_last_update_usec(uint32_t last_update);


    // set dev_id for an instance
    void set_dev_id(uint32_t dev_id);


private:

    AP_Compass_AK8963 _backend;

    //keep track of number of calibration reports sent
    uint8_t _reports_sent;

    //autoreboot after compass calibration
    bool _compass_cal_autoreboot;
    bool _cal_complete_requires_reboot;
    bool _cal_has_run;


    // settable parameters
    AP_Int8 _learn;

    // board orientation from AHRS
    enum Rotation _board_orientation;

    // primary instance
    AP_Int8     _primary;

    // declination in radians
    AP_Float    _declination;

    // enable automatic declination code
    AP_Int8     _auto_declination;

    // first-time-around flag used by offset nulling
    bool        _null_init_done;

    // used by offset correction
    static const uint8_t _mag_history_size = 20;

    // motor compensation type
    // 0 = disabled, 1 = enabled for throttle, 2 = enabled for current
    AP_Int8     _motor_comp_type;

    // throttle expressed as a percentage from 0 ~ 1.0 or current expressed in amps
    float       _thr_or_curr;

    struct mag_state {
        AP_Int8     external;
        bool        healthy;
        AP_Int8     orientation;
        Vector3f offset;
        Vector3f diagonals;
        Vector3f offdiagonals;

        // device id detected at init.
        // saved to eeprom when offsets are saved allowing ram &
        // eeprom values to be compared as consistency check
        AP_Int32    dev_id;

        AP_Int8     use_for_yaw;

        uint8_t     mag_history_index;
        Vector3i    mag_history[_mag_history_size];

        // factors multiplied by throttle and added to compass outputs
        Vector3f motor_compensation;

        // latest compensation added to compass
        Vector3f    motor_offset;

        // corrected magnetic field strength
        Vector3f    field;

        // when we last got data
        uint32_t    last_update_ms;
        uint32_t    last_update_usec;
    } _state;

    CompassCalibrator _calibrator;

    AP_Float _calibration_threshold;
};
