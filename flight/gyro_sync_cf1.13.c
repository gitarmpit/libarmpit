extern gyro_t gyro;

uint32_t targetLooptime;
static uint8_t mpuDividerDrops;

bool gyroSyncCheckUpdate(void)
{
    return gyro.isDataReady && gyro.isDataReady();
}

void gyroSetSampleRate(uint32_t looptime, uint8_t lpf, uint8_t gyroSync, uint8_t gyroSyncDenominator)
{
    if (gyroSync) {
        int gyroSamplePeriod;
        if (lpf == 0) {
            gyroSamplePeriod = 125;
        } else {
            gyroSamplePeriod = 1000;
        }
        mpuDividerDrops = gyroSyncDenominator - 1;
        targetLooptime = gyroSyncDenominator * gyroSamplePeriod;
    } else {
        mpuDividerDrops = 0;
        targetLooptime = looptime;
    }
}

uint8_t gyroMPU6xxxCalculateDivider(void)
{
    return mpuDividerDrops;
}
