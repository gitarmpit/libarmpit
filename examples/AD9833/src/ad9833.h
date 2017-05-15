#ifndef __AD9833__

#define __AD9833__

#include <stdint.h>
#include "spi_cpp.h"

#define pow2_28             268435456L  // 2^28 used in frequency word calculation
#define BITS_PER_DEG        11.3777777777778    // 4096 / 360

#define RESET_CMD           0x0100      // Reset enabled (also CMD RESET)
#define SLEEP_MODE          0x00C0      // Both DAC and Internal Clock
#define DISABLE_DAC         0x0040
#define DISABLE_INT_CLK     0x0080

#define PHASE_WRITE_CMD     0xC000      // Setup for Phase write
#define PHASE1_WRITE_REG    0x2000      // Which phase register
#define FREQ0_WRITE_REG     0x4000      // 
#define FREQ1_WRITE_REG     0x8000
#define PHASE1_OUTPUT_REG   0x0400      // Output is based off REG0/REG1
#define FREQ1_OUTPUT_REG    0x0800      // ditto

typedef enum { SINE_WAVE = 0x2000, TRIANGLE_WAVE = 0x2002,
               SQUARE_WAVE = 0x2028, HALF_SQUARE_WAVE = 0x2020 } WaveformType;
               
typedef enum { REG0, REG1, SAME_AS_REG0 } Registers;

class AD9833 
{

public:
    
    AD9833 (SPI* spi, GPIO_PIN* ssPin, uint32_t referenceFrequency = 25000000UL );

    void Reset ( void );
    void SetFrequency ( Registers freqReg, float frequency );
    void IncrementFrequency ( Registers freqReg, float freqIncHz );
    void SetPhase ( Registers phaseReg, float phaseInDeg );
    void IncrementPhase ( Registers phaseReg, float phaseIncDeg );

    // SINE_WAVE, TRIANGLE_WAVE, SQUARE_WAVE, HALF_SQUARE_WAVE,
    void SetWaveform ( Registers waveFormReg, WaveformType waveType );

    void SetOutputSource ( Registers freqReg, Registers phaseReg = SAME_AS_REG0 );

    float GetActualProgrammedFrequency ( Registers reg );
    float GetActualProgrammedPhase ( Registers reg );
    float GetResolution ( void );

private:

    void            WriteRegister ( int16_t dat );
    void            WriteControlRegister ( void );

    SPI*            _spi;
    GPIO_PIN*       _ssPin;
    uint16_t        waveForm0, waveForm1;
    uint32_t        refFrequency;
    float           frequency0, frequency1, phase0, phase1;
    Registers       activeFreq, activePhase;
};

#endif

