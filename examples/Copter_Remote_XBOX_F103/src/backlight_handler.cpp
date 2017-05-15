#include "backlight_handler.h"

static BacklightHandler* g_handler;

extern "C" void systick_handler(void)
{
    if (g_handler)
    {
        g_handler->Disable();
    }
}


BacklightHandler::BacklightHandler()
{
    uint32_t blDuration_ms = 5000;
    uint32_t clock = RCC_GetAHB_CLK();
    _systick_count = (uint64_t) clock * (uint64_t) blDuration_ms / 1000llu / 8;
    g_handler = this;
    _lcdBL = 0;
}

void BacklightHandler::SetBackLightChannel (TIMER_Channel* blCh)
{
    _lcdBL = blCh;
}


void BacklightHandler::Enable()
{
    _lcdBL->UpdatePWMDuty(1000);
    SystickEnable(_systick_count, true, true, systick_handler);
}

void BacklightHandler::Disable()
{
    _lcdBL->UpdatePWMDuty(0);
    SystickDisable();
}
