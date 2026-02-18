#pragma once
#include <cstdint>
#include <mutex>

#include "Audio/NoiseChannel.h"
#include "Audio/PulseWaveChannel.h"
#include "Audio/WaveChannel.h"

class Bus;

class APU {
public:
    explicit APU(Bus* bus);
    ~APU() = default;

    void clock();
    void reset();

    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);

private:
    Bus* bus;

    PulseWaveChannel ch1;
    PulseWaveChannel ch2;
    WaveChannel ch3;
    NoiseChannel ch4;

    uint8_t& NR10;
    uint8_t& NR11;
    uint8_t& NR12;
    uint8_t& NR13;
    uint8_t& NR14;

    uint8_t& NR21;
    uint8_t& NR22;
    uint8_t& NR23;
    uint8_t& NR24;

    uint8_t& NR30;
    uint8_t& NR31;
    uint8_t& NR32;
    uint8_t& NR33;
    uint8_t& NR34;

    uint8_t& NR41;
    uint8_t& NR42;
    uint8_t& NR43;
    uint8_t& NR44;

    uint8_t& NR50;
    uint8_t& NR51;
    uint8_t& NR52;

    uint8_t* WaveRAM;
};
