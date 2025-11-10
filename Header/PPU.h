//
// Created by ADMIN on 10/23/2025.
//

#pragma once
#include <array>
#include <cstdint>
#include <queue>

struct OBJ
{
    uint8_t yPosition; // Byte 0
    uint8_t xPosition; // Byte 1
    uint8_t tileIndex; // Byte 2
    uint8_t attributes; // Byte 3
};

class Bus;

class PPU {
public:
    explicit PPU(Bus* bus);

    static constexpr std::array<uint32_t, 4> colorPalette = {0xff0f380f, 0xff306230, 0xff8bac0f, 0xff9bbc0f};
    uint32_t LCD[144][160];

    uint16_t returnVRAMAddress(uint8_t addr, bool object);
    std::array<uint8_t, 8> returnPixelValuesFromTwoBytes(uint8_t topLine, uint8_t secondLine);
    ~PPU() = default;

    uint32_t bgColorHandler(uint8_t value);
    // M-cycle clock
    void clock();
private:
    Bus* bus;

    // PPU Register
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

    std::queue<uint8_t> pixelFIFO;
    std::vector<OBJ> OAMInALine;

    uint8_t mode = 2;
    // Rendering mode
    // Mode 2
    void OAMSearch();
    // Track the amount of cycle passed within each OAM Operation
    uint16_t OAMCycle = 0;

    // Mode 3
    void PixelTransfer();
    // Track the amount of cycle passed within each Pixel Transfer Operation
    uint16_t PixelTransferCycle = 0;
    void pushPixel();
    uint8_t discardedPixels = 0;

    void HBlank();
    uint16_t HBlankCycle = 0;

    void VBlank();
    uint16_t VBlankCycle = 0;
    // Pixel Fetcher Coordinate
    uint8_t fetcherX = 0;
    // Location of the current pixel in each frame
    uint8_t pixelX = 0;
};
