#ifndef _FC_H
#define _FC_H
#include <stdint.h>
#include "pid.h"

class Scheduler;

class FC
{
private:
    Scheduler* sched;

    int16_t magHold;
    int16_t headFreeModeHold;
    bool isRXDataNew;
    bool _antiWindupEnabled;
    bool _airMode;
    bool _magMode;
    bool _headFreeMode;
    bool _altHoldMode;

    bool _isArmed;

    int32_t accSum_Z;
    uint32_t accDtSum;
    int accSumCount;

    StabilizationMode stabilizeMode;

    FC();
    FC(const FC&);

    //bool isCalibrating(void);
    void updateRcCommands(void);

    void updateMagHold(void);

    void filterRc(void);

public:

    static FC* GetInstance()
    {
        static FC instance;
        return &instance;
    }

    void taskPid();

    void taskGyro();
    void taskRx();
    void taskAttitude();
    void taskAlt();
    void taskBaro();
    void taskSonar();
    void taskMag();
    void taskGps();

    //TODO move to RC
    void mwDisarm();
    void mwArm();

};



#endif
