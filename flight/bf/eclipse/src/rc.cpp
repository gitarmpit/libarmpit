#include "rc.h"
#include <math.h>

RC::RC(rcConfig_t* cfg,  uint16_t minthrottle,  uint16_t maxthrottle)
{
	this->cfg = cfg;

    for (uint8_t i = 0; i < PITCH_LOOKUP_LENGTH; i++)
        lookupPitchRollRC[i] = (2500 + cfg->rcExpo * (i * i - 25)) * i * (int32_t)cfg->rcRate / 2500;


    for (uint8_t i = 0; i < THROTTLE_LOOKUP_LENGTH; i++)
    {
        int16_t tmp = 10 * i - cfg->thrMid;
        uint8_t y = 1;
        if (tmp > 0)
            y = 100 - cfg->thrMid;
        if (tmp < 0)
            y = cfg->thrMid;

        lookupThrottleRC[i] = 10 * cfg->thrMid + tmp * (100 - cfg->thrExpo + (int32_t)cfg->thrExpo * (tmp * tmp) / (y * y)) / 10;
        lookupThrottleRC[i] = minthrottle + (int32_t)(maxthrottle - minthrottle) * lookupThrottleRC[i] / 1000; // [MINTHROTTLE;MAXTHROTTLE]
    }

}

uint16_t pwmReadRawRC(uint8_t chan)
{
	return 0; //TODO read pwm here
}

RC_ACTION RC::computeRC(bool isArmed, uint8_t* angleTrim)
{
    static uint8_t rcSticks;            // this hold sticks position for command combos
    static int16_t rcDataAverage[4][4];
    static int rcAverageIndex = 0;

	uint16_t capture;
	int i;
	RC_ACTION action = RC_ACTION_NONE;

	for (int chan = 0; chan < 4; chan++)
	{
		capture = pwmReadRawRC(chan);

		// validate input
		if (capture < PULSE_MIN || capture > PULSE_MAX)
			capture = cfg->midrc;

		rcDataAverage[chan][rcAverageIndex % 4] = capture;
		// clear this since we're not accessing it elsewhere. saves a temp var
		rcData[chan] = 0;

		for (i = 0; i < 4; i++)
		{
			rcData[chan] += rcDataAverage[chan][i];
		}

		rcData[chan] /= 4;
	}

	rcAverageIndex++;

    uint8_t stTmp = 0;

	for (i = 0; i < 4; i++)
	{
		stTmp >>= 2;
		if (rcData[i] > cfg->mincheck)
			stTmp |= 0x80;  // check for MIN
		if (rcData[i] < cfg->maxcheck)
			stTmp |= 0x40;  // check for MAX
	}

	//check if there is any change
//	if (stTmp == rcSticks)
//	{
//		if (rcDelayCommand < 250)
//			rcDelayCommand++;
//	}
//	else
//		rcDelayCommand = 0;

	rcSticks = stTmp;

	// perform actions
    bool isThrottleLow = false;

	if ((rcData[THROTTLE] < cfg->mincheck))
	{
		isThrottleLow = true;
	}

	if (isThrottleLow)
	{
		//TODO clear this in PID
//		errorGyroI[ROLL] = 0;
//		errorGyroI[PITCH] = 0;
//		errorGyroI[YAW] = 0;
//		errorAngleI[ROLL] = 0;
//		errorAngleI[PITCH] = 0;
	}


	if (isArmed)
	{      // actions during armed
		// Disarm on throttle down + yaw
		if (rcSticks == THR_LO + YAW_LO + PIT_CE + ROL_CE)
			action = RC_ACTION_DISARM;
		// Disarm on roll (only when retarded_arm is enabled)
		if (rcSticks == THR_LO + YAW_CE + PIT_CE + ROL_LO)
            action = RC_ACTION_DISARM;
	}
	else
	{            // actions during not armed

        if (rcSticks == THR_LO + YAW_HI + PIT_CE + ROL_CE) // Arm via YAW
            action = RC_ACTION_ARM;
        else if (rcSticks == THR_LO + YAW_CE + PIT_CE + ROL_HI)  // Arm via ROLL
            action = RC_ACTION_ARM;
        else if (rcSticks == THR_LO + YAW_LO + PIT_LO + ROL_CE) // GYRO calibration
		{
#ifdef GPS
			if (feature(FEATURE_GPS))
				GPS_reset_home_position();
#endif
			//TODO also calibrate baro
		}
		else if (rcSticks == THR_HI + YAW_LO + PIT_LO + ROL_CE)  // Calibrating Acc
		    action = RC_ACTION_ACC_CALIB;
		else if (rcSticks == THR_HI + YAW_HI + PIT_LO + ROL_CE)  // Calibrating Mag
            action = RC_ACTION_MAG_CALIB;

		// Acc Trim
		else if (rcSticks == THR_HI + YAW_CE + PIT_HI + ROL_CE)
		{
			angleTrim[PITCH] = 2;
		}
		else if (rcSticks == THR_HI + YAW_CE + PIT_LO + ROL_CE)
		{
			angleTrim[PITCH] = -2;
		}
		else if (rcSticks == THR_HI + YAW_CE + PIT_CE + ROL_HI)
		{
			angleTrim[ROLL] = 2;
		}
		else if (rcSticks == THR_HI + YAW_CE + PIT_CE + ROL_LO)
		{
			angleTrim[ROLL] = -2;
		}
	}


	//TODO do this in RC loop
//	if (cfg.throttle_correction_value && (f.ANGLE_MODE || f.HORIZON_MODE))
//	{
//		rcCommand[THROTTLE] += throttleAngleCorrection;
//	}


	return action;

}

