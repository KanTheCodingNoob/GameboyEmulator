//
// Created by ADMIN on 10/5/2025.
//

#include "MBC1.h"

MBC1::MBC1(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram):
MBC(rom, eram) {}

MBC1::~MBC1()
= default;

uint8_t MBC1::read(uint16_t addr)
{
    return 0;
}

void MBC1::write(uint16_t addr, uint8_t data)
{
    // TODO
}

