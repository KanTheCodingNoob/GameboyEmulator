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
    void trigger();
    uint8_t sample();

    void setInitialLengthTimer(uint8_t lengthTimer);

    void setWaveDuty(uint8_t waveDuty);

    void setInitialVolume(uint8_t initialVolume);

    void setEnvDirection(bool envDirection);

    void setSweepPace(uint8_t pace);

    void setLowPeriodValue(uint8_t lowPeriodValue);

    void setHighPeriodValue(uint8_t highPeriodValue);

    void lengthEnable(bool enable);

    bool& getChannelEnabled();

private:
    struct Sweep {

    };

    struct PulsePhaseTimer {
        uint8_t phase = 0;
        int counter = 4 * 2048;
        uint16_t frequency = 0;

        void tick() {
            counter -= 4;
            if (counter <= 0) {
                counter = 4 * (2048 - frequency);
                phase = (phase + 1) % 8;
            }
        }

        void trigger() {
            counter = 4 * (2048 - frequency);
            phase = 0;
        }
    };

    struct DutyCycle {
        uint8_t dutyCycleIndex = 0;

        std::array<std::array<uint8_t, 8>, 4> patterns = {{
            {0, 0, 0, 0, 0, 0, 0, 1}, // 12.5%
            {1, 0, 0, 0, 0, 0, 0, 1}, // 25%
            {1, 0, 0, 0, 0, 1, 1, 1}, // 50%
            {0, 1, 1, 1, 1, 1, 1, 0}  // 75%
        }};

        [[nodiscard]] uint8_t waveformStep(const uint8_t phase) const {
            return patterns[dutyCycleIndex][phase];
        }
    };

    struct LengthCounter {
        bool enabled = false;
        uint8_t counter = 64;

        void load(const uint8_t length) {
            counter = 64 - length;
        }

        void clock(bool& channel_enabled) {
            if (!enabled || counter == 0) {
                return;
            }

            counter -= 1;
            if (counter == 0) {
                channel_enabled = false;
            }
        }

        void trigger() {
            if (counter == 0) {
                counter = 64;
            }
        }
    };

    struct Envelope {
        enum Direction {
            Decreasing,
            Increasing
        };
        // Current counter values
        uint8_t volume = 0;
        uint8_t counter = 0;
        uint8_t period = 0;
        Direction direction = Decreasing;
        // Configured value via registers
        uint8_t startingVolume = 0;
        uint8_t configuredPeriod = 0;
        Direction configuredDirection = Decreasing;

        void clock() {
            if (period == 0) {
                return;
            }

            counter -= 1;
            if (counter == 0) {
                counter = period;
            }

            if ((direction == Decreasing && volume == 0) || direction == Increasing && volume == 15) {
                // Nothing
            } else if (direction == Increasing) {
                volume += 1;
            } else if (direction == Decreasing) {
                volume -= 1;
            }
        }
        void trigger() {
            volume = startingVolume;
            direction = configuredDirection;
            period = configuredPeriod;

            counter = period;
        }
    };

    bool enabled = true;
    std::optional<Sweep> sweep;
    PulsePhaseTimer timer;
    DutyCycle dutyCycle;
    LengthCounter lengthCounter;
    Envelope envelope;

    friend class APU;
};


