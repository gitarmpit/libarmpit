#ifndef DEF_H_
#define DEF_H_

///////////
//TODO
//#define delay(x)
#define micros() (0)

///////////

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))


#define MPU6050
#define AK8975     // MPU6050 + AK8975 = MPU9150
#define ACC_ORIENTATION(X, Y, Z)  {imu.accADC[ROLL]  = -X; imu.accADC[PITCH]  = -Y; imu.accADC[YAW]  =  Z;}
#define GYRO_ORIENTATION(X, Y, Z) {imu.gyroADC[ROLL] =  Y; imu.gyroADC[PITCH] = -X; imu.gyroADC[YAW] = -Z;}
#define MAG_ORIENTATION(Y, X, Z)  {imu.magADC[ROLL]  =  X; imu.magADC[PITCH]  =  Y; imu.magADC[YAW]  =  Z;}
#define BMP085
//#define HMC5883

 #define MULTITYPE 3

#define NUMBER_MOTOR 4
#define RC_CHANS 4



#endif /* DEF_H_ */
