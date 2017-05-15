#include "mw.h"


void taskBaro()
{
#ifdef BARO
	static int16_t initialThrottleHold;
	if (sensors(SENSOR_BARO)) {
		// Baro alt hold activate
		if (rcOptions[BOXBARO]) {
			if (!f.BARO_MODE) {
				f.BARO_MODE = 1;
				AltHold = EstAlt;
				initialThrottleHold = rcCommand[THROTTLE];
				errorVelocityI = 0;
				BaroPID = 0;
			}
		} else {
			f.BARO_MODE = 0;
		}
	}
#endif

#ifdef BARO
	if (sensors(SENSOR_BARO)) {
		if (f.BARO_MODE) {
			static uint8_t isAltHoldChanged = 0;
			// multirotor alt hold
			if (cfg.alt_hold_fast_change) {
				// rapid alt changes
				if (abs(rcCommand[THROTTLE] - initialThrottleHold) > cfg.alt_hold_throttle_neutral) {
					errorVelocityI = 0;
					isAltHoldChanged = 1;
					rcCommand[THROTTLE] += (rcCommand[THROTTLE] > initialThrottleHold) ? -cfg.alt_hold_throttle_neutral : cfg.alt_hold_throttle_neutral;
				} else {
					if (isAltHoldChanged) {
						AltHold = EstAlt;
						isAltHoldChanged = 0;
					}
					rcCommand[THROTTLE] = constrain(initialThrottleHold + BaroPID, mcfg.minthrottle, mcfg.maxthrottle);
				}
			} else {
				// slow alt changes for apfags
				if (abs(rcCommand[THROTTLE] - initialThrottleHold) > cfg.alt_hold_throttle_neutral) {
					// set velocity proportional to stick movement +100 throttle gives ~ +50 cm/s
					setVelocity = (rcCommand[THROTTLE] - initialThrottleHold) / 2;
					velocityControl = 1;
					isAltHoldChanged = 1;
				} else if (isAltHoldChanged) {
					AltHold = EstAlt;
					velocityControl = 0;
					isAltHoldChanged = 0;
				}
				rcCommand[THROTTLE] = constrain(initialThrottleHold + BaroPID, mcfg.minthrottle, mcfg.maxthrottle);
			}
		}
	}
#endif

}

void taskMag()
{
#ifdef  MAG
	//activate mag
        if (sensors(SENSOR_ACC) || sensors(SENSOR_MAG)) {
            if (rcOptions[BOXMAG]) {
                if (!f.MAG_MODE) {
                    f.MAG_MODE = 1;
                    magHold = heading;
                }
            } else {
                f.MAG_MODE = 0;
            }
            if (rcOptions[BOXHEADFREE]) {
                if (!f.HEADFREE_MODE) {
                    f.HEADFREE_MODE = 1;
                }
            } else {
                f.HEADFREE_MODE = 0;
            }
            if (rcOptions[BOXHEADADJ]) {
                headFreeModeHold = heading; // acquire new heading
            }
        }
#endif

//part 2: use mag
#ifdef MAG
        if (sensors(SENSOR_MAG)) {
            if (abs(rcCommand[YAW]) < 70 && f.MAG_MODE) {
                int16_t dif = heading - magHold;
                if (dif <= -180)
                    dif += 360;
                if (dif >= +180)
                    dif -= 360;
                dif *= -mcfg.yaw_control_direction;
                if (f.SMALL_ANGLE)
                    rcCommand[YAW] -= dif * cfg.P8[PIDMAG] / 30;    // 18 deg
            } else
                magHold = heading;
        }
#endif



}


void taskGps()
{
//part 1: in RC loop
#ifdef GPS
	static uint8_t GPSNavReset = 1;
	if (sensors(SENSOR_GPS)) {
		if (f.GPS_FIX && GPS_numSat >= 5) {
			if (nav_mode != NAV_MODE_NONE && (!f.HORIZON_MODE && !f.ANGLE_MODE))
				f.ANGLE_MODE = true; // Force a stable mode in GPS Mode

			// if both GPS_HOME & GPS_HOLD are checked => GPS_HOME is the priority
			if (rcOptions[BOXGPSHOME] || f.FW_FAILSAFE_RTH_ENABLE ) {
				if (!f.GPS_HOME_MODE) {
					f.GPS_HOME_MODE = 1;
					f.GPS_HOLD_MODE = 0;
					GPSNavReset = 0;
					GPS_set_next_wp(&GPS_home[LAT], &GPS_home[LON]);
					nav_mode = NAV_MODE_WP;
					GPS_hold[ALT] = GPS_altitude;
					f.CLIMBOUT_FW = 1;
				}
			} else {
				f.GPS_HOME_MODE = 0;
				if (rcOptions[BOXGPSHOLD] && abs(rcCommand[ROLL]) < cfg.ap_mode && abs(rcCommand[PITCH]) < cfg.ap_mode) {
					if (!f.GPS_HOLD_MODE) {
						f.GPS_HOLD_MODE = 1;
						GPSNavReset = 0;
						GPS_hold[LAT] = GPS_coord[LAT];
						GPS_hold[LON] = GPS_coord[LON];
						GPS_set_next_wp(&GPS_hold[LAT], &GPS_hold[LON]);
						nav_mode = NAV_MODE_POSHOLD;
						GPS_hold[ALT] = GPS_altitude;
						f.CLIMBOUT_FW = 0;
					}
				} else {
					f.GPS_HOLD_MODE = 0;
					// both boxes are unselected here, nav is reset if not already done
					if (GPSNavReset == 0) {
						GPSNavReset = 1;
						GPS_reset_nav();
						f.CLIMBOUT_FW = 0;
					}
				}
			}
			// Beep for indication that GPS has found satellites and naze32 is ready to fly
			buzzer(BUZZER_READY_BEEP);
		} else {
			f.GPS_HOME_MODE = 0;
			f.GPS_HOLD_MODE = 0;
			nav_mode = NAV_MODE_NONE;
		}
	}
#endif
//part 2: outside rc loop

#ifdef GPS
        if (sensors(SENSOR_GPS))
        {
            if ((f.GPS_HOME_MODE || f.GPS_HOLD_MODE) && f.GPS_FIX_HOME)
            {
                float sin_yaw_y = sinf(heading * 0.0174532925f);
                float cos_yaw_x = cosf(heading * 0.0174532925f);
                if (cfg.nav_slew_rate)
                {
                	nav_rated[LON] += constrain(wrap_18000(nav[LON] - nav_rated[LON]), -cfg.nav_slew_rate, cfg.nav_slew_rate); // TODO check this on uint8
                	nav_rated[LAT] += constrain(wrap_18000(nav[LAT] - nav_rated[LAT]), -cfg.nav_slew_rate, cfg.nav_slew_rate);
                	GPS_angle[ROLL] = (nav_rated[LON] * cos_yaw_x - nav_rated[LAT] * sin_yaw_y) / 10;
                	GPS_angle[PITCH] = (nav_rated[LON] * sin_yaw_y + nav_rated[LAT] * cos_yaw_x) / 10;
                }
                else
                {
                	GPS_angle[ROLL] = (nav[LON] * cos_yaw_x - nav[LAT] * sin_yaw_y) / 10;
                	GPS_angle[PITCH] = (nav[LON] * sin_yaw_y + nav[LAT] * cos_yaw_x) / 10;
                }
            }
            else
            {
                GPS_angle[ROLL] = 0;
                GPS_angle[PITCH] = 0;
                GPS_angle[YAW] = 0;
            }
        }
#endif

}

int main (void)
{


	return 0;
}
