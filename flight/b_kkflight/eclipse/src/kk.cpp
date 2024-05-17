#include "gyros.h"
#include "settings.h"
#include "receiver.h"
#include "motors.h"

bool Armed;

static int16_t integral[3];          // PID integral term
static int16_t last_error[3];        // Last proportional error

static void setup(void);
static void loop(void);
int main(void);

static void setup()
{

  //receiverSetup();
  gyrosSetup();
  //motorsSetup();
  //settingsSetup();

  Armed = false;

  //_delay_ms(1500);

  ReadGainPots();
  ReadGainPots();

  bool pitchMin = (GainInADC[PITCH] < (ADC_MAX * 5) / 100);    // 5% threshold
  bool rollMin =  (GainInADC[ROLL]  < (ADC_MAX * 5) / 100);    // 5% threshold
  bool yawMin =   (GainInADC[YAW]   < (ADC_MAX * 5) / 100);    // 5% threshold

  if(pitchMin && rollMin && yawMin) 
  { 
    //settingsClearAll();
  }             
  else if(pitchMin && yawMin)     
  { 
    motorsIdentify(); 
  }   
  else if(pitchMin)   
  { 
    //receiverStickCenter();
  }          
  else if(rollMin) 
  { 
    //gyrosReverse();
  }                 
  else if(yawMin)  
  { 
    motorsThrottleCalibration(); 
  }    
}

static inline void loop()
{
  int16_t error, emax = 1023;
  int16_t imax, derivative;

  RxGetChannels();

  //arming logic

  //right after arming calibrate gyros
  CalibrateGyros();

  ReadGyros();

  gyroADC[ROLL]-= gyroZero[ROLL];
  gyroADC[PITCH]-= gyroZero[PITCH];
  gyroADC[YAW]-= gyroZero[YAW];

  if(RxInCollective > MAX_COLLECTIVE)
    RxInCollective = MAX_COLLECTIVE;

  MotorOut1 = RxInCollective;
  MotorOut2 = RxInCollective;
  MotorOut3 = RxInCollective;
  MotorOut4 = RxInCollective;

  imax = RxInCollective;
  if(imax < 0)
    imax = 0;

  RxInRoll = ((int32_t)RxInRoll * (uint32_t)GainInADC[ROLL]) >> STICK_GAIN_SHIFT;

  gyroADC[ROLL] = ((int32_t)gyroADC[ROLL] * (uint32_t)GainInADC[ROLL]) >> GYRO_GAIN_SHIFT;
  gyroADC[ROLL] = -gyroADC[ROLL];

  if(Armed) {
    if(0) {
      error = RxInRoll - gyroADC[ROLL];
      if(error > emax)
        error = emax;
      else if(error < -emax)
        error = -emax;
      integral[ROLL]+= error;
      if(integral[ROLL] > imax)
        integral[ROLL] = imax;
      else if(integral[ROLL] < -imax)
        integral[ROLL] = -imax;
      derivative = error - last_error[ROLL];
      last_error[ROLL] = error;
      RxInRoll+= error + (integral[ROLL] >> 2) + (derivative >> 2);
    } else {
      RxInRoll-= gyroADC[ROLL];
    }
  }

  RxInRoll = RxInRoll >> 1;
  MotorOut1+= RxInRoll;
  MotorOut2-= RxInRoll;
  MotorOut3-= RxInRoll;
  MotorOut4+= RxInRoll;

  RxInPitch = ((int32_t)RxInPitch * (uint32_t)GainInADC[PITCH]) >> STICK_GAIN_SHIFT;
  gyroADC[PITCH] = ((int32_t)gyroADC[PITCH] * (uint32_t)GainInADC[PITCH]) >> GYRO_GAIN_SHIFT;

  gyroADC[PITCH] = -gyroADC[PITCH];

  if(Armed) {
    if(0) {
      error = RxInPitch - gyroADC[PITCH];
      if(error > emax)
        error = emax;
      else if(error < -emax)
        error = -emax;
      integral[PITCH]+= error;
      if(integral[PITCH] > imax)
        integral[PITCH] = imax;
      else if(integral[PITCH] < -imax)
        integral[PITCH] = -imax;
      derivative = error - last_error[PITCH];
      last_error[PITCH] = error;
      RxInPitch+= error + (integral[PITCH] >> 2) + (derivative >> 2);
    } else {
      RxInPitch-= gyroADC[PITCH];
    }
  }

  RxInPitch = (RxInPitch >> 1);      // cosine of 60
  MotorOut1+= RxInPitch;
  MotorOut2+= RxInPitch;
  MotorOut3-= RxInPitch;
  MotorOut4-= RxInPitch;

  /* Calculate yaw output - Test without props!! */

  RxInYaw = ((int32_t)RxInYaw * (uint32_t)GainInADC[YAW]) >> STICK_GAIN_SHIFT;
  gyroADC[YAW] = ((int32_t)gyroADC[YAW] * (uint32_t)GainInADC[YAW]) >> GYRO_GAIN_SHIFT;
  gyroADC[YAW] = -gyroADC[YAW];

  if(Armed) {
    error = RxInYaw - gyroADC[YAW];
    if(error > emax)
      error = emax;
    else if(error < -emax)
      error = -emax;
    integral[YAW]+= error;
    if(integral[YAW] > imax)
      integral[YAW] = imax;
    else if(integral[YAW] < -imax)
      integral[YAW] = -imax;
    derivative = error - last_error[YAW];
    last_error[YAW] = error;
    RxInYaw+= error + (integral[YAW] >> 4) + (derivative >> 4);
  }
  MotorOut1-= RxInYaw;
  MotorOut2+= RxInYaw;
  MotorOut3-= RxInYaw;
  MotorOut4+= RxInYaw;

  imax = 114;
  //--- Limit the lowest value to avoid stopping of motor if motor value is under-saturated ---
  if(MotorOut1 < imax)
    MotorOut1 = imax;  // this is the motor idle level
  if(MotorOut2 < imax)
    MotorOut2 = imax;
  if(MotorOut3 < imax)
    MotorOut3 = imax;
  if(MotorOut4 < imax)
    MotorOut4 = imax;

  //--- Output to motor ESC's ---
  if(RxInCollective < 1 || !Armed) 
  {
    /* turn off motors unless armed and collective is non-zero */
    MotorOut1 = 0;
    MotorOut2 = 0;
    MotorOut3 = 0;
    MotorOut4 = 0;
  }

  output_motor_ppm();
}

int main()
{
  setup();

  while(1)
    loop();
  return 1;
}
