//
// Created by ADMIN on 2/7/2026.
//

#pragma once
#include "MBC.h"

class MBC5: public MBC
{
    MBC5(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram);
    ~MBC5() override;

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t data) override;
};


