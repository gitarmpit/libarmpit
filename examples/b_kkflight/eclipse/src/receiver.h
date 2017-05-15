#ifndef RECEIVER_H
#define RECEIVER_H

#include <stdint.h>

// Stick arming and throw detection (in % * 10 eg 1000 steps)
#define STICK_THROW 300

// Stick gain shift-right (after 32-bit multiplication of GainInADC[] value).
#define STICK_GAIN_SHIFT 8

// Max Collective
// limits the maximum stick collective (range 80->100  100=Off)
// this allows gyros to stabilise better when full throttle applied
#define MAX_COLLECTIVE 1000      // 95

extern int16_t RxInRoll;
extern int16_t RxInPitch;
extern int16_t RxInCollective;
extern int16_t RxInYaw;

extern uint16_t RxChannel1;
extern uint16_t RxChannel2;
extern uint16_t RxChannel3;
extern uint16_t RxChannel4;

void RxGetChannels(void);

#endif
