#include "config.h"
#include "def.h"
#include "types.h"
#include "MultiWii.h"
#include "EEPROM.h"
#include "IMU.h"
#include "Sensors.h"
#include "utils.h"
#include "debug.h"

static void Device_Mag_getADC();
static void Baro_init();
static void Mag_init();
static void ACC_init();

// ************************************************************************************************************
// board orientation and setup
// ************************************************************************************************************
//default board orientation
#if !defined(ACC_ORIENTATION) 
  #define ACC_ORIENTATION(X, Y, Z)  {imu.accADC[ROLL]  = X; imu.accADC[PITCH]  = Y; imu.accADC[YAW]  = Z;}
#endif
#if !defined(GYRO_ORIENTATION) 
  #define GYRO_ORIENTATION(X, Y, Z) {imu.gyroADC[ROLL] = X; imu.gyroADC[PITCH] = Y; imu.gyroADC[YAW] = Z;}
#endif
#if !defined(MAG_ORIENTATION) 
  #define MAG_ORIENTATION(X, Y, Z)  {imu.magADC[ROLL]  = X; imu.magADC[PITCH]  = Y; imu.magADC[YAW]  = Z;}
#endif



#define GYRO_DLPF_CFG   0  //256
//#define GYRO_DLPF_CFG   1 //188
//#define GYRO_DLPF_CFG   2 //98
//#define GYRO_DLPF_CFG   3 //42
//#define GYRO_DLPF_CFG   4 //20
//#define GYRO_DLPF_CFG   5 //10
//#define GYRO_DLPF_CFG   6 //5


static uint8_t rawADC[6];
#if defined(WMP)
static uint32_t neutralizeTime = 0;
#endif


void i2c_read_reg_to_buf(uint8_t add, uint8_t reg, uint8_t *buf, uint8_t size) {
}

void i2c_getSixRawADC(uint8_t add, uint8_t reg) {
  i2c_read_reg_to_buf(add, reg, rawADC, 6);
}

void i2c_writeReg(uint8_t add, uint8_t reg, uint8_t val) {
}

uint8_t i2c_readReg(uint8_t add, uint8_t reg) {
  uint8_t val;
  i2c_read_reg_to_buf(add, reg, &val, 1);
  return val;
}

void GYRO_Common()
{
  static int16_t previousGyroADC[3] = {0,0,0};
  static int32_t g[3];
  uint8_t axis, tilt=0;


  if (calibratingG>0)
  {
    for (axis = 0; axis < 3; axis++)
    {
      if (calibratingG == 512) { // Reset g[axis] at start of calibration
        g[axis]=0;
      }
      g[axis] +=imu.gyroADC[axis]; // Sum up 512 readings
      gyroZero[axis]=g[axis]>>9;
      if (calibratingG == 1)
      {
          //TODO buzzer
      }
    }

    calibratingG--;
  }

  for (axis = 0; axis < 3; axis++)
  {
      imu.gyroADC[axis]  -= gyroZero[axis];
      //anti gyro glitch, limit the variation between two consecutive readings
      imu.gyroADC[axis] = constrain(imu.gyroADC[axis],previousGyroADC[axis]-800,previousGyroADC[axis]+800);
      previousGyroADC[axis] = imu.gyroADC[axis];
  }

}

void ACC_Common()
{
  static int32_t a[3];

  if (calibratingA>0)
  {
    calibratingA--;
    for (uint8_t axis = 0; axis < 3; axis++)
    {
      if (calibratingA == 511)
          a[axis]=0;   // Reset a[axis] at start of calibration

      a[axis] +=imu.accADC[axis];           // Sum up 512 readings
      global_conf.accZero[axis] = a[axis]>>9; // Calculate average, only the last itteration where (calibratingA == 0) is relevant
    }
    if (calibratingA == 0)
    {
      global_conf.accZero[YAW] -= ACC_1G;   // shift Z down by ACC_1G and store values in EEPROM at end of calibration
      conf.angleTrim[ROLL]   = 0;
      conf.angleTrim[PITCH]  = 0;
      writeGlobalSet(1); // write accZero in EEPROM
    }
  }
  imu.accADC[ROLL]  -=  global_conf.accZero[ROLL] ;
  imu.accADC[PITCH] -=  global_conf.accZero[PITCH];
  imu.accADC[YAW]   -=  global_conf.accZero[YAW] ;
}

