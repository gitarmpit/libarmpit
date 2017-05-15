#ifndef CONFIG_H_
#define CONFIG_H_

#define QUADX

#define MINTHROTTLE 1150
#define MAXTHROTTLE 1850
#define MINCOMMAND  1000  //not spinning


#define PID_CONTROLLER 1

#define YAW_DIRECTION 1


#define CONTROL_RANGE   { 100, 100 }      //  { ROLL,PITCH }

#define DEADBAND 0

// Guard time for failsafe activation after signal lost. 1 step = 0.1sec - 1sec in example
#define FAILSAFE_DELAY     10
// Time for Landing before motors stop in 0.1sec. 1 step = 0.1sec - 20sec in example
#define FAILSAFE_OFF_DELAY 200
// (*) Throttle level used for landing - may be relative to MINTHROTTLE - as in this case
#define FAILSAFE_THROTTLE  (MINTHROTTLE + 200)
#define FAILSAFE_DETECT_TRESHOLD  985


/* defines the neutral zone of throttle stick during altitude hold, default setting is
 +/-50 uncommend and change the value below if you want to change it. */
#define ALT_HOLD_THROTTLE_NEUTRAL_ZONE    50

// in us    - if uncommented, this value is used in ALT_HOLD for throttle stick middle point
//instead of initialThrottleHold parameter.
//#define ALT_HOLD_THROTTLE_MIDPOINT        1500

#define MIDRC 1500
#define ESC_CALIB_LOW  MINCOMMAND
#define ESC_CALIB_HIGH 2000


#define MAG_DECLINATION  4.02f   //(**)

#endif /* CONFIG_H_ */

