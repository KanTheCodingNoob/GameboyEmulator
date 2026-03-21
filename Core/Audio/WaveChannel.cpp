//
// Created by kan on 2/18/26.
//

#include "WaveChannel.h"

void WaveChannel::clock() {
    if (dacOn) {
        timer.tick();
    }
}

void WaveChannel::trigger() {
    enabled = true;
    timer.trigger();
    lengthCounter.trigger();
}

uint8_t WaveChannel::sample() {
    if (!enabled || !dacOn || lengthCounter.counter == 0 || !waveRAM) {
        return 0;
    }

    uint8_t byteIndex = timer.sampleIndex / 2;
    uint8_t nibble = waveRAM[byteIndex];

    if (timer.sampleIndex % 2 == 0) {
        nibble >>= 4; // High nibble first
    } else {
        nibble &= 0x0F; // Low nibble second
    }

    // Apply volume shift
    switch (volumeShift) {
        case 0: // Mute
            return 0;
        case 1: // 100%
            return nibble;
        case 2: // 50%
            return nibble >> 1;
        case 3: // 25%
            return nibble >> 2;
        default:
            return 0;
    }
}

void WaveChannel::setInitialLengthTimer(uint8_t lengthTimer) {
    lengthCounter.load(lengthTimer);
}

void WaveChannel::setOutputLevel(uint8_t outputLevel) {
    volumeShift = outputLevel;
}

void WaveChannel::setLowPeriodValue(uint8_t lowPeriodValue) {
    timer.frequency = (timer.frequency & 0x0700) | lowPeriodValue;
}

void WaveChannel::setHighPeriodValue(uint8_t highPeriodValue) {
    timer.frequency = (timer.frequency & 0x00FF) | ((highPeriodValue & 0x07) << 8);
}

void WaveChannel::lengthEnable(bool enable) {
    lengthCounter.enabled = enable;
}

void WaveChannel::dacEnable(bool enable) {
    dacOn = enable;
}

bool& WaveChannel::getChannelEnabled() {
    return enabled;
}