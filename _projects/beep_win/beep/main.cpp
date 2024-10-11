#include <windows.h>
#include "buzzer.h"
#include <vector>

/*
int main() {

    Buzzer* buzzer = Buzzer::GetInstance();
    buzzer->PlayTune("tune:d=2,o=5,b=350:al,bl,c6l,d6");
    return 0;
}
*/

#include <windows.h>
#include <iostream>
#include <cmath>
#include <thread>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int SAMPLE_RATE = 44100; // Sample rate in Hz
const int BUFFER_SIZE = 44100; // Buffer size for one second of audio

// Function to generate a simple sine wave
void generateSineWave2(short* buffer, int frequency, int durationMs) {
    int totalSamples = (SAMPLE_RATE * durationMs) / 1000;
    for (int i = 0; i < totalSamples; i++) {
        buffer[i] = (short)(32767 * sin((2.0 * M_PI * frequency * i) / SAMPLE_RATE));
    }
}

// Function to play a note
void playNote2(int frequency, int durationMs) {
    short waveBuffer[BUFFER_SIZE];

    // Generate the sine wave for the specified frequency
    generateSineWave2(waveBuffer, frequency, durationMs);

    // Set up the wave format
    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 1; // Mono
    waveFormat.nSamplesPerSec = SAMPLE_RATE;
    waveFormat.nAvgBytesPerSec = SAMPLE_RATE * sizeof(short);
    waveFormat.nBlockAlign = sizeof(short);
    waveFormat.wBitsPerSample = 16;
    waveFormat.cbSize = 0;

    // Open the wave output device
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL);

    // Prepare the header
    WAVEHDR waveHeader;
    waveHeader.lpData = (LPSTR)waveBuffer;
    waveHeader.dwBufferLength = BUFFER_SIZE * sizeof(short);
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 0;

    // Play the sound
    waveOutPrepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));

    // Wait for the sound to finish playing
    while (!(waveHeader.dwFlags & WHDR_DONE)) {
        Sleep(10);
    }

    // Clean up
    waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

// Function to play a sequence of notes
void playNotes(const std::vector<int>& frequencies, int durationMs) {
    for (const auto& freq : frequencies) {
        playNote2(freq, durationMs);
        // Add a slight delay between notes
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Adjust as needed
    }
}

int main() {
    std::vector<int> frequencies = { 440, 494, 523, 587, 659, 698, 784, 880 }; // A4 to A5

    std::cout << "Playing sequence of notes..." << std::endl;
    playNotes(frequencies, 500); // Play each note for 500 ms

    return 0;
}