static void Baro_Common()
{
  static int32_t baroHistTab[BARO_TAB_SIZE];
  static uint8_t baroHistIdx;

  uint8_t indexplus1 = (baroHistIdx + 1);
  if (indexplus1 == BARO_TAB_SIZE)
      indexplus1 = 0;

  baroHistTab[baroHistIdx] = baroPressure;
  baroPressureSum += baroHistTab[baroHistIdx];
  baroPressureSum -= baroHistTab[indexplus1];
  baroHistIdx = indexplus1;
}

// ************************************************************************************************************
// I2C Barometer BOSCH BMP085
// ************************************************************************************************************
// I2C adress: 0x77 (7bit)
// principle:
//  1) read the calibration register (only once at the initialization)
//  2) read uncompensated temperature (not mandatory at every cycle)
//  3) read uncompensated pressure
//  4) raw temp + raw pressure => calculation of the adjusted pressure
//  the following code uses the maximum precision setting (oversampling setting 3)
// ************************************************************************************************************

#if defined(BMP085)
#define BMP085_ADDRESS 0x77

static struct
{
  // sensor registers from the BOSCH BMP085 datasheet
  int16_t  ac1, ac2, ac3;
  uint16_t ac4, ac5, ac6;
  int16_t  b1, b2, mb, mc, md;
  union {uint16_t val; uint8_t raw[2]; } ut; //uncompensated T
  union {uint32_t val; uint8_t raw[4]; } up; //uncompensated P
  uint8_t  state;
  uint32_t deadline;
} bmp085_ctx;  
#define OSS 3

/* transform a series of bytes from big endian to little
   endian and vice versa. */
void swap_endianness(void *buf, size_t size) {
  /* we swap in-place, so we only have to
  * place _one_ element on a temporary tray
  */
  uint8_t tray;
  uint8_t *from;
  uint8_t *to;
  /* keep swapping until the pointers have assed each other */
  for (from = (uint8_t*)buf, to = &from[size-1]; from < to; from++, to--) {
    tray = *from;
    *from = *to;
    *to = tray;
  }
}

void i2c_BMP085_readCalibration(){
  delay(10);
  //read calibration data in one go
  size_t s_bytes = (uint8_t*)&bmp085_ctx.md - (uint8_t*)&bmp085_ctx.ac1 + sizeof(bmp085_ctx.ac1);
  i2c_read_reg_to_buf(BMP085_ADDRESS, 0xAA, (uint8_t*)&bmp085_ctx.ac1, s_bytes);
  // now fix endianness
  int16_t *p;
  for (p = &bmp085_ctx.ac1; p <= &bmp085_ctx.md; p++) {
    swap_endianness(p, sizeof(*p));
  }
}

// read uncompensated temperature value: send command first
void i2c_BMP085_UT_Start(void) {
//  i2c_writeReg(BMP085_ADDRESS,0xf4,0x2e);
//  i2c_rep_start(BMP085_ADDRESS<<1);
//  i2c_write(0xF6);
//  i2c_stop();
}

// read uncompensated pressure value: send command first
void i2c_BMP085_UP_Start () {
//  i2c_writeReg(BMP085_ADDRESS,0xf4,0x34+(OSS<<6)); // control register value for oversampling setting 3
//  i2c_rep_start(BMP085_ADDRESS<<1); //I2C write direction => 0
//  i2c_write(0xF6);
//  i2c_stop();
}

