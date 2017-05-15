#ifndef CONFIG_H_
#define CONFIG_H_

#define QUADX

/****************************    Motor minthrottle    *******************************/
/* Set the minimum throttle command sent to the ESC (Electronic Speed Controller)
 This is the minimum value that allow motors to run at a idle speed  */
#define MINTHROTTLE 1150 // (*) (**)
/****************************    Motor maxthrottle    *******************************/
/* this is the maximum value for the ESCs at full power, this value can be increased up to 2000 */
#define MAXTHROTTLE 1850

/****************************    Mincommand          *******************************/
/* this is the value for the ESCs when they are not armed
 in some cases, this value must be lowered down to 900 for some specific ESCs, otherwise they failed to initiate */
#define MINCOMMAND  1000

/**********************************  constant loop time  ******************************/
#define LOOP_TIME 2800

//#define MPU6050       //combo + ACC
//#define BMP085
//#define HMC5843
//#define HMC5883
//#define AK8975

/********************************  PID Controller *********************************/
/* choose one of the alternate PID control algorithms
 * 1 = evolved oldschool algorithm (similar to v2.2)
 * 2 = new experimental algorithm from Alex Khoroshko - unsupported - http://www.multiwii.com/forum/viewtopic.php?f=8&t=3671&start=10#p37387
 * */
#define PID_CONTROLLER 1

/* NEW: not used anymore for servo coptertypes  <== NEEDS FIXING - MOVE TO WIKI */
#define YAW_DIRECTION 1
//#define YAW_DIRECTION -1 // if you want to reverse the yaw correction direction

#define ONLYARMWHENFLAT //prevent the copter from arming when the copter is tilted

/********************************    ARM/DISARM    *********************************/
/* optionally disable stick combinations to arm/disarm the motors.
 * In most cases one of the two options to arm/disarm via TX stick is sufficient */
#define ALLOW_ARM_DISARM_VIA_TX_YAW
//#define ALLOW_ARM_DISARM_VIA_TX_ROLL


/* Limit Maximum controll for Roll & Nick  in 0-100% */
#define CONTROL_RANGE   { 100, 100 }      //  { ROLL,PITCH }


//#define GYRO_LPF_256HZ     // This is the default setting, no need to uncomment, just for reference
//#define GYRO_LPF_188HZ
//#define GYRO_LPF_98HZ
//#define GYRO_LPF_42HZ
//#define GYRO_LPF_20HZ
//#define GYRO_LPF_10HZ
//#define GYRO_LPF_5HZ       // Use this only in extreme cases, rather change motors and/or props -- setting not available on ITG3200


//#define THROTTLE_ANGLE_CORRECTION 40

/*** HEADFREE : the copter can be controled by an absolute stick orientation, whatever the yaw orientation ***/
//#define HEADFREE
/*************************        Advanced Headfree Mode             ********************/
/* In Advanced Headfree mode when the copter is farther than ADV_HEADFREE_RANGE meters then
 the  bearing between home and copter position will become the control direction
 IF copter come closer than ADV_HEADFREE_RANGE meters, then the control direction freezed to the
 bearing between home and copter at the point where it crosses the ADV_HEADFREE_RANGE meter distance
 first implementation by HAdrian, mods by EOSBandi
 */

//#define ADVANCED_HEADFREE      //Advanced headfree mode is enabled when this is uncommented
//#define ADV_HEADFREE_RANGE 15  //Range where advanced headfree mode activated

//#define GYROCALIBRATIONFAILSAFE


/********                          Failsafe settings                 ********************/
/* Failsafe check pulses on four main control channels CH1-CH4. If the pulse is missing or bellow 985us (on any of these four channels)
 the failsafe procedure is initiated. After FAILSAFE_DELAY time from failsafe detection, the level mode is on (if ACC is avaliable),
 PITCH, ROLL and YAW is centered and THROTTLE is set to FAILSAFE_THROTTLE value. You must set this value to descending about 1m/s or so
 for best results. This value is depended from your configuration, AUW and some other params.  Next, after FAILSAFE_OFF_DELAY the copter is disarmed,
 and motors is stopped. If RC pulse coming back before reached FAILSAFE_OFF_DELAY time, after the small quard time the RC control is returned to normal. */
