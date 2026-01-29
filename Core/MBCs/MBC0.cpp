//
// Created by ADMIN on 11/12/2025.
//

#include "MBC0.h"

MBC0::MBC0(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram)
: MBC(rom, eram)
{}

MBC0::~MBC0()
= default;

uint8_t MBC0::read(const uint16_t addr)
{
    if (addr < 0x8000) {
        return rom[addr];
    }
    if (addr >= 0xA000 && addr < 0xC000) {
        return eram[addr - 0xA000];
    }
    return 0xFF;
}

void MBC0::write(const uint16_t addr, const uint8_t data)
{
    // Ignore write to ROM in the case of MBC0
    if (addr >= 0xA000 && addr < 0xC000) {
        eram[addr - 0xA000] = data;
    }
}
