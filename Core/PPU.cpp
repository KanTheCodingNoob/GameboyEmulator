//
// Created by ADMIN on 10/23/2025.
//

#include "PPU.h"

#include <algorithm>

#include "Bus.h"

PPU::PPU(Bus* bus): bus(bus),LCDC (bus->IORegisters[0x40]),
                    STAT(bus->IORegisters[0x41]),
                    SCY ( bus->IORegisters[0x42]),
                    SCX ( bus->IORegisters[0x43]),
                    LY ( bus->IORegisters[0x44]),
                    LYC ( bus->IORegisters[0x45]),
                    DMA ( bus->IORegisters[0x46]),
                    BGP ( bus->IORegisters[0x47]),
                    OBP0 ( bus->IORegisters[0x48]),
                    OBP1 ( bus->IORegisters[0x49]),
                    WY ( bus->IORegisters[0x4A]),
                    WX ( bus->IORegisters[0x4B])
{
}

uint16_t PPU::returnVRAMAddress(const uint8_t addr, const bool object)
{
    if (object || (LCDC & 0x10))
    {
        return 0x8000 + addr;
    }
    return 0x9000 + static_cast<int8_t>(addr);
}

std::array<uint8_t, 8> PPU::returnPixelValuesFromTwoBytes(const uint8_t topLine, const uint8_t secondLine)
{
    std::array<uint8_t, 8> pixels{};
    for (int i = 0; i < 8; i++)
    {
        const uint8_t lowBit  = (topLine >> (7 - i)) & 1;
        const uint8_t highBit = (secondLine >> (7 - i)) & 1;
        pixels[i] = (highBit << 1) | lowBit;
    }
    return pixels;
}

uint32_t PPU::bgColorHandler(uint8_t value)
{
}

void PPU::clock()
{
    switch (mode)
    {
        // OAM Scan
        case 2:
            OAMSearch();
            break;
        case 3:
            PixelTransfer();
            break;
        case 0:
            HBlank();
            break;
        case 1:
            VBlank();
            break;
        default:
            break;
    }
    updateSTAT();
    checkSTATInterrupt();
}

// Scan all OBJ in one go, then keep track of the cycle until it reaches 20 clocks
// then move on to mode 3 (M-cycles clocks)
void PPU::OAMSearch()
{
    if (OAMCycle == 0)
    {
        OAMInALine.clear();
        // Loop through all OAM Y-Position
        for (int i = 0; i < 160; i = i + 4)
        {
            // Stop after 10 OBJ
            if (OAMInALine.size() == 10) break;

            uint8_t y = bus->OAM[i];
            uint8_t height = 8 * (((LCDC & 0x4) >> 2) + 1);

            // Check if the OBJ Y position is within the scanline
            if (LY + 16 >= y && LY + 16 < bus->OAM[i] + height)
            {
                OAMInALine.push_back(
                    {bus->OAM[i],
                        bus->OAM[i + 1],
                        bus->OAM[i + 2],
                        bus->OAM[i + 3]});
            }
        }
    }
    OAMCycle++;
    if (OAMCycle >= 20)
    {
        OAMCycle = 0;
        // Stable sort them by x position for the Pixel Transfer operation
        std::stable_sort(OAMInALine.begin(), OAMInALine.end(), [](const OBJ& a, const OBJ& b)
        {
            return a.xPosition < b.xPosition;
        });
        mode = 3;
    }
}

// The pixel transfer design is model after one machine clock cycle
void PPU::PixelTransfer()
{
    if (PixelTransferCycle == 0) {
        bgFetcher.reset();
        windowFetcherX = 0;
        discardedPixels = SCX % 8;  // how many pixels to discard from first tile
    }

    // If bg and window are not enabled, just push white pixel
    if (!getLCDCFlags(BGAndWindowEnable))
    {
        for (int i = 0; i < 4; i++)
        {
            pixelFIFO.push(0);
        }
        pushPixels();
        PixelTransferCycle++;
        return;
    }

    stepBGFetcher();
    PixelTransferCycle++;
    pushPixels();
}

