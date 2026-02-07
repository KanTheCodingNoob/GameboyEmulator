//
// Created by ADMIN on 2/7/2026.
//

#include "MBC3.h"

MBC3::MBC3(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram):
MBC(rom, eram) {}

MBC3::~MBC3() = default;

uint8_t MBC3::read(const uint16_t addr)
{
    if (addr <= 0x3FFF)
    {
        return rom[addr];
    }
    if (addr >= 0x4000 && addr <= 0x7FFF)
    {
        uint8_t bankNumber = romBankNumber % romBankCount;
        if (bankNumber == 0) bankNumber = 1;
        const uint32_t index = (bankNumber * 0x4000) + (addr - 0x4000);
        return rom[index];
    }
    if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        // TODO RTC
        if (!ramAndRTCEnabled)
            return 0xFF;

        if (ramBankNumber >= 0x08 && ramBankNumber <= 0x0C)
            return 0xFF;   // RTC selected → not RAM

        const uint8_t bankNumber = ramBankNumber & 0x03;
        const uint32_t index = (bankNumber * 0x2000) + (addr - 0xA000);

        return eram[index];
    }
    return 0xFF;
}

void MBC3::write(const uint16_t addr, const uint8_t data)
{
    if (addr <= 0x1FFF)
    {
        if ((data & 0xF) == 0xA)
        {
            ramAndRTCEnabled = true;
        } else if ((data & 0xF) == 0x0)
        {
            ramAndRTCEnabled = false;
        }
        return;
    }
    if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        romBankNumber = (data & 0x7F);
        if (romBankNumber == 0) romBankNumber = 1;
        return;
    }
    if (addr >= 0x4000 && addr <= 0x5FFF)
    {
        if (const uint8_t value = data & 0xF; value <= 0x07)
        {
            ramBankNumber = value & 0x3;
            return;
        } else if (value <= 0x0C)
        {
            // TODO RTC
            return;
        }
        return;
    }
    if (addr >= 0x6000 && addr <= 0x7FFF)
    {
        // TODO RTC
        return;
    }
    if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        if (!ramAndRTCEnabled)
            return;

        if (ramBankNumber >= 0x08 && ramBankNumber <= 0x0C)
        {
            // TODO RTC register write
            return;
        }

        const uint8_t bank = ramBankNumber & 0x03;
        const uint32_t index = (bank * 0x2000) + (addr - 0xA000);

        eram[index] = data;
        return;
    }
}
