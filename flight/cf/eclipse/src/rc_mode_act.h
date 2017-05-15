#ifndef _RC_MODE_ACT_H
#define _RC_MODE_ACT_H
#include <stdint.h>

typedef enum {
//    BOXARM = 0,  //Enables motors and flight stabilisation
    BOXANGLE,    //Legacy auto-level flight mode
    BOXHORIZON,  //Auto-level flight mode
    //BOXBARO,     //Altitude hold mode (Requires barometer sensor)
    BOXMAG,      //Heading lock
    BOXHEADFREE, //Head Free - When enabled yaw has no effect on pitch/roll inputs
    BOXHEADADJ,  //Heading Adjust - Sets a new yaw origin for HEADFREE mode
    //BOXCAMSTAB,  //   Camera Stabilisation
//    BOXCAMTRIG,
    BOXGPSHOME,   //Autonomous flight to HOME position
    BOXGPSHOLD,   //Maintain the same longitude/lattitude
    //BOXPASSTHRU,  //Pass roll, yaw, and pitch directly from rx to servos in airplane mix
    //BOXBEEPERON,  //Enable beeping - useful for locating a crashed aircraft
    //BOXLEDMAX,
    //BOXLEDLOW,
    //BOXLLIGHTS,
    //BOXCALIB,
    //BOXOSD,        //Enable/Disable On-Screen-Display (OSD)
    //BOXTELEMETRY, //Enable telemetry via switch
    //BOXGTUNE,     //G-Tune - auto tuning of Pitch/Roll/Yaw P values
//    BOXSONAR,    //Altitude hold mode (sonar sensor only)
//    BOXSERVO1,
//    BOXSERVO2,
//    BOXSERVO3,
    //BOXBLACKBOX,  //Enable BlackBox logging
    BOXFAILSAFE,   //Enter failsafe stage 2 manually
    //BOXAIRMODE,    //Alternative mixer and additional PID logic for more stable copter
    CHECKBOX_ITEM_COUNT
} boxId_e;




#endif


