//
// Created by kan on 2/18/26.
//

#pragma once
#include <cstdint>

class NoiseChannel {
public:
    NoiseChannel() = default;
    void clock();
    void trigger();
    uint8_t sample();

    void setInitialLengthTimer(uint8_t lengthTimer);
    void setInitialVolume(uint8_t initialVolume);
    void setEnvDirection(bool envDirection);
    void setEnvPace(uint8_t pace);
    void setLfsrParameters(uint8_t divisorCode, bool widthMode, uint8_t shiftClockFreq);
    void lengthEnable(bool enable);

    bool& getChannelEnabled();

private:
    struct NoiseTimer {
        uint16_t lfsr = 0x7FFF;
        int counter = 0;
        uint8_t divisorCode = 0;
        bool widthMode = false;
        uint8_t shiftClockFreq = 0;

        void tick() {
            counter -= 4;
            if (counter <= 0) {
                reloadCounter();
                // --- Clock the LFSR ---
                // XOR bit 0 and bit 1 together to produce the feedback bit.
                // This is the "linear feedback" in LFSR — it determines the next bit.
                uint8_t result = (lfsr & 1) ^ ((lfsr >> 1) & 1);
                lfsr >>= 1; // Shift the entire register right by 1
                // Feed the result bit back into bit 14 (the top of the 15-bit register)
                lfsr |= (result << 14);
                if (widthMode) {
                    lfsr &= ~0x40;  // Clear bit 6
                    lfsr |= (result << 6); // Set bit 6 to the feedback bit
                }
            }
        }

        void trigger() {
            lfsr = 0x7FFF;
            reloadCounter();
        }

        void reloadCounter() {
            static const uint8_t divisors[] = {8, 16, 32, 48, 64, 80, 96, 112};
            counter = divisors[divisorCode & 0x7] << shiftClockFreq;
        }
    };

    struct LengthCounter {
        bool enabled = false;
        uint8_t counter = 64;

        void load(const uint8_t length) {
            counter = 64 - (length & 0x3F);
        }

        void clock(bool &channel_enabled) {
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
            Decreasing = 0,
            Increasing = 1
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

            if (counter > 0) {
                counter -= 1;
            }

            if (counter == 0) {
                counter = period;

                if (direction == Increasing && volume < 15) {
                    volume += 1;
                } else if (direction == Decreasing && volume > 0) {
                    volume -= 1;
                }
            }
        }
        void trigger() {
            volume = startingVolume;
            direction = configuredDirection;
            period = configuredPeriod;

            counter = period;
        }
    };

    bool enabled = false;
    NoiseTimer timer;
    LengthCounter lengthCounter;
    Envelope envelope;

    friend class APU;
};
