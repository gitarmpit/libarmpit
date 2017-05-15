#include "gpio.h"
#include "timer.h"
#include "flash.h"
#include "rcc.h"
#include "debug.h"


uint32_t _lastCounterValue;
uint32_t _clockFreq;
uint32_t _presc;
uint32_t _ar;
uint32_t _missedCount;
    
uint32_t _period;
uint32_t _duty;

BOOL  _falling_edge;


#define IN_CHANNEL 1

TIM_CFG cfg_in;

static void capture_handler(void ) 
{
    uint32_t cnt = TIM_GetCCR(&cfg_in, IN_CHANNEL);
    
    if (_falling_edge) 
    {
        TIM_SetICCRisingEdge (&cfg_in, IN_CHANNEL);
    }
    else
    {
        TIM_SetICCFallingEdge (&cfg_in, IN_CHANNEL);
    }
    
    _missedCount = 0;
    
    if (_lastCounterValue != 0 ) 
    {
        int32_t tick = cnt - _lastCounterValue;
        if (tick < 0) 
        {
            tick += _ar + 1;
        }
        
        if (_falling_edge) 
        {
            _duty = tick;
        }
        else if (_duty != 0) 
        {
            _period = _duty + tick;
        }
    }
    
    _falling_edge = !_falling_edge;

    _lastCounterValue = cnt;
    
}

static uint32_t get_period() 
{
    return (uint64_t)_period * 1000000ull * (uint64_t)_presc / _clockFreq;
}
static uint32_t get_duty() 
{
    return (uint64_t)_duty * 1000000ull * (uint64_t)_presc / _clockFreq;
}


static void capture_cfg() 
{
    TIM_EnableCCInterrupt (&cfg_in, IN_CHANNEL, TRUE);
    TIM_SetModeInputDirect(&cfg_in, TRUE);
    TIM_SetICCFallingEdge(&cfg_in, IN_CHANNEL);
    _falling_edge = TRUE;
    _presc = TIM_GetPrescalerValue (&cfg_in) + 1;
    _ar = TIM_GetAutoReloadValue (&cfg_in);
    _clockFreq = TIM_GetTimerClockFreq (&cfg_in);
    
    TIM_EnableChannel (&cfg_in, IN_CHANNEL, TRUE);
    TIM_SetHandler (&cfg_in, capture_handler);
    
}

//void TIM3_IRQHandler() 
//{
//    if (TIM_IsUIF(&cfg)) 
//    {
//        GPIO_toggle_pin(&gpio, GPIO_PIN0);
//        TIM_ClearUIF(&cfg);
//    }
//}

int main()
{
    FLASH_SetWaitState(2); 
    RCC_EnablePLL(16); 
    //RCC_SetAHBPrescaler(2);
 
    GPIO_CFG gpio;
    GPIO_init (GPIOA, &gpio);
    GPIO_cfg_out_alt_pp(&gpio, GPIO_PIN0);
    
    TIM_CFG cfg_out;
    TIM_Init (TIM2, &cfg_out);
    
    uint32_t period = 0x22222;
    
    TIM_SetupPWM1 (&cfg_out, IN_CHANNEL, period, 0x20);
    TIM_EnableChannel (&cfg_out, IN_CHANNEL, TRUE);
    TIM_EnableCounter (&cfg_out, TRUE);

    GPIO_cfg_in_float(&gpio, GPIO_PIN6);

    TIM_Init (TIM3, &cfg_in);
    TIM_EnableNVICInterrupt(&cfg_in, TRUE);
    TIM_SetCounterValue(&cfg_in, 0);
    TIM_SetUpdatePeriod_us(&cfg_in, period * 2);
    //Capture_Cfg (&cfg_in, IN_CHANNEL);
    capture_cfg();
    TIM_EnableCounter(&cfg_in, TRUE);
    //TIM_SetHandler (TIM3, f);
    
    Debug_EnableCYCCNT(TRUE);
    delay_us (1000000);
    
    
    volatile uint32_t p[128];
    volatile uint32_t d[128];
    
    for (uint8_t i = 0; i < 128; ++i) 
    {
        p[i] = get_period(); //CaptureGetPeriod ();
        d[i] = get_duty(); //CaptureGetDuty();
        delay_us (10000);
    }
    
    while(1)
        ;

   // test();

}
