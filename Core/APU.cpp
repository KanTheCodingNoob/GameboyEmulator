#include "APU.h"
#include "Bus.h"

APU::APU(Bus* bus) : bus(bus)
                     , ch1(true)
                     , ch2(false)
                     , NR10(bus->IORegisters[0x10])
                     , NR11(bus->IORegisters[0x11])
                     , NR12(bus->IORegisters[0x12])
                     , NR13(bus->IORegisters[0x13])
                     , NR14(bus->IORegisters[0x14])
                     , NR21(bus->IORegisters[0x16])
                     , NR22(bus->IORegisters[0x17])
                     , NR23(bus->IORegisters[0x18])
                     , NR24(bus->IORegisters[0x19])
                     , NR30(bus->IORegisters[0x1A])
                     , NR31(bus->IORegisters[0x1B])
                     , NR32(bus->IORegisters[0x1C])
                     , NR33(bus->IORegisters[0x1D])
                     , NR34(bus->IORegisters[0x1E])
                     , NR41(bus->IORegisters[0x20])
                     , NR42(bus->IORegisters[0x21])
                     , NR43(bus->IORegisters[0x22])
                     , NR44(bus->IORegisters[0x23])
                     , NR50(bus->IORegisters[0x24])
                     , NR51(bus->IORegisters[0x25])
                     , NR52(bus->IORegisters[0x26])
                     , WaveRAM(&bus->IORegisters[0x30]
                     ) {
    ch3.setWaveRAM(WaveRAM);
}

void APU::reset() {

}

void APU::clock() {
    ch1.clock();
    ch2.clock();
    ch3.clock();
    ch4.clock();

    frameSequencer.tick(ch1);
    frameSequencer.tick(ch2);
    frameSequencer.tick(ch3);
    frameSequencer.tick(ch4);

    cycleCounter++;
    while (cycleCounter >= cyclesPerSample) {
        cycleCounter -= cyclesPerSample;
        generateAudioSample();
    }
}

void APU::generateAudioSample() {
    const float s1 = ch1.sample();
    const float s2 = ch2.sample();
    const float s3 = ch3.sample();
    const float s4 = ch4.sample();

    float left = 0.0f, right = 0.0f;

    left  += (NR51 >> 4) & 1 ? s1 : 0.0f;
    left  += (NR51 >> 5) & 1 ? s2 : 0.0f;
    left  += (NR51 >> 6) & 1 ? s3 : 0.0f;
    left  += (NR51 >> 7) & 1 ? s4 : 0.0f;

    right += (NR51 >> 0) & 1 ? s1 : 0.0f;
    right += (NR51 >> 1) & 1 ? s2 : 0.0f;
    right += (NR51 >> 2) & 1 ? s3 : 0.0f;
    right += (NR51 >> 3) & 1 ? s4 : 0.0f;

    // NR50: bits 6-4 = left volume, bits 2-0 = right volume (0-7 → 1-8)
    const float leftVol  = ((NR50 >> 4) & 0x07) + 1;
    const float rightVol = ((NR50 >> 0) & 0x07) + 1;

    left  = (left  / 4.0f / 15.0f) * 2.0f - 1.0f;
    right = (right / 4.0f / 15.0f) * 2.0f - 1.0f;

    // Scale by master volume (max vol = 8, so divide by 8 to keep in -1..1)
    left  *= leftVol  / 8.0f;
    right *= rightVol / 8.0f;

    audioBuffer.push_back(left);
    audioBuffer.push_back(right);
}

std::vector<float>& APU::getAudioBuffer() {
    return audioBuffer;
}

void APU::clearAudioBuffer() {
    audioBuffer.clear();
}