//#define FAILSAFE                                // uncomment  to activate the failsafe function
#define FAILSAFE_DELAY     10                     // Guard time for failsafe activation after signal lost. 1 step = 0.1sec - 1sec in example
#define FAILSAFE_OFF_DELAY 200                    // Time for Landing before motors stop in 0.1sec. 1 step = 0.1sec - 20sec in example
#define FAILSAFE_THROTTLE  (MINTHROTTLE + 200)    // (*) Throttle level used for landing - may be relative to MINTHROTTLE - as in this case

#define FAILSAFE_DETECT_TRESHOLD  985


/**************************************************************************************/
/***********************                  GPS                **************************/
/**************************************************************************************/

/* ENable this for using GPS simulator (NMEA only)*/
//#define GPS_SIMULATOR
/* GPS using a SERIAL port
 if enabled, define here the Arduino Serial port number and the UART speed
 note: only the RX PIN is used in case of NMEA mode, the GPS is not configured by multiwii
 in NMEA mode the GPS must be configured to output GGA and RMC NMEA sentences (which is generally the default conf for most GPS devices)
 at least 5Hz update rate. uncomment the first line to select the GPS serial port of the arduino */

//#define GPS_SERIAL 2         // should be 2 for flyduino v2. It's the serial port number on arduino MEGA
// must be 0 for PRO_MINI (ex GPS_PRO_MINI)
// note: Now a GPS can share MSP on the same port. The only constrain is to not use it simultaneously, and use the same port speed.
// avoid using 115200 baud because with 16MHz arduino the 115200 baudrate have more than 2% speed error (57600 have 0.8% error)
#define GPS_BAUD   57600       // GPS_BAUD will override SERIALx_COM_SPEED for the selected port

/* GPS protocol
 NMEA  - Standard NMEA protocol GGA, GSA and RMC  sentences are needed
 UBLOX - U-Blox binary protocol, use the ublox config file (u-blox-config.ublox.txt) from the source tree
 MTK_BINARY16 and MTK_BINARY19 - MTK3329 chipset based GPS with DIYDrones binary firmware (v1.6 or v1.9)
 With UBLOX and MTK_BINARY you don't have to use GPS_FILTERING in multiwii code !!! */

//#define NMEA
//#define UBLOX
//#define MTK_BINARY16
//#define MTK_BINARY19
//#define INIT_MTK_GPS        // initialize MTK GPS for using selected speed, 5Hz update rate and GGA & RMC sentence or binary settings
//#define VENUS8
/* I2C GPS device made with an independant arduino + GPS device
 including some navigation functions
 contribution from EOSBandi   http://code.google.com/p/i2c-gps-nav/
 You have to use at least I2CGpsNav code r33 */
/* all functionnalities allowed by SERIAL_GPS are now available for I2C_GPS: all relevant navigation computations are gathered in the main FC */

//#define I2C_GPS
// If your I2C GPS board has Sonar support enabled
//#define I2C_GPS_SONAR
/* indicate a valid GPS fix with at least 5 satellites by flashing the LED  - Modified by MIS - Using stable LED (YELLOW on CRIUS AIO) led work as sat number indicator
 - No GPS FIX -> LED blink at speed of incoming GPS frames
 - Fix and sat no. bellow 5 -> LED off
 - Fix and sat no. >= 5 -> LED blinks, one blink for 5 sat, two blinks for 6 sat, three for 7 ... */
#define GPS_LED_INDICATOR

//Enables the MSP_WP command set , which is used by WinGUI for displaying an setting up navigation
//#define USE_MSP_WP

// HOME position is reset at every arm, uncomment it to prohibit it (you can set home position with GyroCalibration)
//#define DONT_RESET_HOME_AT_ARM

/* GPS navigation can control the heading */

// copter faces toward the navigation point, maghold must be enabled for it
#define NAV_CONTROLS_HEADING       1    //(**)
// true - copter comes in with tail first
#define NAV_TAIL_FIRST             0    //(**)
// true - when copter arrives to home position it rotates it's head to takeoff direction
#define NAV_SET_TAKEOFF_HEADING    1    //(**)

