#pragma once
#include <cstdint>
#include <vector>
#include <mutex>

class Bus;

class APU {
public:
    explicit APU(Bus* bus);
    ~APU() = default;

    void clock();
    void reset();

    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);

    // Get samples and clear buffer
    std::vector<float> getSamples();

private:
    Bus* bus;

    // Basic oscillator for testing (Square wave Channel 1 simplified)
    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    bool enabled = false;

    // Registers
    uint8_t NR10 = 0, NR11 = 0, NR12 = 0, NR13 = 0, NR14 = 0;
    // ... other registers ...

    std::vector<float> sampleBuffer;
    std::mutex bufferMutex;

    int sampleRate = 44100;
    int cyclesPerSample = 4194304 / sampleRate;
    int cycleCounter = 0;
};
