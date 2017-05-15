#include "scheduler.h"
#include "task.h"
#include "maths.h"
#include "platform.h"


Scheduler::Scheduler()
{
	tasks[TASK_SYSTEM] = new TaskSystem(this);
	tasks[TASK_GYRO] = new TaskGyro();
	tasks[TASK_PID] = new TaskPid();
	tasks[TASK_ATTITUDE] = new TaskAttitude();
	//tasks[TASK_ACCEL] = new TaskAccel();
	//tasks[TASK_BATTERY] = new TaskBat();
	tasks[TASK_RX] = new TaskRx();
	//tasks[TASK_GPS] = new TaskGps();
	tasks[TASK_COMPASS] = new TaskMag();
	tasks[TASK_BARO] = new TaskBaro();
	tasks[TASK_SONAR] = new TaskSonar();
	tasks[TASK_ALTITUDE] = new TaskAlt();

	totalWaitingTasks = 0;
	totalIterations = 0;
	realtimeGuardInterval = REALTIME_GUARD_INTERVAL_MAX;


	taskQueueSize = 0;
	for (uint8_t i = 0; i < TASK_COUNT; ++i) 
	{
		taskQueue[i] = 0;
	}

	currentTask = 0;
	averageSystemLoadPercent = 0;
	currentTime_us = 0;

}




void Scheduler::run()
{
    // Cache currentTime
    currentTime_us = micros();

    // Check for realtime tasks
    uint32_t timeToNextRealtimeTask = UINT32_MAX;

	for (uint8_t i = 0; i < taskQueueSize; ++i) 
	{
		if (taskQueue[i]->staticPriority == TASK_PRIORITY_REALTIME) 
		{
	        const uint32_t nextExecuteAt = taskQueue[i]->lastExecutedAt + taskQueue[i]->desiredPeriod_us;
		    if ((int32_t)(currentTime_us - nextExecuteAt) >= 0) 
			{
				timeToNextRealtimeTask = 0;
			} 
			else 
			{
				const uint32_t newTimeInterval = nextExecuteAt - currentTime_us;
				timeToNextRealtimeTask = MIN(timeToNextRealtimeTask, newTimeInterval);
			}
		}
	}

    const bool outsideRealtimeGuardInterval = (timeToNextRealtimeTask > realtimeGuardInterval);

    // The task to be invoked
    Task_base *selectedTask = NULL;
    uint16_t selectedTaskDynamicPriority = 0;

    // Update task dynamic priorities
    uint16_t waitingTasks = 0;
	for (uint8_t i = 0; i < taskQueueSize; ++i) 
	{
		Task_base* task = taskQueue[i];

		//TODO event driven tasks (rx) do we need them?
        // Task has checkFunc - event driven
  //      if (task->checkFunc != NULL) 
		//{
  //          // Increase priority for event driven tasks
  //          if (task->dynamicPriority > 0) {
  //              task->taskAgeCycles = 1 + ((currentTime - task->lastSignaledAt) / task->desiredPeriod_us);
  //              task->dynamicPriority = 1 + task->staticPriority * task->taskAgeCycles;
  //              waitingTasks++;
  //          } else if (task->checkFunc(currentTime - task->lastExecutedAt)) {
  //              task->lastSignaledAt = currentTime;
  //              task->taskAgeCycles = 1;
  //              task->dynamicPriority = 1 + task->staticPriority;
  //              waitingTasks++;
  //          } else {
  //              task->taskAgeCycles = 0;
  //          }
  //      } 
		//else 
		{
            // Task is time-driven, dynamicPriority is last execution age (measured in desiredPeriods)
            // Task age is calculated from last execution
            task->taskAgeCycles = ((currentTime_us - task->lastExecutedAt) / task->desiredPeriod_us);

            if (task->taskAgeCycles > 0) 
			{
                task->dynamicPriority = 1 + task->staticPriority * task->taskAgeCycles;
                waitingTasks++;
            }
        }

        if (task->dynamicPriority > selectedTaskDynamicPriority) 
		{

            const bool taskCanBeChosenForScheduling =
                (outsideRealtimeGuardInterval) ||
                (task->taskAgeCycles > 1) ||
                (task->staticPriority == TASK_PRIORITY_REALTIME);

            if (taskCanBeChosenForScheduling) 
			{
                selectedTaskDynamicPriority = task->dynamicPriority;
                selectedTask = task;
            }
        }
    }

    totalIterations++;
    totalWaitingTasks += waitingTasks;

    currentTask = selectedTask;

    if (selectedTask != NULL) {
        // Found a task that should be run
        selectedTask->taskLatestDeltaTime = currentTime_us - selectedTask->lastExecutedAt;
        selectedTask->lastExecutedAt = currentTime_us;
        selectedTask->dynamicPriority = 0;

        // Execute task
        const uint32_t currentTimeBeforeTaskCall = micros();
        selectedTask->taskFunc();
        //TODO remove this counter
        selectedTask->counter++;
        const uint32_t taskExecutionTime = micros() - currentTimeBeforeTaskCall;

        selectedTask->averageExecutionTime = ((uint32_t)selectedTask->averageExecutionTime * 31 + taskExecutionTime) / 32;
#ifndef SKIP_TASK_STATISTICS
        selectedTask->totalExecutionTime += taskExecutionTime;   // time consumed by scheduler + task
        selectedTask->maxExecutionTime = MAX(selectedTask->maxExecutionTime, taskExecutionTime);
#endif
    }

}

