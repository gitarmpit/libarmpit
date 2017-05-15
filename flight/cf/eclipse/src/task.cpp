#include "platform.h"
#include "task.h"
#include "fc.h"

////////////////////////////////////////////
Task_base::Task_base (const int taskId, const char* taskName, uint32_t period_ms, uint8_t priority) 
: taskId (taskId), staticPriority (priority)
{
    this->taskName = taskName;
    this->desiredPeriod_us = period_ms * 1000;

    dynamicPriority = 0;       // measurement of how old task was last executed, used to avoid task starvation
    taskAgeCycles = 0;
    lastExecutedAt = 0;        // last time of invocation
    lastSignaledAt = 0;        // time of invocation event for event-driven tasks

    averageExecutionTime = 0;  // Moving average over 6 samples, used to calculate guard interval
    taskLatestDeltaTime = 0;   //

#ifndef SKIP_TASK_STATISTICS
    maxExecutionTime = 0;
    totalExecutionTime = 0;    // total time consumed by task since boot
#endif

    counter = 0;

    fc = FC::GetInstance();
}

void TaskSystem::taskFunc(void)
{
    sched->taskSystem();
}
void TaskPid::taskFunc(void)
{
    fc->taskPid();
}
void TaskGyro::taskFunc(void)
{
    fc->taskGyro();
}
void TaskRx::taskFunc(void)
{
    fc->taskRx();
}
//void TaskAccel::taskFunc(void)
//{
//    fc->taskAccel();
//}
void TaskAttitude::taskFunc(void)
{
    fc->taskAttitude();
}

//void TaskBat::taskFunc(void)
//{
//    fc->taskVbat();
//}

#ifdef GPS
void TaskGps::taskFunc(void)
{
    fc->taskGps();
}
#endif
void TaskMag::taskFunc(void)
{
    fc->taskMag();
}
//#ifdef BARO
void TaskBaro::taskFunc(void)
{
    fc->taskBaro();
}
//#endif
//#ifdef SONAR
void TaskSonar::taskFunc(void)
{
    fc->taskSonar();
}
//#endif

//#if defined(BARO) || defined(SONAR)
void TaskAlt::taskFunc(void)
{
    fc->taskAlt();
//#endif
}
