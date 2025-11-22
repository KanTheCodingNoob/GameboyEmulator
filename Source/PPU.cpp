//
// Created by ADMIN on 10/23/2025.
//

#include "../Header/PPU.h"

#include <algorithm>

#include "../Header/Bus.h"

PPU::PPU(Bus* bus): bus(bus), LCDC (bus->IORegisters[0x40]),
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
    static uint8_t tileIndex = 0;
    static uint16_t tileAddr = 0;
    static uint8_t lowByte = 0;
    static uint8_t highByte = 0;
    if (PixelTransferCycle == 0) {
        fetcherX = 0;
        discardedPixels = SCX % 8;  // how many pixels to discard from first tile
    }

    // Cycle 0 -> Get Tile and Tile data low
    // Cyle 1 -> Get Tile Data High and Sleep
    // Each cycle push pixel 2 time
    switch (PixelTransferCycle % 2)
    {
        case 0:
            {
                // Get Tile

                // Determine background position
                uint8_t bgY = (SCY + LY) & 0xFF; // Which pixel row in the 256x256 BG
                uint8_t tileRow = (bgY >> 3) & 0x1F; // Which tile row
                uint8_t tileLine = bgY & 0x07; // Which line inside the 8x8 tile


                // Which tile column
                uint8_t tileCol = ((SCX >> 3) + fetcherX) & 0x1F;

                // Select tile map base (LCDC.3)
                // Each tilemap entry = 1 byte (tile index)
                uint16_t tilemapBase = (LCDC & 0x08) ? 0x9C00 : 0x9800;
                uint16_t tilemapAddr = tilemapBase + (tileRow * 32) + tileCol;

                tileIndex = bus->read(tilemapAddr);

                // Get Tile Data Low
                // Select tile data base (LCDC.4)
                if (LCDC & 0x10) {
                    tileAddr = 0x8000 + (tileIndex * 16) + (tileLine * 2);
                } else {
                    tileAddr = 0x9000 + (static_cast<int8_t>(tileIndex) * 16) + (tileLine * 2);
                }

                lowByte = bus->read(tileAddr);
                break;
            }
        case 1:
            {
                // Get Tile Data High and Push Into Fifo
                highByte = bus->read(tileAddr + 1);
                for (int bit = 7; bit >= 0; bit--) {
                    uint8_t color_id =
                        ((highByte >> bit) & 1) << 1 |
                        ((lowByte  >> bit) & 1);

                    // Push to FIFO (for rendering pipeline)
                    pixelFIFO.push(color_id);
                }
                fetcherX++;
                // Sleep
                break;
            }
        default:
            break;
    }
    PixelTransferCycle++;
    pushPixels();
}

// Push 4 pixel per M-cycle to the LCD
void PPU::pushPixels()
{
    for (int i = 0; i < 4; i++)
    {
        if (LY >= 144) return; // prevent out-of-bounds
        if (mode != 3) return; // Return the mode is not 3
        if (pixelFIFO.size() >= 8) // Only push when the FIFO has 8 pixel value or more
        {
            if (discardedPixels > 0) {
                pixelFIFO.pop();
                discardedPixels--;
                return; // Skip drawing until SCX alignment done
            }

            const uint8_t pixel = pixelFIFO.front();      // get the front value
            pixelFIFO.pop();                        // then remove it
            LCD[LY][pixelX] = colorPalette[pixel];        // render
            pixelX++;
            if (pixelX == 160)
            {
                pixelX = 0;
                mode = 0;
                HBlankCycle = 94 - PixelTransferCycle; // Calculate the HBlank cycle needed based on the PixelTransferCycle
                PixelTransferCycle = 0;
            }
        }
    }
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
            bus->interrupt.requestInterrupt(0);
        } else
        {
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
            mode = 2;
            LY = 0;
        }
    }
}

uint8_t PPU::getLCDCFlags(LCDCFlags f) const
{
    return ((LCDC & f) > 0) ? 1 : 0;
}

uint8_t PPU::getSTATFlags(STATFlags f) const
{
    return ((STAT & f) > 0) ? 1 : 0;
}
