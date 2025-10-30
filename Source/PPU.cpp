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
    BGP = 0b11100100;
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
            break;
    }
}

// Scan 2 OBJ per clock
void PPU::OAMSearch()
{
    // Loop through all OAM Y-Position
    for (int i = OAMCycle * 2 * 4; i < (OAMCycle * 2 + 2) * 4; i = i + 4)
    {
        // Check if the OBJ Y position is within the scanline
        if (LY + 16 >= bus->OAM[i] && LY + 16 < bus->OAM[i] + 8 * (((LCDC & 0x4) >> 2) + 1))
        {
            OAMInALine.push_back({bus->OAM[i], bus->OAM[i + 1], bus->OAM[i + 2], bus->OAM[i + 3]});
        }
        // Stop after 10 OBJ
        if (OAMInALine.size() == 10)
        {
            break;
        }
    }
    OAMCycle++;
    if (OAMCycle == 10)
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

void PPU::PixelTransfer(const std::vector<OBJ>& objs)
{
    int index = 0;
    // Get Tile
    // Get Tile Data Low
    // Get Tile Data High
    // Push
    // Sleep
}
