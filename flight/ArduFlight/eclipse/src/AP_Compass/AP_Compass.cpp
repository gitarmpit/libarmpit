
#include "AP_Compass.h"

#include "debug.h"

#define COMPASS_LEARN_DEFAULT Compass::LEARN_NONE

// Default constructor.
// Note that the Vector/Matrix constructors already implicitly zero
// their values.
//
Compass::Compass(void) :
    _compass_cal_autoreboot(false),
    _cal_complete_requires_reboot(false),
    _cal_has_run(false),
    _board_orientation(ROTATION_NONE),
    _null_init_done(false),
    _thr_or_curr(0.0f),
    _backend(this)
{
    _state.last_update_usec = 0;
    _reports_sent = 0;
    _state.dev_id = 0;

    //XYZ Compass offsets in milligauss
    //added to the compass values to compensate for metal in the frame
    //Range -400, 400
    _state.offset.zero();
    _declination = 0;  //TODO
    _learn = COMPASS_LEARN_DEFAULT;
    _state.use_for_yaw = 1;
    _auto_declination = 1;  //automatic calculation of the declination based on gps location

    //0:Disabled,1:Use Throttle,2:Use Current
    _motor_comp_type = AP_COMPASS_MOT_COMP_DISABLED;  //Motor interference compensation type

    //Motor interference compensation for body frame
    //Multiplied by the current throttle and added to the compass's XYZ axis values to compensate for motor interference
    //Range: -1000 1000, Offset per Amp or at Full Throttle
    _state.motor_compensation.zero();

    /* The orientation of the compass relative to the autopilot board.
     * This will default to the right value for each board type, but can be changed if you have an external compass.
     * See the documentation for your external compass for the right value.
     * The correct orientation should give the X axis forward, the Y axis to the right and the Z axis down.
     * So if your aircraft is pointing west it should show a positive value for the Y axis, and a value close to zero for the X axis.
     * On a PX4 or Pixhawk with an external compass the correct value is zero if the compass is correctly oriented.
     * NOTE: This orientation is combined with any AHRS_ORIENTATION setting.*/
    _state.orientation = ROTATION_NONE;

    //internal compass
    _state.external = 0;


    //Compass soft-iron diagonal component
    _state.diagonals.zero();

    //Compass soft-iron off diagonal component
    _state.offdiagonals.zero();

    //This controls the fitness level required for a successful compass calibration.
    //A lower value makes for a stricter fit (less likely to pass).
    _calibration_threshold = 8.0f;

}

// Default init method
//
bool
Compass::init()
{
    delay(100);
    read();
    return true;
}



void
Compass::accumulate(void)
{
    //TODO
//    _backend.accumulate();
}

bool
Compass::read(void)
{
    _backend.read();


    _state.healthy = (millis() - _state.last_update_ms < 500);

    return healthy();
}


void
Compass::set_offsets(const Vector3f &offsets)
{
    _state.offset = (offsets);
}

void
Compass::set_and_save_offsets(const Vector3f &offsets)
{
    _state.offset = (offsets);
    save_offsets();
}

void
Compass::set_and_save_diagonals(const Vector3f &diagonals)
{
    _state.diagonals = (diagonals);
}

void
Compass::set_and_save_offdiagonals(const Vector3f &offdiagonals)
{
    _state.offdiagonals = (offdiagonals);
}

void
Compass::save_offsets() //TODO
{
}


void
Compass::set_motor_compensation(const Vector3f &motor_comp_factor)
{
    _state.motor_compensation = (motor_comp_factor);
}

void
Compass::save_motor_compensation() //TODO
{
}

void
Compass::set_initial_location(int32_t latitude, int32_t longitude)
{
    // if automatic declination is configured, then compute
    // the declination based on the initial GPS fix
    if (_auto_declination) {
        // Set the declination based on the lat/lng from GPS
        _declination = (radians(
                AP_Declination::get_declination(
                    (float)latitude / 10000000,
                    (float)longitude / 10000000)));
    }
}

/// return true if the compass should be used for yaw calculations
bool
Compass::use_for_yaw(void) const
{
    return healthy() && _state.use_for_yaw;
}


void
Compass::set_declination(float radians, bool save_to_eeprom)
{
    _declination = (radians);
}

float
Compass::get_declination() const
{
    return _declination;
}

/*
  calculate a compass heading given the attitude from DCM and the mag vector
 */
float
Compass::calculate_heading(const Matrix3f &dcm_matrix) const
{
    float cos_pitch_sq = 1.0f-(dcm_matrix.c.x*dcm_matrix.c.x);

    // Tilt compensated magnetic field Y component:
    const Vector3f &field = get_field();

    float headY = field.y * dcm_matrix.c.z - field.z * dcm_matrix.c.y;

    // Tilt compensated magnetic field X component:
    float headX = field.x * cos_pitch_sq - dcm_matrix.c.x * (field.y * dcm_matrix.c.y + field.z * dcm_matrix.c.z);

    // magnetic heading
    // 6/4/11 - added constrain to keep bad values from ruining DCM Yaw - Jason S.
    float heading = constrain_float(atan2f(-headY,headX), -3.15f, 3.15f);

    // Declination correction (if supplied)
    if( fabsf(_declination) > 0.0f )
    {
        heading = heading + _declination;
        if (heading > M_PI)    // Angle normalization (-180 deg, 180 deg)
            heading -= (2.0f * M_PI);
        else if (heading < -M_PI)
            heading += (2.0f * M_PI);
    }

    return heading;
}

