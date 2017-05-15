#include <stdio.h>
#include "_pins.h"
#include "telemetry.h"
#include "GPIO_Helper.h"
#include "LED_Indicators.h"
#include "displays/9163/TFT_ILI9163C.h"
#include "system_time.h"
#define DEBUG_LCD

//Si4432* g_radio;

TFT_ILI9163C* g_lcd;

static void setup_lcd(SPI* spi);
static void setup_radio(SPI* spi);

void debug_init(TFT_ILI9163C* lcd)
{
    g_lcd = lcd;
    g_lcd->setBgColor(0);
    g_lcd->setTextColor(YELLOW);
    g_lcd->setTextSize(2);
}


void debug (telemetry* telemetry)
{
    char buf2[64], buf3[64];
#ifdef DEBUG_LCD
    if (!g_lcd)
        return;

    uint32_t t0 = millis();
    g_lcd->clearScreen();
    g_lcd->printf (0, 0, "Timestamp: %lu", telemetry->timestamp_ms);
    g_lcd->printf (0, 1, "Throttle: %d", telemetry->throttle);

    fix16_to_str ((telemetry->p_set), buf2, 2);
    g_lcd->printf (0, 2,  "Pitch set: %s", buf2);
    fix16_to_str ((telemetry->r_set), buf2, 2);
    g_lcd->printf (0, 3,  "Roll set: %s", buf2);

    fix16_to_str ((telemetry->p), buf2, 2);
    g_lcd->printf (0, 4,  "Pitch: %s", buf2);
    fix16_to_str ((telemetry->r), buf3, 2);
    g_lcd->printf (0, 5,  "Roll: %s", buf2);
//
//    g_lcd->printf (0, 3,  "%3.1f %3.1f", telemetry->p, telemetry->r);
//
    g_lcd->printf (0, 6,  "M1:%4d M2:%4d", telemetry->m[0], telemetry->m[1]);
    g_lcd->printf (0, 7,  "M3:%4d M4:%4d", telemetry->m[2], telemetry->m[3]);

    g_lcd->displayFast();

    volatile uint32_t d = millis() - t0;
    ++d;

#endif
#ifdef DEBUG_RADIO
    static const uint8_t retries = 150;
    if (g_radio && g_radio->isInited())
    {
        static int i = 0;
        ((i++ % 10) == 0) ? LED_Indicators::GetInstance()->MotorLedOn() :
                LED_Indicators::GetInstance()->MotorLedOff();
        g_radio->trySendPacket(sizeof(struct _telemetry), (uint8_t *) telemetry, retries);
    }
#endif
}



//static void setup_radio(SPI* spi)
//{
//    uint16_t radioRateKbit = 50;
//    uint32_t radioFreqMhz = 433;
//
//    GPIO_PIN* sdnPin = RADIO_SDN_PIN;
//    sdnPin->SetupGPIO_OutPP();
//
//    GPIO_PIN* radio_ssPin = RADIO_SS_PIN;
//    radio_ssPin->SetupGPIO_OutPP();
//
//    //radio and lcd share the same SPI
//    g_radio = new Si4432(spi, radio_ssPin, sdnPin, 0);
//
//    bool rc = g_radio->init();
//    if (rc)
//    {
//        g_radio->setBaudRate(radioRateKbit);
//        g_radio->setFrequency(radioFreqMhz);
//    }
//}
