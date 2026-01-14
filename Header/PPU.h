//
// Created by ADMIN on 10/23/2025.
//

#pragma once
#include <array>
#include <cstdint>
#include <queue>

struct TileFetcher {
    uint8_t tileIndex = 0;
    uint16_t tileAddr = 0;
    uint8_t lowByte = 0;
    uint8_t highByte = 0;
    uint8_t fetcherX = 0;

    void reset() {
        fetcherX = 0;
    }
};

struct TileSource {
    uint16_t tilemapBase;
    uint8_t tileX;
    uint8_t tileY;
    uint8_t tileLine;
};

class Bus;

class PPU {
public:
    explicit PPU(Bus* bus);
    ~PPU() = default;

    static constexpr std::array<uint32_t, 4> colorPalette = {0xffffffff, 0xffa9a9a9, 0xff545454, 0xff000000};
    uint32_t LCD[144][160]{};

    uint16_t returnVRAMAddress(uint8_t addr, bool object);
    std::array<uint8_t, 8> returnPixelValuesFromTwoBytes(uint8_t topLine, uint8_t secondLine);

    uint32_t bgColorHandler(uint8_t value);
    // M-cycle clock
    void clock();

    struct OBJ
    {
        uint8_t yPosition; // Byte 0
        uint8_t xPosition; // Byte 1
        uint8_t tileIndex; // Byte 2
        uint8_t attributes; // Byte 3
    };

    enum LCDCFlags
    {
        PPUEnable = (1 << 7),
        WindowTileMapArea = (1 << 6),
        WindowEnable = (1 << 5),
        TileDataArea = (1 << 4),
        BGTileMapArea = (1 << 3),
        OBJSize = (1 << 2),
        OBJEnable = (1 << 1),
        BGAndWindowEnable = (1 << 0)
    };

    enum STATFlags
    {
        LYCSelect = (1 << 6),
        Mode2Select = (1 << 5),
        Mode1Select = (1 << 4),
        Mode0Select = (1 << 3),
        LYEqualsLYC = (1 << 2),
        PPUModeBit1 = (1 << 1),
        PPUModeBit0 = (1 << 0),
    };

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
    std::queue<uint8_t> objectFIFO;
    std::vector<OBJ> OAMInALine;
    int OAMInALinePointer = 0;

    TileFetcher bgFetcher;

    uint8_t mode = 2;
    // Rendering mode
    // Mode 2
    void OAMSearch();
    // Track the amount of cycle passed within each OAM Operation
    uint16_t OAMCycle = 0;

    // Mode 3
    void PixelTransfer();
    void stepBGFetcher();
    void stepOBJFetcher(OBJ& obj);
    TileSource getBGSource() const;
    TileSource getWindowSource() const;
    bool usingWindow() const;
    // Track the amount of cycle passed within each Pixel Transfer Operation
    uint16_t PixelTransferCycle = 0;
    void pushPixels();
    void PixelTransferToHBlankTransition();
    uint8_t discardedPixels = 0;

    void HBlank();
    uint16_t HBlankCycle = 0;

    void VBlank();
    uint16_t VBlankCycle = 0;
    // BG pixel Fetcher Coordinate
    uint8_t bgFetcherX = 0;
    // Window active indicator
    uint8_t windowLineCounter = 0;
    bool windowActive = false;
    // Window pixel fetcher coordinate
    uint8_t windowFetcherX = 0;
    // Location of the current pixel in each frame
    uint8_t currentX = 0;
    // Boolean value used to mimic the low to high transition of the STAT register
    bool prevSTATCondition = false;

    bool getLCDCFlags(LCDCFlags f) const;
    bool getSTATFlags(STATFlags f) const;
    void setSTATFlags(STATFlags f, bool value) const;
    void checkSTATInterrupt();
    void updateSTAT();
};
