void annexCode(void)
{
    rx->rc 
    dyn pid calc

    head free
    bat
    buzzerUpdate();

    cal LEDs


uint16_t pwmReadRawRC(uint8_t chan)
{
    return 0; //TODO read pwm here
}

void computeRC(void)
{
    read PWM
}

static void mwArm(void)
{
}

static void mwDisarm(void)
}


void loop(void)
{
    static uint8_t rcDelayCommand;      // this indicates the number of time (multiple of RC measurement at 50Hz) the sticks must be maintained to run or switch off motors
    static uint8_t rcSticks;            // this hold sticks position for command combos
    uint8_t stTmp = 0;
    int i;
    static uint32_t rcTime = 0;
    static uint32_t loopTime;
    bool isThrottleLow = false;

    if (((int32_t)(currentTime - rcTime) >= 0)) { // 50Hz
        rcTime = currentTime + 20000;
        computeRC();
        failsafe
        process sticks         arm disarm, activate boxes, start cal
    } 
    else 
    {                        // not in rc loop
        static int taskOrder = 0;   // never call all function in the same loop, to avoid high delay spikes
        switch (taskOrder) {
            case 0:
                taskOrder++;   get mag adc
                    break;
            case 1:
                taskOrder++;  baro adc
                    break;
            case 2:
                taskOrder++;  altitude
                break
            case 3:
                taskOrder++;  GPS
                break  
            case 4:
                taskOrder = 0;  sonar
                break;
        }
    }

    currentTime = micros();
    if (mcfg.looptime == 0 || (int32_t)(currentTime - loopTime) >= 0) 
    {
        loopTime = currentTime + mcfg.looptime;
        computeIMU();
        currentTime = micros();
        cycleTime = (int32_t)(currentTime - previousTime);
        previousTime = currentTime;
        annexCode();
        apply magHold
        apply altHold
        
        throttle angle corr
        gps
        pid_controller();
        mixTable();
        writeMotors();
    }
}
