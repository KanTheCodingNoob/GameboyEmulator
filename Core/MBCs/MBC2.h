//
// Created by ADMIN on 2/7/2026.
//

#pragma once
#include "MBC.h"
#include <array>


class MBC2: public MBC
{
public:
    MBC2(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram);
    ~MBC2() override;

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t data) override;

private:
    uint8_t romBankNumber = 0;
    uint8_t romBankCount = rom.size() / 0x4000;

    std::array<uint8_t, 0x200> builtInRam{};
    bool ramEnabled = true;
};