//
// Created by kan on 2/18/26.
//

#include "NoiseChannel.h"

void NoiseChannel::clock() {
    timer.tick();
}

void NoiseChannel::trigger() {
    enabled = true;
    timer.trigger();
    lengthCounter.trigger();
    envelope.trigger();
}

uint8_t NoiseChannel::sample() {
    if (!enabled || lengthCounter.counter == 0) {
        return 0;
    }

    // Output is the inverted low bit of the LFSR
    uint8_t output = (timer.lfsr & 1) ? 0 : 1;
    return output * envelope.volume;
}

void NoiseChannel::setInitialLengthTimer(uint8_t lengthTimer) {
    lengthCounter.load(lengthTimer);
}

void NoiseChannel::setInitialVolume(uint8_t initialVolume) {
    envelope.startingVolume = initialVolume;
}

void NoiseChannel::setEnvDirection(bool envDirection) {
    envelope.configuredDirection = static_cast<Envelope::Direction>(envDirection);
}

void NoiseChannel::setEnvPace(uint8_t pace) {
    envelope.configuredPeriod = pace;
}

void NoiseChannel::setLfsrParameters(uint8_t divisorCode, bool widthMode, uint8_t shiftClockFreq) {
    timer.divisorCode = divisorCode;
    timer.widthMode = widthMode;
    timer.shiftClockFreq = shiftClockFreq;
}

void NoiseChannel::lengthEnable(bool enable) {
    lengthCounter.enabled = enable;
}