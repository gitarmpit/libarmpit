#ifndef _BUZZER_H
#define _BUZZER_H

#include "gpio.h"
#include "timer_cpp.h"

#define barbie2 "barbie2:d=8,0=5,b=150:f#,d#,f#,b,2g#"
#define  cuca  "cuca:d=8,o=5,b=200:2p,p,c,c,c,4f.,4a,c,c,c,4f.,al,2a,p,4f,f,e,e,d,d,4c,4p."
#define  cuca_short "cs:d=8,o=5,b=200:2p,p,c,c,c,4f.,4a,c,c,c,4f.,4a"
#define  chiz "ñhiz:d=8,o=5,b=200:e,c,e,c,f,e,d,p,g4,g4,g4,16a4,16b4,c,c,c,p"
#define  nokia "nokia:d=8,o=5,b=220:e6,d6,4f#,4g#,c#6,b,4d,4e,b,a,4c#,4e,1a"
#define  barbie "barbie:d=8,0=5,b=150:g#,e,g#,c#6,4a,4p,f#,d#,f#,b,4g#"
#define  barbie2 "barbie2:d=8,0=5,b=150:f#,d#,f#,b,2g#"
#define  polonez1 "polonez:d=8,o=5,b=120:4e.,16d#,16e,f,e,c,16cl,16b4,4a4"
#define  polonez2 "polonez:d=8,o=5,b=120:16d#,16e,f,e,c,16cl,16b4,4a4"
#define  pv "pv:d=8,o=5,b=120:c,eb,g,eb,4f,eb,d,4g,4f,2c,eb,g,bb,bb,4c6,bb,ab,2g,4a,4b,d6,c6,4g.,4d,c,g.,16f,2ab,bb,ab,4g,f,eb,4g,4f,2c"
#define  pv_short "pv_short:d=8,o=5,b=200:c,eb,g,eb,4f,eb,d,4g,4f,2c"
#define  inter "inter:d=8,o=5,b=200:4f,4bb.,a,c6,bb,f,d,2g,4eb"
#define  mars_short "mars:d=8,o=5,b=200:16d,d.,16d,4g,4g,4a,4a,4d6.,b,g."
#define  mars "mars:d=8,o=5,b=200:16d,d.,16d,4g,4g,4a,4a,4d6.,b,g.,16g,b.,16g,4e,2c6,a.,16f#,2g"
#define  rt1 "r1:d=8,o=6,b=400:c,e,g,e,c,g,e,g,c,g,c,e,c,g,e,g,e,c"
//#define  ii "ii:d=16,o=5,b=320:d,p,d,p,d,p,g,p,g,p,g,p,d,p,d,p,d,p,a,p,a,p,a,2p"
#define  rt_triple "Triple:d=8,o=5,b=635:c,e,g,c,e,g,c,e,g,c6,e6,g6,c6,e6,g6,c6,e6,g6,c7,e7,g7,c7,e7,g7,c7,e7,g7"

class Buzzer : protected TIMER_Interrupt_Handler
{
private:
    TIMER_Channel* _buzzerCh;
    uint8_t        _volume;
    const char*    _tune;
    int16_t       _tunePos;
    int           _d;
    int           _o;
    int           _b;
    bool          _cyclic;

    TIMER*        _asyncPlayTimer;

    static int find_char (const char* str, const char* c);

    int16_t process_control (const char* tune) ;
    bool parse_note (const char* note, int* freq, int* duron_ms, int* duroff_ms);

    virtual void HandleInterrupt(bool isUpdate, bool isTrigger);
    Buzzer(const Buzzer&);
    Buzzer();
    void operator=(Buzzer const&);

public:
    static Buzzer* GetInstance()
    {
        static Buzzer instance;
        return &instance;
    }

    void Init (TIMER_Number timer_no, const char* pin_name, int timerChannel);

    void PlayTone (uint16_t freq, uint16_t durationMs = 0);
    void Stop();
    void SetVolume(uint8_t vol); // 1 - 100

    bool PlayTune (const char* tune);

    void PlayTuneAsync(const char* tune, TIMER* timer, bool cyclic =false);

};


#endif