void APU::write(const uint16_t addr, const uint8_t data) {
    switch (addr) {
        // Channel 1
        case 0xFF10:
            return;
        case 0xFF11: {
            const uint8_t lengthTimer = data & 0b00111111; // Low 6 bit
            ch1.setInitialLengthTimer(lengthTimer);
            const uint8_t waveDuty = (data >> 6); // High 2 bit
            ch1.setWaveDuty(waveDuty);
            NR11 = data;
            return;
        }
        case 0xFF12: {
            const uint8_t initialVolume = data >> 4; // High 4 bit
            ch1.setInitialVolume(initialVolume);
            const bool envDirection = (data >> 3) & 0b00000001;
            ch1.setEnvDirection(envDirection);
            const uint8_t sweepPace = data & 0b00000111;
            ch1.setSweepPace(sweepPace);
            NR12 = data;
            return;
        }
        case 0xFF13: {
            ch1.setLowPeriodValue(data);
            NR13 = data;
            return;
        }
        case 0xFF14: {
            if (data >> 7) {
                ch1.trigger();
            }

            ch1.lengthEnable((data >> 6) & 0x01);
            const uint8_t period = data & 0b00000111;
            ch1.setHighPeriodValue(period);
            NR14 = data;
            return;
        }
            // Channel 2 (Pretty much just channel 1 without the sweep)
        case 0xFF16: {
            const uint8_t lengthTimer = data & 0b00111111; // Low 6 bit
            ch2.setInitialLengthTimer(lengthTimer);
            const uint8_t waveDuty = (data >> 6); // High 2 bit
            ch2.setWaveDuty(waveDuty);
            NR21 = data;
            return;
        }
        case 0xFF17: {
            const uint8_t initialVolume = data >> 4; // High 4 bit
            ch2.setInitialVolume(initialVolume);
            const bool envDirection = (data >> 3) & 0b00000001;
            ch2.setEnvDirection(envDirection);
            const uint8_t sweepPace = data & 0b00000111;
            ch2.setSweepPace(sweepPace);
            NR22 = data;
            return;
        }
        case 0xFF18: {
            ch2.setLowPeriodValue(data);
            NR23 = data;
            return;
        }
        case 0xFF19: {
            if (data >> 7) {
                ch2.trigger();
            }

            ch2.lengthEnable((data >> 6) & 0x01);
            const uint8_t period = data & 0b00000111;
            ch2.setHighPeriodValue(period);
            NR24 = data;
            return;
        }
            // Channel 3
        case 0xFF1A: {
            ch3.dacEnable(data >> 7);
            NR30 = data;
            return;
        }
        case 0xFF1B: {
            ch3.setInitialLengthTimer(data);
            NR31 = data;
            return;
        }
        case 0xFF1C: {
            ch3.setOutputLevel((data >> 5) & 0x03);
            NR32 = data;
            return;
        }
        case 0xFF1D: {
            ch3.setLowPeriodValue(data);
            NR33 = data;
            return;
        }
        case 0xFF1E: {
            if (data >> 7) {
                ch3.trigger();
            }
            ch3.lengthEnable((data >> 6) & 0x01);
            ch3.setHighPeriodValue(data & 0x07);
            NR34 = data;
            return;
        }
            // Channel 4
        case 0xFF20: {
            ch4.setInitialLengthTimer(data & 0x3F);
            NR41 = data;
            return;
        }
        case 0xFF21: {
            ch4.setInitialVolume(data >> 4);
            ch4.setEnvDirection((data >> 3) & 1);
            ch4.setEnvPace(data & 0x7);
            NR42 = data;
            return;
        }
        case 0xFF22: {
            ch4.setLfsrParameters(data & 0x7, (data >> 3) & 1, data >> 4);
            NR43 = data;
            return;
        }
        case 0xFF23: {
            if (data >> 7) {
                ch4.trigger();
            }
            ch4.lengthEnable((data >> 6) & 1);
            NR44 = data;
            return;
        }
            // Global setting
        case 0xFF24: {
            NR50 = data;
        }
        case 0xFF25: {
            NR51 = data;
        }
        case 0xFF26: {
            NR52 = data;
        }
        default:
            break;
    }
    if (addr >= 0xFF30 && addr <= 0xFF3F) {
        WaveRAM[addr - 0xFF30] = data;
    }
}

uint8_t APU::read(uint16_t addr) {
    switch (addr) {
        case 0xFF26: return NR52 |
                ((ch4.getChannelEnabled() << 3) |
                    (ch3.getChannelEnabled() << 2) |
                    (ch2.getChannelEnabled() << 1) |
                    (ch1.getChannelEnabled()));
        default:
            return bus->io.directIORead(addr);
    }
}