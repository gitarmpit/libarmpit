#include "AP_Compass.h"
#include "debug.h"
#include <string.h>

void
Compass::compass_cal_update()
{
    bool running = false;

    bool failure;
    _calibrator.update(failure);
    if (failure) {
        //TODO
        //AP_Notify::events.compass_cal_failed = 1;
    }

    if (_calibrator.check_for_timeout()) {
        //TODO
        //AP_Notify::events.compass_cal_failed = 1;
        cancel_calibration();
    }

    if (_calibrator.running()) {
        running = true;
    }

    //TODO
    //AP_Notify::flags.compass_cal_running = running;

    if (is_calibrating()) {
        _cal_has_run = true;
        return;
    } else if (_cal_has_run && auto_reboot()) {
        delay(1000);
    }
}

bool
Compass::start_calibration(bool retry, bool autosave, float delay, bool autoreboot)
{
    if (!healthy()) {
        return false;
    }
    _reports_sent = 0;
    if (!is_calibrating() && delay > 0.5f) {
        //AP_Notify::events.initiated_compass_cal = 1;
    }

    _calibrator.set_tolerance(_calibration_threshold);
    _calibrator.start(retry, autosave, delay);
    _compass_cal_autoreboot = autoreboot;


    return true;
}


void
Compass::cancel_calibration()
{
    //AP_Notify::events.initiated_compass_cal = 0;

    if (_calibrator.running() || _calibrator.get_status() == COMPASS_CAL_WAITING_TO_START) {
        //AP_Notify::events.compass_cal_canceled = 1;
    }
    _calibrator.clear();
}


bool
Compass::accept_calibration()
{
    CompassCalibrator& cal = _calibrator;
    uint8_t cal_status = cal.get_status();

    if (cal_status == COMPASS_CAL_SUCCESS) {
        _cal_complete_requires_reboot = true;
        Vector3f ofs, diag, offdiag;
        cal.get_calibration(ofs, diag, offdiag);
        cal.clear();

        set_and_save_offsets(ofs);
        set_and_save_diagonals(diag);
        set_and_save_offdiagonals(offdiag);

        if (!is_calibrating()) {
        }
        return true;
    } else {
        return false;
    }
}


//TODO
//void
//Compass::send_mag_cal_progress(mavlink_channel_t chan)
//{
//    uint8_t cal_mask = get_cal_mask();
//
//    for (uint8_t compass_id=0; compass_id<COMPASS_MAX_INSTANCES; compass_id++) {
//        auto& calibrator = _calibrator[compass_id];
//        uint8_t cal_status = calibrator.get_status();
//
//        if (cal_status == COMPASS_CAL_WAITING_TO_START  ||
//            cal_status == COMPASS_CAL_RUNNING_STEP_ONE ||
//            cal_status == COMPASS_CAL_RUNNING_STEP_TWO) {
//            uint8_t completion_pct = calibrator.get_completion_percent();
//            auto& completion_mask = calibrator.get_completion_mask();
//            Vector3f direction(0.0f,0.0f,0.0f);
//            uint8_t attempt = _calibrator[compass_id].get_attempt();
//
//            // ensure we don't try to send with no space available
//            if (!HAVE_PAYLOAD_SPACE(chan, MAG_CAL_PROGRESS)) {
//                return;
//            }
//
//            mavlink_msg_mag_cal_progress_send(
//                chan,
//                compass_id, cal_mask,
//                cal_status, attempt, completion_pct, completion_mask,
//                direction.x, direction.y, direction.z
//            );
//        }
//    }
//}


