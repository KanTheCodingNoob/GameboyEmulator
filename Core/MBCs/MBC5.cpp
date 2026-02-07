//
// Created by ADMIN on 2/7/2026.
//

#include "MBC5.h"

MBC5::MBC5(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram):
MBC(rom, eram){}

MBC5::~MBC5() = default;

uint8_t MBC5::read(const uint16_t addr)
{
    if (addr <= 0x3FFF)
    {
        return rom[addr];
    }
    if (addr >= 0x4000 && addr <= 0x7FFF)
    {
        return rom[romBankNumber * 0x4000 + (addr - 0x4000)];
    }
    if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        if (!ramEnabled || eram.empty()) return 0xFF;
        return eram[ramBankNumber * 0x2000 + (addr - 0xA000)];
    }
    return 0xFF;
}

void MBC5::write(const uint16_t addr, const uint8_t data)
{
    if (addr <= 0x1FFF)
    {
        if ((data & 0xF) == 0xA)
        {
            ramEnabled = true;
        } else if ((data & 0xF) == 0x0)
        {
            ramEnabled = false;
        }
        return;
    }
    if (addr >= 0x2000 && addr <= 0x2FFF)
    {
        romBankNumber = (romBankNumber & 0xFF00) | data;
        return;
    }
    if (addr >= 0x3000 && addr <= 0x3FFF)
    {
        romBankNumber = (romBankNumber & 0x00FF) | ((data & 0x01) << 8);
        return;
    }
    if (addr >= 0x4000 && addr <= 0x5FFF)
    {
        ramBankNumber = data & 0x0F;
        return;
    }
    if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        if (!ramEnabled || eram.empty()) return;
        eram[ramBankNumber * 0x2000 + (addr - 0xA000)] = data;
        return;
    }
}