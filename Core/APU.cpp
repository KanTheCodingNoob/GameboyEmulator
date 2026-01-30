#include "APU.h"
#include "Bus.h"
#include <cmath>

APU::APU(Bus* bus) : bus(bus) {
    sampleBuffer.reserve(4410); // ~100ms
}

void APU::clock() {
    // Game Boy CPU runs at 4.194304 MHz. 
    // This clock() is called every M-cycle (4 T-cycles) or every T-cycle?
    // Bus::clock() calls cpu.clock() and ppu.clock(). 
    // Usually Bus::clock() corresponds to one M-cycle (4 T-cycles).
    
    cycleCounter += 4; // Assuming Bus::clock() is 4 T-cycles
    if (cycleCounter >= cyclesPerSample) {
        cycleCounter -= cyclesPerSample;

        float sample = 0.0f;
        if (enabled) {
            // Very simple square wave
            sample = (phase < 0.5f) ? 0.2f : -0.2f;
            phase += phaseIncrement;
            if (phase >= 1.0f) phase -= 1.0f;
        }

        std::lock_guard<std::mutex> lock(bufferMutex);
        sampleBuffer.push_back(sample);
    }
}

void APU::reset() {
    std::lock_guard<std::mutex> lock(bufferMutex);
    sampleBuffer.clear();
    enabled = false;
    phase = 0.0f;
}

void APU::write(uint16_t addr, uint8_t data) {
    if (addr >= 0xFF10 && addr <= 0xFF14) {
        // Simple test: if NR14 (0xFF14) bit 7 is set, enable sound and set frequency
        if (addr == 0xFF10) NR10 = data;
        if (addr == 0xFF11) NR11 = data;
        if (addr == 0xFF12) NR12 = data;
        if (addr == 0xFF13) NR13 = data;
        if (addr == 0xFF14) {
            NR14 = data;
            if (data & 0x80) {
                enabled = true;
                // frequency = 131072 / (2048 - (NR13 | ((NR14 & 7) << 8)))
                uint16_t freq_val = NR13 | ((NR14 & 0x07) << 8);
                float freq = 131072.0f / (2048.0f - (float)freq_val);
                phaseIncrement = freq / (float)sampleRate;
            }
        }
    }
}

uint8_t APU::read(uint16_t addr) {
    if (addr == 0xFF10) return NR10;
    if (addr == 0xFF11) return NR11;
    if (addr == 0xFF12) return NR12;
    if (addr == 0xFF13) return NR13;
    if (addr == 0xFF14) return NR14;
    return 0xFF;
}

std::vector<float> APU::getSamples() {
    std::lock_guard<std::mutex> lock(bufferMutex);
    std::vector<float> samples = std::move(sampleBuffer);
    sampleBuffer.clear();
    sampleBuffer.reserve(4410);
    return samples;
}