//TODO
//void Compass::send_mag_cal_report(mavlink_channel_t chan)
//{
//    uint8_t cal_mask = get_cal_mask();
//
//    for (uint8_t compass_id=0; compass_id<COMPASS_MAX_INSTANCES; compass_id++) {
//
//        uint8_t cal_status = _calibrator[compass_id].get_status();
//
//        if ((cal_status == COMPASS_CAL_SUCCESS ||
//            cal_status == COMPASS_CAL_FAILED) && ((_reports_sent[compass_id] < MAX_CAL_REPORTS) || CONTINUOUS_REPORTS)) {
//            float fitness = _calibrator[compass_id].get_fitness();
//            Vector3f ofs, diag, offdiag;
//            _calibrator[compass_id].get_calibration(ofs, diag, offdiag);
//            uint8_t autosaved = _calibrator[compass_id].get_autosave();
//
//            // ensure we don't try to send with no space available
//            if (!HAVE_PAYLOAD_SPACE(chan, MAG_CAL_REPORT)) {
//                return;
//            }
//
//            mavlink_msg_mag_cal_report_send(
//                chan,
//                compass_id, cal_mask,
//                cal_status, autosaved,
//                fitness,
//                ofs.x, ofs.y, ofs.z,
//                diag.x, diag.y, diag.z,
//                offdiag.x, offdiag.y, offdiag.z
//            );
//            _reports_sent[compass_id]++;
//        }
//
//        if (cal_status == COMPASS_CAL_SUCCESS && _calibrator[compass_id].get_autosave()) {
//            accept_calibration(compass_id);
//        }
//    }
//}

bool
Compass::is_calibrating() const
{
    _calibrator.get_status() != COMPASS_CAL_NOT_STARTED;
}


/*
  handle an incoming MAG_CAL command
 */
//uint8_t Compass::handle_mag_cal_command(const mavlink_command_long_t &packet)
//{
//    uint8_t result = MAV_RESULT_FAILED;
//
//    switch (packet.command) {
//    case MAV_CMD_DO_START_MAG_CAL: {
//        result = MAV_RESULT_ACCEPTED;
//        if (hal.util->get_soft_armed()) {
//            hal.console->println("Disarm for compass calibration");
//            result = MAV_RESULT_FAILED;
//            break;
//        }
//        if (packet.param1 < 0 || packet.param1 > 255) {
//            result = MAV_RESULT_FAILED;
//            break;
//        }
//
//        uint8_t mag_mask = packet.param1;
//        bool retry = !is_zero(packet.param2);
//        bool autosave = !is_zero(packet.param3);
//        float delay = packet.param4;
//        bool autoreboot = !is_zero(packet.param5);
//
//        if (mag_mask == 0) { // 0 means all
//            if (!start_calibration_all(retry, autosave, delay, autoreboot)) {
//                result = MAV_RESULT_FAILED;
//            }
//        } else {
//            if (!start_calibration_mask(mag_mask, retry, autosave, delay, autoreboot)) {
//                result = MAV_RESULT_FAILED;
//            }
//        }
//
//        break;
//    }
//
//    case MAV_CMD_DO_ACCEPT_MAG_CAL: {
//        result = MAV_RESULT_ACCEPTED;
//        if(packet.param1 < 0 || packet.param1 > 255) {
//            result = MAV_RESULT_FAILED;
//            break;
//        }
//
//        uint8_t mag_mask = packet.param1;
//
//        if (mag_mask == 0) { // 0 means all
//            if(!accept_calibration_all()) {
//                result = MAV_RESULT_FAILED;
//            }
//            break;
//        }
//
//        if(!accept_calibration_mask(mag_mask)) {
//            result = MAV_RESULT_FAILED;
//        }
//        break;
//    }
//
//    case MAV_CMD_DO_CANCEL_MAG_CAL: {
//        result = MAV_RESULT_ACCEPTED;
//        if(packet.param1 < 0 || packet.param1 > 255) {
//            result = MAV_RESULT_FAILED;
//            break;
//        }
//
//        uint8_t mag_mask = packet.param1;
//
//        if (mag_mask == 0) { // 0 means all
//            cancel_calibration_all();
//            break;
//        }
//
//        cancel_calibration_mask(mag_mask);
//        break;
//    }
//    }
//
//    return result;
//}
