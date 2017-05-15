//long gyrBias[3];
//long accBias[3];
//int result;
//int i;
//
//result = mpu_run_self_test(gyrBias, accBias);
//
//// Check for passing accelerometer
//if ((result & 0x02) == 0x02)
//{
//  for (i=0; i<3; ++i) {
//    // The accBias values are stored in +/-8g format
//    accBias[i] *= 2048;
//    accBias[i] = accBias[i] >> 16;
//  }
//
//  // Cache these bias values, as they're not persisted to MPU memory
//  // Update the MPU bias registers (needs to be done on every reset)
//  mpu_set_accel_bias_6050_reg(accBias);
//}
