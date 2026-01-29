//
// Created by ADMIN on 10/5/2025.
//

#include "MBC1.h"

MBC1::MBC1(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram):
MBC(rom, eram) {}

MBC1::~MBC1()
= default;

uint8_t MBC1::read(const uint16_t addr)
{
    // 0000–3FFF (ROM Bank 00 or Banks 00/20/40/60)
    if (addr <= 0x3FFF)
    {
        if (!bankingMode) // Mode 0
        {
            return rom[addr];
        }
        else // Mode 1
        {
            uint8_t bankNumber = (ramBankNumber << 5) % romBankCount;
            const uint32_t index = (bankNumber * 0x4000) + addr;
            return rom[index];
        }
    }

    // 4000–7FFF (ROM Bank 01-7F)
    if (addr <= 0x7FFF)
    {
        uint8_t bankNumber = romBankNumber;
        if (bankNumber == 0) bankNumber = 1;
        bankNumber |= (ramBankNumber << 5);
        bankNumber %= romBankCount;

        const uint32_t index = (bankNumber * 0x4000) + (addr - 0x4000);
        return rom[index];
    }

    // A000–BFFF (RAM Bank 00-03)
    if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        if (!ramEnable || eram.empty()) return 0xFF;

        uint32_t ramOffset = 0;
        if (bankingMode) // Mode 1
        {
            ramOffset = ramBankNumber * 0x2000;
        }

        const uint32_t index = ramOffset + (addr - 0xA000);
        if (index < eram.size())
            return eram[index];

        return 0xFF;
    }

    return 0xFF; // Garbage data
}

void MBC1::write(const uint16_t addr, const uint8_t data)
{
    if (addr < 0x2000)
    {
        ramEnable = (data & 0x0F) == 0x0A;
        return;
    }
    if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        romBankNumber = data & 0x1F; // Extract the first 5 bit
        return;
    }
    if (addr >= 0x4000 && addr <= 0x5FFF)
    {
        ramBankNumber = data & 0x03; // Extract the first 2 bit
        return;
    }
    if (addr >= 0x6000 && addr <= 0x7FFF) // Extract the first 1 bit
    {
        bankingMode = data & 0x01;
    }
    if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        if (ramEnable && !eram.empty())
        {
            uint32_t ramOffset = 0;
            if (bankingMode) // Mode 1
            {
                ramOffset = ramBankNumber * 0x2000;
            }

            const uint32_t index = ramOffset + (addr - 0xA000);
            if (index < eram.size())
                eram[index] = data;
        }
    }
}
