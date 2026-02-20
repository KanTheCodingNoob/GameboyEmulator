#pragma once
#include <cstdint>
#include <mutex>
#include <vector>

#include "Audio/NoiseChannel.h"
#include "Audio/PulseWaveChannel.h"
#include "Audio/WaveChannel.h"

class Bus;

class APU {
public:
    explicit APU(Bus* bus);
    ~APU() = default;

    void clock();

    void generateAudioSample();

    std::vector<float> &getAudioBuffer();

    void clearAudioBuffer();

    void reset();

    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);

private:
    Bus* bus;

    static constexpr double CPU_FREQ = 1048576.0;
    static constexpr double AUDIO_FREQ = 48000.0;

    double cyclesPerSample = CPU_FREQ / AUDIO_FREQ;
    double cycleCounter = 0.0;

    std::vector<float> audioBuffer;

    PulseWaveChannel ch1;
    PulseWaveChannel ch2;
    WaveChannel ch3;
    NoiseChannel ch4;

    struct FrameSequencer {
        int counter = 2048; // machine cycles
        uint8_t step = 0;   // 0-7

        template<typename Channel>
        void tick(Channel& ch) {
            counter--;

            if (counter > 0)
                return;

            counter = 2048;

            // 512 Hz step
            switch (step) {
                case 0:
                case 2:
                case 4:
                case 6:
                    ch.lengthCounter.clock();
                    break;
            }

            if (step == 7) {
                ch.envelope.clock();
            }

            // Sweep would go on steps 2 and 6

            step = (step + 1) & 7;
        }
    };

    FrameSequencer frameSequencer;

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
