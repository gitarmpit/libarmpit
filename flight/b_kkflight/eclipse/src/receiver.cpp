#include "receiver.h"

//received value (ex 1150 - 1800)
uint16_t RxChannel1;   
uint16_t RxChannel2;
uint16_t RxChannel3;
uint16_t RxChannel4;


int16_t RxInRoll;
int16_t RxInPitch;
int16_t RxInCollective;
int16_t RxInYaw;


/*
 * Copy, scale, and offset the Rx inputs from the interrupt-modified
 * registers.
 */
void RxGetChannels()
{
    //RxInRoll = fastdiv8(RxChannel1 - 1520 * 8);
    //RxInPitch = fastdiv8(RxChannel2 - 1520 * 8);
    //RxInCollective = fastdiv8(RxChannel3 - 1120 * 8);
    //RxInYaw = fastdiv8(RxChannel4 - 1520 * 8);
}