// read uncompensated pressure value: read result bytes
// the datasheet suggests a delay of 25.5 ms (oversampling settings 3) after the send command
void i2c_BMP085_UP_Read () {
//  i2c_rep_start((BMP085_ADDRESS<<1) | 1);//I2C read direction => 1
//  bmp085_ctx.up.raw[2] = i2c_readAck();
//  bmp085_ctx.up.raw[1] = i2c_readAck();
//  bmp085_ctx.up.raw[0] = i2c_readNak();
}

// read uncompensated temperature value: read result bytes
// the datasheet suggests a delay of 4.5 ms after the send command
void i2c_BMP085_UT_Read() {
//  i2c_rep_start((BMP085_ADDRESS<<1) | 1);//I2C read direction => 1
//  bmp085_ctx.ut.raw[1] = i2c_readAck();
//  bmp085_ctx.ut.raw[0] = i2c_readNak();
}

void i2c_BMP085_Calculate() {
  int32_t  x1, x2, x3, b3, b5, b6, p, tmp;
  uint32_t b4, b7;
  // Temperature calculations
  x1 = ((int32_t)bmp085_ctx.ut.val - bmp085_ctx.ac6) * bmp085_ctx.ac5 >> 15;
  x2 = ((int32_t)bmp085_ctx.mc << 11) / (x1 + bmp085_ctx.md);
  b5 = x1 + x2;
  baroTemperature = (b5 * 10 + 8) >> 4; // in 0.01 degC (same as MS561101BA temperature)
  // Pressure calculations
  b6 = b5 - 4000;
  x1 = (bmp085_ctx.b2 * (b6 * b6 >> 12)) >> 11; 
  x2 = bmp085_ctx.ac2 * b6 >> 11;
  x3 = x1 + x2;
  tmp = bmp085_ctx.ac1;
  tmp = (tmp*4 + x3) << OSS;
  b3 = (tmp+2)/4;
  x1 = bmp085_ctx.ac3 * b6 >> 13;
  x2 = (bmp085_ctx.b1 * (b6 * b6 >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (bmp085_ctx.ac4 * (uint32_t)(x3 + 32768)) >> 15;
  b7 = ((uint32_t) (bmp085_ctx.up.val >> (8-OSS)) - b3) * (50000 >> OSS);
  p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  baroPressure = p + ((x1 + x2 + 3791) >> 4);
}

void  Baro_init() {
  delay(10);
  i2c_BMP085_readCalibration();
  delay(5);
  i2c_BMP085_UT_Start(); 
  bmp085_ctx.deadline = currentTime+5000;
}

//return 0: no data available, no computation ;  1: new value available  ; 2: no new value, but computation time
uint8_t Baro_update()
{                   // first UT conversion is started in init procedure
  if (currentTime < bmp085_ctx.deadline)
      return 0;

  bmp085_ctx.deadline = currentTime+6000; // 1.5ms margin according to the spec (4.5ms T convetion time)

  if (bmp085_ctx.state == 0)
  {
    i2c_BMP085_UT_Read(); 
    i2c_BMP085_UP_Start(); 
    bmp085_ctx.state = 1; 
    Baro_Common();
    bmp085_ctx.deadline += 21000;   // 6000+21000=27000 1.5ms margin according to the spec (25.5ms P convetion time with OSS=3)
    return 1;
  }
  else
  {
    i2c_BMP085_UP_Read(); 
    i2c_BMP085_UT_Start(); 
    i2c_BMP085_Calculate(); 
    bmp085_ctx.state = 0; 
    return 2;
  }
}
#endif




// ************************************************************************************************************
// I2C Compass common function
// ************************************************************************************************************
static float magGain[3] = {1.0,1.0,1.0};  // gain for each axis, populated at sensor init

uint8_t Mag_getADC() { // return 1 when news values are available, 0 otherwise
  static uint32_t t,tCal = 0;
  static int16_t magZeroTempMin[3],magZeroTempMax[3];
  uint8_t axis;

  if ( currentTime < t ) return 0; //each read is spaced by 100ms
  t = currentTime + 100000;
  Device_Mag_getADC();

  for(axis=0;axis<3;axis++) {
    imu.magADC[axis]  = imu.magADC[axis]  * magGain[axis];
    if (!f.CALIBRATE_MAG) imu.magADC[axis]  -= global_conf.magZero[axis];
  }
  
  if (f.CALIBRATE_MAG) {
    if (tCal == 0) // init mag calibration
      tCal = t;
    if ((t - tCal) < 30000000)
    { // 30s: you have 30s to turn the multi in all directions
      //LEDPIN_TOGGLE;
      for(axis=0;axis<3;axis++) {
        if(tCal == t) { // it happens only in the first step, initialize the zero
          magZeroTempMin[axis] = imu.magADC[axis];
          magZeroTempMax[axis] = imu.magADC[axis];
        }
        if (imu.magADC[axis] < magZeroTempMin[axis])
        {
            magZeroTempMin[axis] = imu.magADC[axis];
        }
        if (imu.magADC[axis] > magZeroTempMax[axis])
        {
            magZeroTempMax[axis] = imu.magADC[axis];
        }
        global_conf.magZero[axis] = (magZeroTempMin[axis] + magZeroTempMax[axis])>>1;
      }
    } else {
      f.CALIBRATE_MAG = 0;
      tCal = 0;
      writeGlobalSet(1);
    }
  }

  return 1;
}



// ************************************************************************************************************
// I2C Compass AK8975
// ************************************************************************************************************
// I2C adress: 0x0C (7bit)
// ************************************************************************************************************
#if defined(AK8975)
  #define MAG_ADDRESS 0x0C
  #define MAG_DATA_REGISTER 0x03
  
  void Mag_init() {
    delay(100);
    i2c_writeReg(MAG_ADDRESS,0x0a,0x01);  //Start the first conversion
    delay(100);
  }

  void Device_Mag_getADC() {
    i2c_getSixRawADC(MAG_ADDRESS,MAG_DATA_REGISTER);
    MAG_ORIENTATION( ((rawADC[1]<<8) | rawADC[0]) ,          
                     ((rawADC[3]<<8) | rawADC[2]) ,     
                     ((rawADC[5]<<8) | rawADC[4]) );
    //Start another meassurement
    i2c_writeReg(MAG_ADDRESS,0x0a,0x01);
  }
#endif

// ************************************************************************************************************
// I2C Gyroscope and Accelerometer MPU6050
// ************************************************************************************************************
#if defined(MPU6050)
#if !defined(MPU6050_ADDRESS)
  #define MPU6050_ADDRESS     0x68 // address pin AD0 low (GND), default for FreeIMU v0.4 and InvenSense evaluation board
  //#define MPU6050_ADDRESS     0x69 // address pin AD0 high (VCC)
#endif

static void Gyro_init() {
  i2c_writeReg(MPU6050_ADDRESS, 0x6B, 0x80);             //PWR_MGMT_1    -- DEVICE_RESET 1
  delay(50);
  i2c_writeReg(MPU6050_ADDRESS, 0x6B, 0x03);             //PWR_MGMT_1    -- SLEEP 0; CYCLE 0; TEMP_DIS 0; CLKSEL 3 (PLL with Z Gyro reference)
  i2c_writeReg(MPU6050_ADDRESS, 0x1A, GYRO_DLPF_CFG);    //CONFIG        -- EXT_SYNC_SET 0 (disable input pin for data sync) ; default DLPF_CFG = 0 => ACC bandwidth = 260Hz  GYRO bandwidth = 256Hz)
  i2c_writeReg(MPU6050_ADDRESS, 0x1B, 0x18);             //GYRO_CONFIG   -- FS_SEL = 3: Full scale set to 2000 deg/sec
  // enable I2C bypass for AUX I2C
    i2c_writeReg(MPU6050_ADDRESS, 0x37, 0x02);           //INT_PIN_CFG   -- INT_LEVEL=0 ; INT_OPEN=0 ; LATCH_INT_EN=0 ; INT_RD_CLEAR=0 ; FSYNC_INT_LEVEL=0 ; FSYNC_INT_EN=0 ; I2C_BYPASS_EN=1 ; CLKOUT_EN=0
}

void Gyro_getADC () {
  i2c_getSixRawADC(MPU6050_ADDRESS, 0x43);
  GYRO_ORIENTATION( ((rawADC[0]<<8) | rawADC[1])>>2 , // range: +/- 8192; +/- 2000 deg/sec
                    ((rawADC[2]<<8) | rawADC[3])>>2 ,
                    ((rawADC[4]<<8) | rawADC[5])>>2 );
  GYRO_Common();
}

static void ACC_init ()
{
  i2c_writeReg(MPU6050_ADDRESS, 0x1C, 0x10);             //ACCEL_CONFIG  -- AFS_SEL=2 (Full Scale = +/-8G)  ; ACCELL_HPF=0   //note something is wrong in the spec.
  //note: something seems to be wrong in the spec here. With AFS=2 1G = 4096 but according to my measurement: 1G=2048 (and 2048/8 = 256)
  //confirmed here: http://www.multiwii.com/forum/viewtopic.php?f=8&t=1080&start=10#p7480

    //at this stage, the MAG is configured via the original MAG init function in I2C bypass mode
    //now we configure MPU as a I2C Master device to handle the MAG via the I2C AUX port (done here for HMC5883)
    i2c_writeReg(MPU6050_ADDRESS, 0x6A, 0b00100000);       //USER_CTRL     -- DMP_EN=0 ; FIFO_EN=0 ; I2C_MST_EN=1 (I2C master mode) ; I2C_IF_DIS=0 ; FIFO_RESET=0 ; I2C_MST_RESET=0 ; SIG_COND_RESET=0
    i2c_writeReg(MPU6050_ADDRESS, 0x37, 0x00);             //INT_PIN_CFG   -- INT_LEVEL=0 ; INT_OPEN=0 ; LATCH_INT_EN=0 ; INT_RD_CLEAR=0 ; FSYNC_INT_LEVEL=0 ; FSYNC_INT_EN=0 ; I2C_BYPASS_EN=0 ; CLKOUT_EN=0
    i2c_writeReg(MPU6050_ADDRESS, 0x24, 0x0D);             //I2C_MST_CTRL  -- MULT_MST_EN=0 ; WAIT_FOR_ES=0 ; SLV_3_FIFO_EN=0 ; I2C_MST_P_NSR=0 ; I2C_MST_CLK=13 (I2C slave speed bus = 400kHz)
    i2c_writeReg(MPU6050_ADDRESS, 0x25, 0x80|MAG_ADDRESS);//I2C_SLV0_ADDR -- I2C_SLV4_RW=1 (read operation) ; I2C_SLV4_ADDR=MAG_ADDRESS
    i2c_writeReg(MPU6050_ADDRESS, 0x26, MAG_DATA_REGISTER);//I2C_SLV0_REG  -- 6 data bytes of MAG are stored in 6 registers. First register address is MAG_DATA_REGISTER
    i2c_writeReg(MPU6050_ADDRESS, 0x27, 0x86);             //I2C_SLV0_CTRL -- I2C_SLV0_EN=1 ; I2C_SLV0_BYTE_SW=0 ; I2C_SLV0_REG_DIS=0 ; I2C_SLV0_GRP=0 ; I2C_SLV0_LEN=3 (3x2 bytes)
}

void ACC_getADC () {
  i2c_getSixRawADC(MPU6050_ADDRESS, 0x3B);
  ACC_ORIENTATION( ((rawADC[0]<<8) | rawADC[1])>>3 ,
                   ((rawADC[2]<<8) | rawADC[3])>>3 ,
                   ((rawADC[4]<<8) | rawADC[5])>>3 );
  ACC_Common();
}

#endif




void initSensors() {
  Gyro_init();
  Baro_init();
  Mag_init();
  ACC_init();
}
