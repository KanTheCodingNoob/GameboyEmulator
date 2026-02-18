#include "APU.h"
#include "Bus.h"

APU::APU(Bus* bus) : bus(bus)
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
                    , WaveRAM(&bus->IORegisters[0x30]) {
}

void APU::reset() {

}

void APU::clock() {

}

void APU::write(uint16_t addr, uint8_t data) {

}

uint8_t APU::read(uint16_t addr) {
    return 0;
}