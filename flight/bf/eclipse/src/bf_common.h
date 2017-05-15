#ifndef _BF_COMMON_H
#define _BF_COMMON_H
#define abs(x) ((x) > 0 ? (x) : -(x))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

enum
{
    X = 0,
    Y = 1,
    Z = 2
};

enum
{
    ROLL = 0,
    PITCH = 1,
    YAW = 2,
    THROTTLE = 3
};

enum FLIGHT_MODE
{
	NONE,
    ANGLE_MODE,
    HORIZON_MODE,
};

int constrain(int amt, int low, int high);

#endif
