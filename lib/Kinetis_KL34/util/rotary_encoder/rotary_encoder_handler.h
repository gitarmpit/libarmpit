#ifndef _ROTARY_HANDLER_H
#define _ROTARY_HANDLER_H

#include "pit.h"
#include "gpio.h"
#include "system_time.h"

#define SETBIT(var,bit) (var |= (0x01 << (bit)))
#define CLRBIT(var,bit) (var &= (~(0x01 << (bit))))

/* The default behavior is active low: button is configured as a weak pull-up
 * So to activate it, the switch needs to be connected from the input port to ground
 * Then pushing the switch will set it low: the IsOn method will return true
 *
 */
class RotaryHandler
{
private:
    const static uint32_t DEFAULT_TIMER_UPDATE_INTERVAL_US = 1000;

    uint32_t _timer_update_interval_us;

    PIT* _timer;

    GPIO_PIN* _A;
    GPIO_PIN* _B;
    uint8_t   _state = 0;
    uint16_t  _store=0;
    const int8_t _table[16] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};
    void  ProcessInterrupt();
protected:
    //A is CLK, B is DT
    RotaryHandler(PIT* timer, GPIO_PIN* A, GPIO_PIN* B);

public:

    static void RotaryHandleInterrupt(void* ctx);

    // Should be called after adding all buttons
    void Init(bool initialize_timer);

    void SetUpdateIntervalUs (uint32_t us);

    virtual void OnStateChange(int8_t dir) { UNUSED(dir); }


};

#endif
