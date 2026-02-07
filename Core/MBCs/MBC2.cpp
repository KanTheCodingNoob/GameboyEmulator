//
// Created by ADMIN on 2/7/2026.
//

#include "MBC2.h"

MBC2::MBC2(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram):
MBC(rom, eram) {}

MBC2::~MBC2()
= default;

uint8_t MBC2::read(const uint16_t addr)
{
    if (addr <= 0x3FFF)
    {
        return rom[addr];
    }
    if (addr >= 0x4000 && addr <= 0x7FFF)
    {
        uint8_t bankNumber = romBankNumber & 0x0F;
        if (bankNumber == 0) bankNumber = 1;        // bank 0 becomes bank 1
        bankNumber %= romBankCount;

        const uint32_t index = (bankNumber * 0x4000) + (addr - 0x4000);
        return rom[index];
    }
    if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        if (!ramEnabled)
            return 0xFF;

        const uint16_t index = (addr - 0xA000) & 0x01FF; // 512 x 4-bit
        return 0xF0 | (builtInRam[index] & 0x0F);
    }
    return 0xFF;
}

void MBC2::write(const uint16_t addr, const uint8_t data)
{
    if (addr <= 0x3FFF)
    {
        if (addr & 0x0100) // Check if the 8th bit of the address is set
        {
            romBankNumber = data & 0x0F;
        } else
        {
            ramEnabled = (data & 0xF) == 0x0A;
        }
    }
}