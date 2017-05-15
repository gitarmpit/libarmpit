#ifndef _TASK_H 
#define _TASK_H
#include "scheduler.h"

class FC;

class Task_base 
{
    friend class Scheduler;
private:
    const int  taskId;
    const char * taskName;
    uint32_t desiredPeriod_us;         // target period of execution
    const uint8_t staticPriority;   // dynamicPriority grows in steps of this size, shouldn't be zero

    /* Scheduling */
    uint16_t dynamicPriority;       // measurement of how old task was last executed, used to avoid task starvation
    uint16_t taskAgeCycles;
    uint32_t lastExecutedAt;        // last time of invocation
    uint32_t lastSignaledAt;        // time of invocation event for event-driven tasks

    /* Statistics */
    uint32_t averageExecutionTime;  // Moving average over 6 samples, used to calculate guard interval
    uint32_t taskLatestDeltaTime;   //
#ifndef SKIP_TASK_STATISTICS
    uint32_t maxExecutionTime;
    uint32_t totalExecutionTime;    // total time consumed by task since boot
#endif

protected:
    int counter;
    FC* fc;


public:
    Task_base (const int taskId, const char* taskName, uint32_t period_ms, uint8_t priority);

    //TODO
    //virtual bool checkFunc(uint32_t currentDeltaTime) { return false; }
    virtual void taskFunc(void) = 0;
};


class TaskSystem : public Task_base 
{
private:
    Scheduler* sched;

 public:                                //10HZ
    TaskSystem (Scheduler* sched) : Task_base (TASK_SYSTEM, "SYSTEM", 100, TASK_PRIORITY_HIGH){ this->sched = sched;}
    virtual void taskFunc(void);
};

class TaskPid : public Task_base
{
public:                                //1000HZ
    TaskPid () : Task_base (TASK_PID, "PID", 1, TASK_PRIORITY_REALTIME){}
    virtual void taskFunc(void);
};


class TaskGyro : public Task_base 
{
public:                                //1000HZ
    TaskGyro () : Task_base (TASK_GYRO, "GYRO", 1, TASK_PRIORITY_REALTIME){}
    virtual void taskFunc(void);
};

//class TaskAccel : public Task_base
//{
//public:                             //1000HZ
//    TaskAccel () : Task_base (TASK_ACCEL, "ACCEL", 1, TASK_PRIORITY_MEDIUM) {}
//    virtual void taskFunc(void);
//};

class TaskAttitude : public Task_base
{
public:                             //100HZ
    TaskAttitude () : Task_base (TASK_ATTITUDE, "ATTITUDE", 10, TASK_PRIORITY_MEDIUM) {}
    virtual void taskFunc(void);
};



//class TaskBat : public Task_base
//{
//public:                              //50HZ
//    TaskBat () : Task_base (TASK_BATTERY, "BATTERY", 20, TASK_PRIORITY_MEDIUM) {}
//    virtual void taskFunc(void);
//};

class TaskRx : public Task_base 
{
public:                         //50HZ
    TaskRx () : Task_base (TASK_RX, "RX", 20, TASK_PRIORITY_HIGH) {}
    virtual void taskFunc(void);

};

#ifdef GPS
class TaskGps : public Task_base 
{
public:                         //10HZ
    TaskGps () : Task_base (TASK_GPS, "GPS", 100, TASK_PRIORITY_MEDIUM) {}
    virtual void taskFunc(void);
};
#endif

class TaskMag : public Task_base 
{
public:                         //10HZ
    TaskMag () : Task_base (TASK_COMPASS, "MAG", 100, TASK_PRIORITY_MEDIUM) {}
    virtual void taskFunc(void);
};

class TaskBaro : public Task_base 
{
public:                             //20HZ
    TaskBaro () : Task_base (TASK_BARO, "BARO", 50, TASK_PRIORITY_MEDIUM) {}
    virtual void taskFunc(void);
};

class TaskSonar : public Task_base 
{
public:                                //14HZ
    TaskSonar () : Task_base (TASK_SONAR, "SONAR", 70, TASK_PRIORITY_MEDIUM) {}
    virtual void taskFunc(void);
};

class TaskAlt : public Task_base 
{
public:                           //40HZ 
    TaskAlt () : Task_base (TASK_ALTITUDE, "ALT", 25, TASK_PRIORITY_MEDIUM) {}
    virtual void taskFunc(void);
};

#ifdef DISPLAY
class TaskDisplay : public Task_base
{
public:                           //10HZ
    TaskDisplay () : Task_base (TASK_DISPLAY, "DISPLAY", 100, TASK_PRIORITY_LOW) {}
    virtual void taskFunc(void);
};
#endif

#ifdef TELEMTRY
class TaskTelemetry : public Task_base
{
public:                           //10HZ
    TaskTelemetry () : Task_base (TASK_TELEMETRY, "TELEMETRY", 100, TASK_PRIORITY_IDLE) {}
    virtual void taskFunc(void);
};

#endif



#endif
