//
// Created by kan on 2/18/26.
//

#include "PulseWaveChannel.h"

void PulseWaveChannel::clock() {
    timer.clock();
    lengthCounter.clock();
    envelope.clock();
}

void PulseWaveChannel::trigger() {
    enabled = true;

    timer.trigger();
    lengthCounter.trigger();
    envelope.trigger();
}

uint8_t PulseWaveChannel::sample() {
    if (!enabled) {
        return 0;
    }

    return dutyCycle.waveformStep(timer.phase) * envelope.volume;
}