/* Get your magnetic declination from here : http://magnetic-declination.com/
 Convert the degree+minutes into decimal degree by ==> degree+minutes*(1/60)
 Note the sign on declination it could be negative or positive (WEST or EAST)
 Also note, that maqgnetic declination changes with time, so recheck your value every 3-6 months */
#define MAG_DECLINATION  4.02f   //(**)

// Adds a forward predictive filterig to compensate gps lag. Code based on Jason Short's lead filter implementation
#define GPS_LEAD_FILTER               //(**)

// add a 5 element moving average filter to GPS coordinates, helps eliminate gps noise but adds latency comment out to disable
// use it with NMEA gps only 
//#define GPS_FILTERING                 //(**)

// if we are within this distance to a waypoint then we consider it reached (distance is in cm)
#define GPS_WP_RADIUS              100      //(**)

// Safe WP distance, do not start mission if the first wp distance is larger than this number (in meters)
// Also aborts mission if the next waypoint distance is more than this number
#define SAFE_WP_DISTANCE           500      //(**)

//Maximu allowable navigation altitude (in meters) automatic altitude control will not go above this height
#define MAX_NAV_ALTITUDE           100     //(**)

// minimum speed when approach waypoint
#define NAV_SPEED_MIN              100    // cm/sec //(**)
// maximum speed to reach between waypoints
#define NAV_SPEED_MAX              400    // cm/sec //(**)
// Slow down to zero when reaching waypoint (same as NAV_SPEED_MIN = 0)
#define NAV_SLOW_NAV               0      //(**)
// Weight factor of the crosstrack error in navigation calculations (do not touch)
#define CROSSTRACK_GAIN            .4     //(**)
// Maximum allowable banking than navigation outputs
#define NAV_BANK_MAX 3000                 //(**)

//Defines the RTH altitude. 0 means keep current alt during RTH (in meters)
#define RTH_ALTITUDE               15        //(**)
//Wait to reach RTH alt before start moving to home (0-no, 1-yes)
#define WAIT_FOR_RTH_ALT           1         //(**)

//Navigation engine will takeover BARO mode control
#define NAV_TAKEOVER_BARO          1         //(**)

//Throttle stick input will be ignored  (only in BARO)
#define IGNORE_THROTTLE            1         //(**)

//If FENCE DISTANCE is larger than 0 then copter will switch to RTH when it farther from home
//than the defined number in meters
#define FENCE_DISTANCE      600

//This governs the descent speed during landing. 100 is equals approc 50cm/sec
#define LAND_SPEED          100

//#define ONLY_ALLOW_ARM_WITH_GPS_3DFIX      // Only allow FC arming if GPS has a 3D fix.


/********************************************************************/
/****           altitude hold                                    ****/
/********************************************************************/

/* defines the neutral zone of throttle stick during altitude hold, default setting is
 +/-50 uncommend and change the value below if you want to change it. */
#define ALT_HOLD_THROTTLE_NEUTRAL_ZONE    50
//#define ALT_HOLD_THROTTLE_MIDPOINT        1500  // in us    - if uncommented, this value is used in ALT_HOLD for throttle stick middle point instead of initialThrottleHold parameter.

/* uncomment to disable the altitude hold feature.
 * This is useful if all of the following apply
 * + you have a baro
 * + want altitude readout and/or variometer
 * + do not use altitude hold feature
 * + want to save memory space */
//#define SUPPRESS_BARO_ALTHOLD


/**************************************************************************************/
/***********************     motor, servo and other presets     ***********************/
/**************************************************************************************/
/* motors will not spin when the throttle command is in low position
 this is an alternative method to stop immediately the motors */
//#define MOTOR_STOP
/* some radios have not a neutral point centered on 1500. can be changed here */
#define MIDRC 1500



#define ESC_CALIB_LOW  MINCOMMAND
#define ESC_CALIB_HIGH 2000
//#define ESC_CALIB_CANNOT_FLY  // uncomment to activate


#endif /* CONFIG_H_ */

