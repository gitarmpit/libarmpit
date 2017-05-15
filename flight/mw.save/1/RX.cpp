#include "config.h"
#include "def.h"
#include "types.h"
#include "MultiWii.h"

//RAW RC values will be store here

volatile uint16_t rcValue[RC_CHANS] = {1502, 1502, 1502, 1502}; // interval [1000;2000]

static uint8_t rcChannel[RC_CHANS]  = {0};

void rxInt(void);
void configureReceiver()
{
}


uint16_t readRawRC(uint8_t chan) {
  uint16_t data;
  data = rcValue[rcChannel[chan]]; // Let's copy the data Atomically
  return data; // We return the value correctly copied when the IRQ's where disabled
}

#define AVERAGING_ARRAY_LENGTH 4

void computeRC()
{
    static uint16_t rcData4Values[RC_CHANS][AVERAGING_ARRAY_LENGTH-1];
    uint16_t rcDataMean,rcDataTmp;
    static uint8_t rc4ValuesIndex = 0;
    uint8_t chan,a;
    uint8_t failsafeGoodCondition = 1;

    rc4ValuesIndex++;
    if (rc4ValuesIndex == AVERAGING_ARRAY_LENGTH-1)
    {
        rc4ValuesIndex = 0;
    }

    for (chan = 0; chan < RC_CHANS; chan++)
    {
        rcDataTmp = readRawRC(chan);

#if defined(FAILSAFE)
        failsafeGoodCondition = rcDataTmp>FAILSAFE_DETECT_TRESHOLD || !f.ARMED; // update controls channel only if pulse is above FAILSAFE_DETECT_TRESHOLD
#endif

        if(failsafeGoodCondition)
        {
            //rcData[chan] = rcDataTmp;  //TODO no averaging

            rcDataMean = rcDataTmp;
            for (a=0;a<AVERAGING_ARRAY_LENGTH-1;a++)
            {
                rcDataMean += rcData4Values[chan][a];
            }
            rcDataMean = (rcDataMean+(AVERAGING_ARRAY_LENGTH/2))/AVERAGING_ARRAY_LENGTH;

            if ( rcDataMean < (uint16_t)rcData[chan] -3)
                rcData[chan] = rcDataMean+2;

            if ( rcDataMean > (uint16_t)rcData[chan] +3)
                rcData[chan] = rcDataMean-2;

            rcData4Values[chan][rc4ValuesIndex] = rcDataTmp;
        }

    }
}


