#ifndef _SCHEDULER_H 
#define _SCHEDULER_H 

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "platform.h"

#define REALTIME_GUARD_INTERVAL_MIN     10
#define REALTIME_GUARD_INTERVAL_MAX     300
#define REALTIME_GUARD_INTERVAL_MARGIN  25


enum TASK_PRORITY {
    TASK_PRIORITY_IDLE = 0,     // Disables dynamic scheduling, task is executed only if no other task is active this cycle
    TASK_PRIORITY_LOW = 1,
    TASK_PRIORITY_MEDIUM = 3,
    TASK_PRIORITY_HIGH = 5,
    TASK_PRIORITY_REALTIME = 6,
    TASK_PRIORITY_MAX = 255
}; 

#define TASK_SELF -1

struct TaskInfo {
    const char * taskName;
    bool         isEnabled;
    uint32_t     desiredPeriod;
    uint8_t      staticPriority;
    uint32_t     maxExecutionTime;
    uint32_t     totalExecutionTime;
    uint32_t     averageExecutionTime;
    uint32_t     latestDeltaTime;
};


class FC;
class Task_base;

////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
    /* Actual tasks */
    TASK_SYSTEM = 0,
    TASK_GYRO,
    TASK_PID,
    TASK_ACCEL,
    TASK_ATTITUDE,
    TASK_RX,
    TASK_SERIAL,
    TASK_BATTERY,
#ifdef BEEPER
    TASK_BEEPER,
#endif
#ifdef GPS
    TASK_GPS,
#endif
#ifdef MAG
    TASK_COMPASS,
#endif
#ifdef BARO
    TASK_BARO,
#endif
#ifdef SONAR
    TASK_SONAR,
#endif
#if defined(BARO) || defined(SONAR)
    TASK_ALTITUDE,
#endif
#ifdef DISPLAY
    TASK_DISPLAY,
#endif
#ifdef TELEMETRY
    TASK_TELEMETRY,
#endif
#ifdef LED_STRIP
    TASK_LEDSTRIP,
#endif
#ifdef TRANSPONDER
    TASK_TRANSPONDER,
#endif

    /* Count of real tasks */
    TASK_COUNT
} cfTaskId_e;



//////////////////////////////////////////////////////////////////////
class Scheduler 
{

private:

    Task_base* tasks[TASK_COUNT];
    Task_base* taskQueue[TASK_COUNT];

    Task_base* currentTask;

    int taskQueueSize;

    uint32_t totalWaitingTasks;
    uint32_t totalIterations;
    uint32_t realtimeGuardInterval;
    uint16_t averageSystemLoadPercent;
    uint32_t currentTime_us;

    bool isActive(int taskId);

    Scheduler();
    Scheduler(const Scheduler&);
    
public:

    static Scheduler* GetInstance()
    {
        static Scheduler instance;
        return &instance;
    }

    void run();
    void rescheduleTask(const int taskId, uint32_t newPeriodMicros);
    void addTask(const int taskId);
    void removeTask(const int taskId);
    void taskSystem(void);
    uint32_t getTaskDeltaTime(const int taskId);

    bool isSystemOverloaded() { return averageSystemLoadPercent >= 100; }
    uint32_t getCurrentTime_us() { return currentTime_us; }

    uint16_t getAverageSystemLoadPercent() { return averageSystemLoadPercent; }
};



#endif
