//
// Created by kan on 2/18/26.
//

#pragma once

#include <cstdint>

class WaveChannel {
public:
    WaveChannel() = default;
    void clock();
    void trigger();
    uint8_t sample();

    void setInitialLengthTimer(uint8_t lengthTimer);
    void setOutputLevel(uint8_t outputLevel);
    void setLowPeriodValue(uint8_t lowPeriodValue);
    void setHighPeriodValue(uint8_t highPeriodValue);
    void lengthEnable(bool enable);
    void dacEnable(bool enable);

    void setWaveRAM(uint8_t* ram) {
        waveRAM = ram;
    }

private:
    struct WaveTimer {
        uint8_t sampleIndex = 0;
        int counter = 0;
        uint16_t frequency = 0;

        void tick() {
            counter -= 4;

            if (counter <= 0) {
                reloadCounter();
                sampleIndex = (sampleIndex + 1) % 32;
            }
        }

        void trigger() {
            reloadCounter();
            sampleIndex = 0;
        }

        void reloadCounter() {
            counter = 2 * (2048 - frequency);
        }
    };

    struct LengthCounter {
        bool enabled = false;
        int counter = 256;

        void load(const uint8_t length) {
            counter = 256 - length;
        }

        void clock() {
            if (!enabled || counter == 0) {
                return;
            }

            counter -= 1;
            if (counter == 0) {
                // Channel is disabled when length counter hits 0
            }
        }

        void trigger() {
            if (counter == 0) {
                counter = 256;
            }
        }
    };

    bool enabled = false;
    bool dacOn = false;
    uint8_t volumeShift = 0; // 0=mute, 1=100%, 2=50%, 3=25%
    WaveTimer timer;
    LengthCounter lengthCounter;
    uint8_t* waveRAM = nullptr;

    friend class APU;
};

