//
// Created by ADMIN on 10/23/2025.
//

#include "../Header/PPU.h"
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

uint32_t PPU::bgColorHandler(const uint8_t value)
{
    return colorPalette[(BGP >> (value * 2) & 0x03)];
};
