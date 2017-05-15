#include <platform.h>
#include "acc.h"
#include "cfg.h"
#include "sensors/mpu6500.h"

Acceleration::Acceleration(MPU* mpu)
{
    _mpu = mpu;
    accAlign = ALIGN_DEFAULT;
    calibratingA = 0;
    acfg = Config::GetInstance()->getAccConfig();

    if (acfg->acc_cut_hz != 0)
    {
    	for (int axis = 0; axis < 3; axis++)
    	{
    		biquadFilterInitLPF(&accFilter[axis], acfg->acc_cut_hz, acfg->accSamplingInterval);
    	}
    	isFiltering = true;
    }
    else
    {
    	isFiltering = true;
    }
}


void Acceleration::startCalibration()
{
    calibratingA = CALIBRATING_ACC_CYCLES;
}

void Acceleration::performAcclerationCalibration()
{
    static int32_t sum[3];

    for (uint8_t axis = 0; axis < 3; axis++)
    {
        if (calibratingA == CALIBRATING_ACC_CYCLES)
        {
            sum[axis] = 0;
            accZero[axis] = 0;
        }
        sum[axis] += accADC[axis];
    }

    if (calibratingA == 1)
    {
        accZero[0] = (sum[0] + (CALIBRATING_ACC_CYCLES / 2)) / CALIBRATING_ACC_CYCLES;
        accZero[1] = (sum[1] + (CALIBRATING_ACC_CYCLES / 2)) / CALIBRATING_ACC_CYCLES;
        accZero[2] = (sum[2] + (CALIBRATING_ACC_CYCLES / 2)) / CALIBRATING_ACC_CYCLES - acfg->acc_1G;

        //TODO
        //saveConfigAndNotify();
    }

    calibratingA--;
}



void Acceleration::update()
{
	int32_t accADCRaw[3];
    if (!_mpu->AccRead(accADCRaw))
    {
        return;
    }

    alignSensors(accADC, accADC, accAlign);

    if (calibratingA != 0)
    {
        performAcclerationCalibration();
        accADC[0] = accADC[1] = accADC[2] = 0;
    }
    else
    {
    	accADC[0] -= accZero[0];
    	accADC[1] -= accZero[1];
    	accADC[2] -= accZero[2];

        for (int axis = 0; axis < 3; axis++)
        {
        	if (isFiltering)
        	{
        		accADC[axis] = lrintf(biquadFilterApply(&accFilter[axis], (float)accADCRaw[axis]));
        	}
        	else
        	{
        		accADC[axis] = accADCRaw[axis];
        	}
        }

    }



}



