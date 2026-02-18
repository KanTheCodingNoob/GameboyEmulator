//
// Created by kan on 2/18/26.
//

#pragma once
#include <array>
#include <cstdint>
#include <optional>

class PulseWaveChannel {
public:
    explicit PulseWaveChannel(const bool hasSweep) {
        if (hasSweep) {
            sweep.emplace();
        } else {
            sweep = std::nullopt;
        }
    };
    void clock();
private:
    struct Sweep {

    };

    struct PulsePhaseTimer {
        uint8_t phase = 0;
        uint16_t counter = 4 * 2048;
        uint16_t frequency = 0;

        void clock() {
            counter -= 4;
            if (counter <= 0) {
                counter = 4 * (2048 - frequency);
                phase = (phase + 1) % 8;
            }
        }

        void trigger() {
            counter = 4 * 2048;
        }
    };

    struct DutyCycle {
        std::array<std::array<uint8_t, 8>, 4> patterns = {{
            {0, 0, 0, 0, 0, 0, 0, 1}, // 12.5%
            {1, 0, 0, 0, 0, 0, 0, 1}, // 25%
            {1, 0, 0, 0, 0, 1, 1, 1}, // 50%
            {0, 1, 1, 1, 1, 1, 1, 0}  // 75%
        }};
    };

    struct LengthCounter {

    };

    struct Envelope {

    };

    bool enabled = true;
    std::optional<Sweep> sweep;
    PulsePhaseTimer timer;
    DutyCycle dutyCycle;
    LengthCounter lengthCounter;
    Envelope envelope;
};


