#include "pid_mw.h"

Pid_mw::Pid_mw(pidProfile_t* cfg)
{
    //TODO
    cfg->P[ROLL] = 33;
    cfg->I[ROLL] = 30;
    cfg->D[ROLL] = 23;

    cfg->P[PITCH] = 33;
    cfg->I[PITCH] = 30;
    cfg->D[PITCH] = 23;

    cfg->P[YAW] = 68;
    cfg->I[YAW] = 45;
    cfg->D[YAW] = 0;

    cfg->pidLevel_P = 90;
    cfg->pidLevel_I = 10;
    cfg->pidLevel_D = 100;

    cfg->yawRate = 0;

    //TODO not used
    cfg->rollPitchRate[0] = 0;
    cfg->rollPitchRate[1] = 0;

    this->cfg = cfg;

    //500 is hardcoded in the pid function
    //max_angle_inclination = 500;    // 50 degrees
}


void Pid_mw::pid_mw(int16_t* rcCommand, int16_t* gyroADC, int16_t* attitude, FLIGHT_MODE flightMode, int16_t* axisPID)
{
    static int32_t errorGyroI_YAW;
    static int16_t delta1[2],delta2[2];
    static int16_t errorGyroI[2] = {0,0};
    static int16_t errorAngleI[2] = {0,0};
    static int16_t lastGyro[2] = {0,0};

    int32_t prop = 0;
    int16_t rc;
    int16_t error,errorAngle;
    int16_t PTerm = 0,ITerm = 0,DTerm, PTermACC, ITermACC;
    int16_t delta;

    if (flightMode == HORIZON_MODE )
    {
        prop = min(max(abs(rcCommand[PITCH]),abs(rcCommand[ROLL])),512);
    }

    // PITCH & ROLL
    for(uint8_t axis=0; axis < 2; axis++)
    {
      rc = rcCommand[axis]<<1;
      error = rc - gyroADC[axis];
      errorGyroI[axis]  = constrain(errorGyroI[axis]+error,-16000,+16000);       // WindUp   16 bits is ok here
      if (abs(gyroADC[axis])>640) errorGyroI[axis] = 0;

      ITerm = (errorGyroI[axis]>>7) * cfg->I[axis]>>6;                        // 16 bits is ok here 16000/125 = 128 ; 128*250 = 32000

      PTerm = (rc * cfg->P[axis])>>6;

      if (flightMode == ANGLE_MODE || flightMode == HORIZON_MODE)
      { // axis relying on ACC
          // 50 degrees max inclination
#ifdef GPS
          errorAngle         = constrain(rc + GPS_angle[axis],-500,+500) - attitude[axis] + rollPitchTrim[axis]; //16 bits is ok here
#else
          errorAngle         = constrain(rc,-500,+500) - attitude[axis] + rollPitchTrim[axis]; //16 bits is ok here
#endif
          // WindUp     //16 bits is ok here
          errorAngleI[axis]  = constrain(errorAngleI[axis]+errorAngle,-10000,+10000);

          // 32 bits is needed for calculation: errorAngle*P8 could exceed 32768   16 bits is ok for result
          PTermACC           = (errorAngle * cfg->pidLevel_P)>>7;

          int16_t limit      = cfg->pidLevel_D*5;
          PTermACC           = constrain(PTermACC,-limit,+limit);

          // 32 bits is needed for calculation:10000*I8 could exceed 32768   16 bits is ok for result
          ITermACC           = (errorAngleI[axis] *  cfg->pidLevel_I)>>12;

          ITerm              = ITermACC + ((ITerm-ITermACC)*prop>>9);
          PTerm              = PTermACC + ((PTerm-PTermACC)*prop>>9);
      }

      // 32 bits is needed for calculation
      PTerm -= (gyroADC[axis] * dynP8[axis])>>6;

      // 16 bits is ok here, the dif between 2 consecutive gyro reads is limited to 800
      delta          = gyroADC[axis] - lastGyro[axis];
      lastGyro[axis] = gyroADC[axis];
      DTerm          = delta1[axis]+delta2[axis]+delta;
      delta2[axis]   = delta1[axis];
      delta1[axis]   = delta;

      DTerm = (DTerm * dynD8[axis])>>5;        // 32 bits is needed for calculation

      axisPID[axis] =  PTerm + ITerm - DTerm;
    }

    //YAW
    #define GYRO_P_MAX 300
    #define GYRO_I_MAX 250

    rc = (rcCommand[YAW] * (2*cfg->yawRate + 30))  >> 5;

    error = rc - gyroADC[YAW];
    errorGyroI_YAW  += (error * cfg->I[YAW]);
    errorGyroI_YAW  = constrain(errorGyroI_YAW, 2-((int32_t)1<<28), -2+((int32_t)1<<28));

    if (abs(rc) > 50)
    {
        errorGyroI_YAW = 0;
    }

    PTerm =(error * cfg->P[YAW])>>6;
    int16_t limit = GYRO_P_MAX - cfg->D[YAW];
    PTerm = constrain(PTerm,-limit,+limit);

    ITerm = constrain((int16_t)(errorGyroI_YAW>>13),-GYRO_I_MAX,+GYRO_I_MAX);

    axisPID[YAW] =  PTerm + ITerm;


}
