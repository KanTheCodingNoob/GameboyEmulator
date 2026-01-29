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
    if (!bankingMode) // Simple mode
    {
        // 0000–3FFF (fixed bank 0 in mode 0)
        if (addr <= 0x3FFF)
        {
            return rom[addr];
        }
        // 4000–7FFF (switchable bank)
        if (addr <= 0x7FFF)
        {
            uint8_t bankNumber = romBankNumber;
            if (bankNumber == 0) bankNumber = 1; // In simple mode 0 is treated as 1
            bankNumber %= totalBanks;

            const uint32_t index  = bankNumber * 0x4000 + (addr - 0x4000);
            return rom[index];
        }
        // A000–BFFF (RAM, bank 0 only in mode 0)
        if (addr >= 0xA000 && addr <= 0xBFFF)
        {
            if (!ramEnable) return 0xFF;

            if (const uint32_t index = addr - 0xA000; index < eram.size())
                return eram[index];

            return 0xFF;
        }
    } else // Advanced mode
    {

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
        if (ramEnable)
        {
            if (const uint32_t index = addr - 0xA000; index < eram.size())
                eram[index] = data;
        }
    }
}