/// Returns True if the compasses have been configured (i.e. offsets saved)
///
/// @returns                    True if compass has been configured
///
bool Compass::configured()
{
    // exit immediately if all offsets are zero
    if (is_zero(get_offsets().length())) {
        return false;
    }

    // backup detected dev_id
    int32_t dev_id_orig = _state.dev_id;

    // load dev_id from eeprom
    //_state[i].dev_id.load();    TODO

    // if different then the device has not been configured
    if (_state.dev_id != dev_id_orig) {
        // restore device id
        _state.dev_id = dev_id_orig;
        // return failure
        return false;
    }

    // if we got here then it must be configured
    return true;
}




/*
  set the type of motor compensation to use
 */
void Compass::motor_compensation_type(const uint8_t comp_type)
{
    if (_motor_comp_type <= AP_COMPASS_MOT_COMP_CURRENT && _motor_comp_type != (int8_t)comp_type) {
        _motor_comp_type = (int8_t)comp_type;
        _thr_or_curr = 0;                               // set current current or throttle to zero
        set_motor_compensation(Vector3f(0,0,0)); // clear out invalid compensation vectors
    }
}

bool Compass::consistent() const
{
    Vector3f primary_mag_field = get_field();
    Vector3f primary_mag_field_norm;

    if (!primary_mag_field.is_zero()) {
        primary_mag_field_norm = primary_mag_field.normalized();
    } else {
        return false;
    }

    Vector2f primary_mag_field_xy = Vector2f(primary_mag_field.x,primary_mag_field.y);
    Vector2f primary_mag_field_xy_norm;

    if (!primary_mag_field_xy.is_zero()) {
        primary_mag_field_xy_norm = primary_mag_field_xy.normalized();
    } else {
        return false;
    }

    if (use_for_yaw()) {
        Vector3f mag_field = get_field();
        Vector3f mag_field_norm;

        if (!mag_field.is_zero()) {
            mag_field_norm = mag_field.normalized();
        } else {
            return false;
        }

        Vector2f mag_field_xy = Vector2f(mag_field.x,mag_field.y);
        Vector2f mag_field_xy_norm;

        if (!mag_field_xy.is_zero()) {
            mag_field_xy_norm = mag_field_xy.normalized();
        } else {
            return false;
        }

        float xyz_ang_diff = acosf(constrain_float(mag_field_norm * primary_mag_field_norm,-1.0f,1.0f));
        float xy_ang_diff  = acosf(constrain_float(mag_field_xy_norm*primary_mag_field_xy_norm,-1.0f,1.0f));
        float xy_len_diff  = (primary_mag_field_xy-mag_field_xy).length();

        // check for gross misalignment on all axes
        bool xyz_ang_diff_large = xyz_ang_diff > AP_COMPASS_MAX_XYZ_ANG_DIFF;

        // check for an unacceptable angle difference on the xy plane
        bool xy_ang_diff_large = xy_ang_diff > AP_COMPASS_MAX_XY_ANG_DIFF;

        // check for an unacceptable length difference on the xy plane
        bool xy_length_diff_large = xy_len_diff > AP_COMPASS_MAX_XY_LENGTH_DIFF;

        // check for inconsistency in the XY plane
        if (xyz_ang_diff_large || xy_ang_diff_large || xy_length_diff_large) {
            return false;
        }
    }
    return true;
}



void Compass::rotate_field(Vector3f &mag)
{
    mag.rotate(MAG_BOARD_ORIENTATION);

    if (!_state.external) {
        // and add in AHRS_ORIENTATION setting if not an external compass
        mag.rotate(_board_orientation);
    } else {
        // add user selectable orientation
        mag.rotate((enum Rotation)_state.orientation);
    }
}

void Compass::publish_raw_field(const Vector3f &mag, uint32_t time_us)
{
    // note that we do not set last_update_usec here as otherwise the
    // EKF and DCM would end up consuming compass data at the full
    // sensor rate. We want them to consume only the filtered fields
    _state.last_update_ms = millis();
    _calibrator.new_sample(mag);
}

void Compass::correct_field(Vector3f &mag)
{

    if (_state.diagonals.is_zero()) {
        _state.diagonals = (Vector3f(1.0f,1.0f,1.0f));
    }

    const Vector3f &offsets = _state.offset;
    const Vector3f &diagonals = _state.diagonals;
    const Vector3f &offdiagonals = _state.offdiagonals;
    const Vector3f &mot = _state.motor_compensation;

    /*
     * note that _motor_offset[] is kept even if compensation is not
     * being applied so it can be logged correctly
     */
    mag += offsets;
    if(_motor_comp_type != AP_COMPASS_MOT_COMP_DISABLED && !is_zero(_thr_or_curr)) {
        _state.motor_offset = mot * _thr_or_curr;
        mag += _state.motor_offset;
    } else {
        _state.motor_offset.zero();
    }

    Matrix3f mat(
        diagonals.x, offdiagonals.x, offdiagonals.y,
        offdiagonals.x,    diagonals.y, offdiagonals.z,
        offdiagonals.y, offdiagonals.z,    diagonals.z
    );

    mag = mat * mag;
}

/*
  copy latest data to the frontend from a backend
 */
void Compass::publish_filtered_field(const Vector3f &mag)
{
    _state.field = mag;
    _state.last_update_ms = millis();
    _state.last_update_usec = micros();
}


void Compass::set_last_update_usec(uint32_t last_update)
{
    _state.last_update_usec = last_update;
}


void Compass::set_dev_id(uint32_t dev_id)
{
    _state.dev_id = (dev_id);
}



