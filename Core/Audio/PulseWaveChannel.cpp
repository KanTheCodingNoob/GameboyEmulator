//
// Created by kan on 2/18/26.
//

#include "PulseWaveChannel.h"

#include <iostream>
#include <ostream>

void PulseWaveChannel::clock() {
    timer.tick();
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

void PulseWaveChannel::setInitialLengthTimer(const uint8_t lengthTimer) {
    lengthCounter.load(lengthTimer);
}

void PulseWaveChannel::setWaveDuty(const uint8_t waveDuty) {
    dutyCycle.dutyCycleIndex = waveDuty;
}

void PulseWaveChannel::setInitialVolume(const uint8_t initialVolume) {
    envelope.startingVolume = initialVolume;
}

void PulseWaveChannel::setEnvDirection(bool envDirection) {
    envelope.configuredDirection = static_cast<Envelope::Direction>(envDirection);
}

void PulseWaveChannel::setSweepPace(uint8_t pace) {
    envelope.configuredPeriod = pace;
}

void PulseWaveChannel::setLowPeriodValue(const uint8_t lowPeriodValue) {
    // Keep upper 3 bit within the 11 bit value and set the low 8 bit
    timer.frequency = (timer.frequency & 0x0700) | lowPeriodValue;
}

void PulseWaveChannel::setHighPeriodValue(const uint8_t highPeriodValue) {
    timer.frequency =
        (timer.frequency & 0x00FF) | ((highPeriodValue & 0x07) << 8);
}

void PulseWaveChannel::lengthEnable(const bool enable) {
    lengthCounter.enabled = enable;
}

bool& PulseWaveChannel::getChannelEnabled() {
    return lengthCounter.enabled;
}