void RC::processRC(int32_t* dynAdj)
{
    int32_t tmp, tmp2;
    int32_t prop1, prop2;

    // PITCH & ROLL only dynamic PID adjustemnt,  depending on throttle value
    if (rcData[THROTTLE] < cfg->tpa_breakpoint)
    {
        prop2 = 100;
    }
    else
    {
        if (rcData[THROTTLE] < 2000)
        {
            prop2 = 100 - (uint16_t)cfg->dynThrPID * (rcData[THROTTLE] - cfg->tpa_breakpoint) / (2000 - cfg->tpa_breakpoint);
        }
        else
        {
            prop2 = 100 - cfg->dynThrPID;
        }
    }

    for (int axis = 0; axis < 3; axis++)
    {
        tmp = min(abs(rcData[axis] - cfg->midrc), 500);
        if (axis != 2)
        {        // ROLL & PITCH
            if (cfg->deadband_rp)
            {
                if (tmp > cfg->deadband_rp)
                {
                    tmp -= cfg->deadband_rp;
                }
                else
                {
                    tmp = 0;
                }
            }

            tmp2 = tmp / 100;
            rcCommand[axis] = lookupPitchRollRC[tmp2] + (tmp - tmp2 * 100) * (lookupPitchRollRC[tmp2 + 1] - lookupPitchRollRC[tmp2]) / 100;
            prop1 = 100 - (uint16_t)cfg->rates[axis] * tmp / 500;
            prop1 = (uint16_t)prop1 * prop2 / 100;
        }
        else
        {                // YAW
            if (cfg->deadband_yaw)
            {
                if (tmp > cfg->deadband_yaw)
                {
                    tmp -= cfg->deadband_yaw;
                } else
                {
                    tmp = 0;
                }
            }
            rcCommand[axis] = tmp * -cfg->yaw_control_direction;
            prop1 = 100 - (uint16_t)cfg->rates[YAW] * abs(tmp) / 500;
        }

        if (rcData[axis] < cfg->midrc)
            rcCommand[axis] = -rcCommand[axis];

        dynAdj[axis] = prop1;
    }

    tmp = constrain(rcData[THROTTLE], cfg->mincheck, 2000);
    tmp = (uint32_t)(tmp - cfg->mincheck) * 1000 / (2000 - cfg->mincheck);       // [MINCHECK;2000] -> [0;1000]
    tmp2 = tmp / 100;
    rcCommand[THROTTLE] = lookupThrottleRC[tmp2] + (tmp - tmp2 * 100) * (lookupThrottleRC[tmp2 + 1] - lookupThrottleRC[tmp2]) / 100;    // [0;1000] -> expo -> [MINTHROTTLE;MAXTHROTTLE]


    //TODO call headfreemode after this

    //TODO calibrating leds



//    if (sensors(SENSOR_GPS)) {
//        static uint32_t GPSLEDTime;
//        if ((int32_t)(currentTime - GPSLEDTime) >= 0 && (GPS_numSat >= 5)) {
//            GPSLEDTime = currentTime + 150000;
//            //LED1_TOGGLE;
//        }
//    }

}


void RC::headFreeMode(int16_t heading, int16_t headFreeModeHold)
{
    float radDiff = (heading - headFreeModeHold) * M_PI / 180.0f;
    float cosDiff = cosf(radDiff);
    float sinDiff = sinf(radDiff);
    int16_t rcCommand_PITCH = rcCommand[PITCH] * cosDiff + rcCommand[ROLL] * sinDiff;
    rcCommand[ROLL] = rcCommand[ROLL] * cosDiff - rcCommand[PITCH] * sinDiff;
    rcCommand[PITCH] = rcCommand_PITCH;
}