void PPU::stepBGFetcher()
{
    // Decide source once per fetch step
    const TileSource src = usingWindow()
        ? getWindowSource()
        : getBGSource();

    switch (PixelTransferCycle % 2)
    {
    case 0: {
            // Fetch tile index
            const uint16_t tilemapAddr =
                src.tilemapBase + src.tileY * 32 + src.tileX;

            bgFetcher.tileIndex = bus->read(tilemapAddr);

            // Select tile data base (LCDC.4)
            if (LCDC & 0x10) {
                bgFetcher.tileAddr =
                    0x8000 +
                    bgFetcher.tileIndex * 16 +
                    src.tileLine * 2;
            } else {
                bgFetcher.tileAddr =
                    0x9000 +
                    static_cast<int8_t>(bgFetcher.tileIndex) * 16 +
                    src.tileLine * 2;
            }

            bgFetcher.lowByte = bus->read(bgFetcher.tileAddr);
            break;
    }

    case 1: {
            // Fetch high byte
            bgFetcher.highByte = bus->read(bgFetcher.tileAddr + 1);

            // Push 8 pixels MSB → LSB
            for (int bit = 7; bit >= 0; bit--) {
                const uint8_t color =
                    ((bgFetcher.highByte >> bit) & 1) << 1 |
                    ((bgFetcher.lowByte  >> bit) & 1);

                pixelFIFO.push(color);
            }

            // Advance tile X depending on source
            if (usingWindow()) {
                windowFetcherX++;
            } else {
                bgFetcher.fetcherX++;
            }

            break;
        }
    default: break;
    }
}

void PPU::stepOBJFetcher(const OBJ& obj)
{
    const bool is8x16 = LCDC & 0x04;

    uint8_t tileIndex = obj.tileIndex;
    if (is8x16)
        tileIndex &= 0xFE;

    const int spriteY = LY + 16;
    int line = spriteY - obj.yPosition;

    // Priority
    bool priority = getAttributeFlags(Priority, obj.attributes);

    // Y flip
    if (getAttributeFlags(YFlip, obj.attributes))
        line = is8x16 ? (15 - line) : (7 - line);

    if (is8x16 && line >= 8) {
        tileIndex++;
        line -= 8;
    }

    // X flip
    const bool xFlip = getAttributeFlags(XFlip, obj.attributes);

    // DMG Palette
    const uint8_t obp =
    getAttributeFlags(DMGPalette, obj.attributes)
        ? OBP1
        : OBP0;

    const uint16_t tileAddr =
        0x8000 +
        tileIndex * 16 +
        line * 2;

    const uint8_t low  = bus->read(tileAddr);
    const uint8_t high = bus->read(tileAddr + 1);

    // Cover for overlapping OBJ, in which case the obj with the lower x coordinate take priority
    const int pixelIgnored = static_cast<int>(objectFIFO.size());

    for (int i = pixelIgnored; i < 8; i++)
    {
        const int bit = xFlip ? i : (7 - i); // Reversed the order of the color being pushed

        uint8_t color =
            ((high >> bit) & 1) << 1 |
            ((low  >> bit) & 1);

        if (color == 0) {
            objectFIFO.push({TRANSPARENT, priority});
            continue;
        }

        const uint8_t shade = (obp >> (color * 2)) & 0x03;
        objectFIFO.push({shade, priority});
    }
}

TileSource PPU::getBGSource() const
{
    const uint8_t bgY = (SCY + LY) & 0xFF;
    const uint16_t tilemapBase = (getLCDCFlags(BGTileMapArea)) ? 0x9C00 : 0x9800;
    const uint8_t tileX = ((SCX >> 3) + bgFetcher.fetcherX) & 0x1F;
    const uint8_t tileY = (bgY >> 3) & 0x1F;
    const uint8_t tileLine = bgY & 7;

    return {
        tilemapBase,
        tileX,
        tileY,
        tileLine
    };
}

TileSource PPU::getWindowSource() const
{
    const uint8_t winY = windowLineCounter;
    const uint16_t tilemapBase = (getLCDCFlags(WindowTileMapArea)) ? 0x9C00 : 0x9800;
    const uint8_t tileY = (winY >> 3) & 0x1F;
    const uint8_t tileLine = winY & 7;

    return {
        tilemapBase,
        windowFetcherX,
        tileY,
        tileLine
    };
}

bool PPU::usingWindow() const
{
    return getLCDCFlags(WindowEnable) &&
           LY >= WY &&
           currentX >= WX - 7;
}

