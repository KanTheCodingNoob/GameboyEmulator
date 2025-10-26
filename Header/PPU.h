//
// Created by ADMIN on 10/23/2025.
//

#pragma once
#include <array>
#include <cstdint>

class Bus;

class PPU {
public:
    explicit PPU(Bus* bus);

    const std::array<uint32_t, 4> colorPalette = {0x0f380f, 0x306230, 0x8bac0f, 0x9bbc0f};
    const std::array<std::array<uint8_t, 256>, 256> LCD{};

    uint16_t returnVRAMAddress(uint8_t addr, bool object);
    std::array<uint8_t, 8> returnPixelValuesFromTwoBytes(uint8_t topLine, uint8_t secondLine);
    ~PPU() = default;

    uint32_t bgColorHandler(uint8_t value);

private:
    Bus* bus;

    uint8_t& LCDC;
    uint8_t& STAT;
    uint8_t& SCY;
    uint8_t& SCX;
    uint8_t& LY;
    uint8_t& LYC;
    uint8_t& DMA;
    uint8_t& BGP;
    uint8_t& OBP0;
    uint8_t& OBP1;
    uint8_t& WY;
    uint8_t& WX;
};
