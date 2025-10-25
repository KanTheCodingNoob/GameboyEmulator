//
// Created by ADMIN on 10/23/2025.
//

#pragma once
#include <array>
#include <cstdint>

#include "Bus.h"

class Bus;

class PPU {
public:
    explicit PPU(Bus* bus);
    uint16_t returnVRAMAddress(uint8_t addr, bool object);
    std::array<uint8_t, 8> returnPixelValuesFromTwoBytes(uint8_t topLine, uint8_t secondLine);
    ~PPU() = default;

    uint32_t bgColorHandler(uint8_t value);

private:
    Bus* bus;
    const std::array<uint32_t, 4> colorPalette = {0x0f380f, 0x306230, 0x8bac0f, 0x9bbc0f};

    uint8_t& LCDC = bus->IORegisters[0x40];
    uint8_t& STAT = bus->IORegisters[0x41];
    uint8_t& SCY = bus->IORegisters[0x42];
    uint8_t& SCX = bus->IORegisters[0x43];
    uint8_t& LY = bus->IORegisters[0x44];
    uint8_t& LYC = bus->IORegisters[0x45];
    uint8_t& DMA = bus->IORegisters[0x46];
    uint8_t& BGP = bus->IORegisters[0x47];
    uint8_t& OBP0 = bus->IORegisters[0x48];
    uint8_t& OBP1 = bus->IORegisters[0x49];
    uint8_t& WY = bus->IORegisters[0x4A];
    uint8_t& WX = bus->IORegisters[0x4B];
};
