#include "AD9833.h"
#include "RCC_Helper.h"
#include <math.h>
#include "debug.h"

AD9833::AD9833 (SPI* spi, GPIO_PIN* ssPin, uint32_t referenceFrequency )
{
	_spi = spi;
	_ssPin = ssPin;
    refFrequency = referenceFrequency;
    waveForm0 = waveForm1 = SINE_WAVE;
    frequency0 = frequency1 = 1000;     // 1 KHz sine wave to start
    phase0 = phase1 = 0.0;              // 0 phase
    activeFreq = REG0; 
    activePhase = REG0;
    delay(100);
    Reset();    // Hold in RESET until first WriteRegister command
}

void AD9833::Reset ( void )
{
    WriteRegister(RESET_CMD);
    delay(15);
}

void AD9833::SetFrequency ( Registers freqReg, float frequency ) {
    
    if ( frequency > 12.5e6 )   
        frequency = 12.5e6;
    if ( frequency < 0.0 ) frequency = 0.0;
    
    if ( freqReg == REG0 ) 
    {
        frequency0 = frequency;
    }
    else 
    {
        frequency1 = frequency;
    }
    
    int32_t freqWord = (frequency * pow2_28) / (float)refFrequency;
    int16_t upper14 = (int16_t)((freqWord & 0xFFFC000) >> 14), 
            lower14 = (int16_t)(freqWord & 0x3FFF);

    uint16_t reg = freqReg == REG0 ? FREQ0_WRITE_REG : FREQ1_WRITE_REG;
    lower14 |= reg;
    upper14 |= reg;   

    WriteControlRegister();
    WriteRegister(lower14);         // Write lower 14 bits to AD9833
    WriteRegister(upper14);         // Write upper 14 bits to AD9833
}

void AD9833::IncrementFrequency ( Registers freqReg, float freqIncHz ) {
    float frequency = (freqReg == REG0) ? frequency0 : frequency1;
    SetFrequency(freqReg,frequency+freqIncHz);
}

void AD9833::SetPhase ( Registers phaseReg, float phaseInDeg ) {
    // Sanity checks on input
    phaseInDeg = fmod(phaseInDeg,360);
    if ( phaseInDeg < 0 ) phaseInDeg += 360;
    
    // Phase is in float degrees ( 0.0 - 360.0 )
    // Convert to a number 0 to 4096 where 4096 = 0 by masking
    uint16_t phaseVal = (uint16_t)(BITS_PER_DEG * phaseInDeg) & 0x0FFF;
    phaseVal |= PHASE_WRITE_CMD;
    
    // Save phase for use by IncrementPhase function
    if ( phaseReg == REG0 ) {
        phase0 = phaseInDeg;
    }
    else {
        phase1 = phaseInDeg;
        phaseVal |= PHASE1_WRITE_REG;
    }
    WriteRegister(phaseVal);
}

void AD9833::IncrementPhase ( Registers phaseReg, float phaseIncDeg ) {
    float phase = (phaseReg == REG0) ? phase0 : phase1;
    SetPhase(phaseReg,phase + phaseIncDeg);
}

void AD9833::SetWaveform (  Registers waveFormReg, WaveformType waveType ) {
    if ( waveFormReg == REG0 )
        waveForm0 = waveType;
    else
        waveForm1 = waveType;
    WriteControlRegister();
}


/*
 * Set which frequency and phase register is being used to output the
 * waveform. If phaseReg is not supplied, it defaults to the same
 * register as freqReg.
 */
void AD9833::SetOutputSource ( Registers freqReg, Registers phaseReg ) {
    // TODO: Add more error checking?
    activeFreq = freqReg;
    if ( phaseReg == SAME_AS_REG0 ) activePhase = activeFreq;
    else activePhase = phaseReg;
    WriteControlRegister();
}


float AD9833::GetActualProgrammedFrequency ( Registers reg ) {
    float frequency = reg == REG0 ? frequency0 : frequency1;
    int32_t freqWord = (uint32_t)((frequency * pow2_28) / (float)refFrequency) & 0x0FFFFFFFUL;
    return (float)freqWord * (float)refFrequency / (float)pow2_28;
}

float AD9833::GetActualProgrammedPhase ( Registers reg ) {
    float phase = reg == REG0 ? phase0 : phase1;
    uint16_t phaseVal = (uint16_t)(BITS_PER_DEG * phase) & 0x0FFF;
    return (float)phaseVal / BITS_PER_DEG;
}

float AD9833::GetResolution ( void ) {
    return (float)refFrequency / (float)pow2_28;
}

void AD9833::WriteControlRegister ( void ) {
    uint16_t waveForm;

    if ( activeFreq == REG0 ) 
    {
        waveForm = waveForm0;
        waveForm &= ~FREQ1_OUTPUT_REG;
    }
    else {
        waveForm = waveForm1;
        waveForm |= FREQ1_OUTPUT_REG;
    }

    if ( activePhase == REG0 )
    {
        waveForm &= ~PHASE1_OUTPUT_REG;
    }
    else
    {
        waveForm |= PHASE1_OUTPUT_REG;
    }

//    waveForm &= ~RESET_CMD;
//    waveForm &= ~DISABLE_DAC;
//    waveForm &= ~DISABLE_INT_CLK;

    WriteRegister ( waveForm );
}

static void send_byte(uint8_t data)
{
    static GPIO_PIN* clockPin =  0;
    static GPIO_PIN* dataPin = 0;
    if (clockPin == 0)
    {
        clockPin = GPIOA::GetInstance()->GetPin(GPIO_PIN5);
        clockPin->SetupGPIO_OutPP();
        dataPin =  GPIOA::GetInstance()->GetPin(GPIO_PIN7);
        dataPin->SetupGPIO_OutPP();
    }

    for (int i = 0; i < 8; i++, data >>= 1)
    {
        (data & 0x01) ? dataPin->Set() : dataPin->Reset();
        clockPin->Set();
        //RCC_Delay_us(40);
        clockPin->Reset();
    }
}


void AD9833::WriteRegister ( int16_t dat )
{
    _ssPin->Reset();
    //send_byte((dat>>8) & 0xff);
    _spi->TransmitByte((dat>>8) & 0xff);
    delay(2);
    _spi->TransmitByte(dat & 0xff);
    //send_byte(dat & 0xff);
    _ssPin->Set();


    //WRITE_FNCPIN(LOW);      // FNCpin low to write to AD9833

    //delayMicroseconds(2); // Some delay may be needed

    // TODO: Are we running at the highest clock rate?
    //SPI.transfer(highByte(dat));    // Transmit 16 bits 8 bits at a time
    //SPI.transfer(lowByte(dat));

    //WRITE_FNCPIN(HIGH);     // Write done
}