void PPU::pushPixels()
{
    for (int i = 0; i < 4; i++)
    {
        if (LY >= 144) return;
        if (mode != 3) return;

        // Check window has been reached per pixel
        if (!windowActive &&
            getLCDCFlags(WindowEnable) &&
            LY >= WY &&
            currentX == WX - 7)
        {
            windowActive = true;

            // Hard reset fetcher pipeline
            windowFetcherX = 0;
            PixelTransferCycle = 0;

            pixelFIFO = {};
        }

        // Check if any object is pending
        if (getLCDCFlags(OBJEnable) &&
            !OAMInALine.empty() &&
            OAMInALinePointer >= 0 &&
            currentX == OAMInALine.at(OAMInALinePointer).xPosition - 8)
        {
            stepOBJFetcher(OAMInALine.at(OAMInALinePointer));
            OAMInALinePointer++;
            if (OAMInALinePointer == OAMInALine.size())
            {
                OAMInALinePointer = -1;
            }
        }

        if (pixelFIFO.size() >= 8)
        {
            if (discardedPixels > 0) {
                pixelFIFO.pop();
                discardedPixels--;
                continue;
            }

            // If object fifo has a pixel, use it instead of bg pixel
            uint8_t pixel;
            if (!objectFIFO.empty()) {
                pixel = objectFIFO.front().color;
                if (pixel == TRANSPARENT || (objectFIFO.front().priority && pixelFIFO.front() != 0))
                {
                    pixel = pixelFIFO.front();
                }
                objectFIFO.pop();
            } else {
                pixel = pixelFIFO.front();
            }
            pixelFIFO.pop();

            LCD[LY][currentX] = colorPalette[pixel];
            currentX++;

            if (currentX == 160)
            {
                PixelTransferToHBlankTransition();
                return;
            }
        }
    }
}

void PPU::PixelTransferToHBlankTransition()
{
    if (windowActive) { // Only increment if the window was active this scanline
        windowLineCounter++;
    }
    OAMInALinePointer = 0;
    windowActive = false;
    currentX = 0;
    mode = 0;
    HBlankCycle = 94 - PixelTransferCycle; // Calculate the HBlank cycle needed based on the PixelTransferCycle
    PixelTransferCycle = 0;
    pixelFIFO = {};
}

void PPU::HBlank()
{
    HBlankCycle--;
    if (HBlankCycle == 0)
    {
        LY++;
        if (LY >= 144)
        {
            mode = 1;
            bus->interrupt.requestVBlankInterrupt();
        } else
        {
            windowActive = false;
            mode = 2;
        }
    }
}

void PPU::VBlank()
{
    VBlankCycle++;
    if (VBlankCycle >= 144)
    {
        VBlankCycle = 0;
        LY++;
        if (LY > 0x99)
        {
            windowLineCounter = 0;
            mode = 2;
            LY = 0;
        }
    }
}

bool PPU::getLCDCFlags(const LCDCFlags f) const
{
    return (LCDC & f) != 0;
}

bool PPU::getSTATFlags(const STATFlags f) const
{
    return (STAT & f) != 0;
}

bool PPU::getAttributeFlags(const AttributeFlags f, const uint8_t& OBJFlags)
{
    return (OBJFlags & f) != 0;
}

void PPU::setSTATFlags(const STATFlags f, const bool value) const
{
    if (value)
    {
        STAT |= f;
    } else
    {
        STAT &= ~f;
    }
}

void PPU::checkSTATInterrupt()
{
    const bool statCondition =
         ((LYC == LY) && getSTATFlags(LYCSelect)) ||
         ((mode == 2) && getSTATFlags(Mode2Select)) ||
         ((mode == 1) && getSTATFlags(Mode1Select)) ||
         ((mode == 0) && getSTATFlags(Mode0Select));

    if (statCondition && !prevSTATCondition)
    {
        bus->interrupt.requestSTATInterrupt();
    }

    prevSTATCondition = statCondition;
}

void PPU::updateSTAT()
{
    setSTATFlags(LYEqualsLYC, (LY == LYC));
    setSTATFlags(PPUModeBit1, (mode & 0x2));
    setSTATFlags(PPUModeBit0, (mode & 0x1));
}