bool Scheduler::isActive(int taskId) 
{
	for (uint8_t i = 0; i < taskQueueSize; ++i) 
	{
		if (taskQueue[i]->taskId == taskId) 
		{
			return true;
		}
	}

	return false;
}

void Scheduler::rescheduleTask(const int taskId, uint32_t newPeriodMillis)
{
	for (uint8_t i = 0; i < taskQueueSize; ++i) 
	{
		if (taskQueue[i]->taskId == taskId) 
		{
			taskQueue[i]->desiredPeriod_us =  newPeriodMillis * 1000;
		}
	}
}

void Scheduler::addTask(const int taskId)
{
	if (taskQueueSize == TASK_COUNT ||  isActive (taskId))
	{
		return;
	}

	Task_base* task = tasks[taskId];

    for (int i = 0; i <= taskQueueSize; ++i)
	{
        if (taskQueue[i] == NULL || (taskQueue[i]->staticPriority < task->staticPriority) )
		{
			for (int j = taskQueueSize; j > i; --j) 
			{
				taskQueue[j] = taskQueue[j-1];
			}
			taskQueue[i] = task;
            ++taskQueueSize;
			break;
		}
    }
}

void Scheduler::removeTask(const int taskId)
{
	for (uint8_t i = 0; i < taskQueueSize; ++i) 
	{
		if (taskQueue[i]->taskId == taskId) 
		{
			for (int j = i; j < taskQueueSize-1; ++j) 
			{
				taskQueue[j] = taskQueue[j+1];
			}
			taskQueue[taskQueueSize-1] = NULL;
			--taskQueueSize;
			break;
		}
	}
}


void Scheduler::taskSystem(void)
{
    /* Calculate system load */
    if (totalIterations > 0) {
        averageSystemLoadPercent = 100 * totalWaitingTasks / totalIterations;
        totalIterations = 0;
        totalWaitingTasks = 0;
    }

    /* Calculate guard interval */
    uint32_t maxNonRealtimeTaskTime = 0;
    for (int i = 0; i <= taskQueueSize; ++i)
	{
        if (taskQueue[i]->staticPriority != TASK_PRIORITY_REALTIME) 
		{
            maxNonRealtimeTaskTime = MAX(maxNonRealtimeTaskTime, taskQueue[i]->averageExecutionTime);
        }
    }

    realtimeGuardInterval = constrain(maxNonRealtimeTaskTime, REALTIME_GUARD_INTERVAL_MIN, REALTIME_GUARD_INTERVAL_MAX) + REALTIME_GUARD_INTERVAL_MARGIN;
}


uint32_t Scheduler::getTaskDeltaTime(const int taskId)
{
	Task_base* task = 0;
	if (taskId == TASK_SELF) 
	{
		task = currentTask;
	}
	else if (taskId < (int)TASK_COUNT) 
	{
		task = tasks[taskId];
	}

	return (task != 0) ? task->taskLatestDeltaTime : 0;
}



