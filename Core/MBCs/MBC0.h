//
// Created by ADMIN on 10/5/2025.
//

#pragma once
#include "../MBC.h"


class MBC0: public MBC {
public:
    MBC0(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram);
    ~MBC0() override;

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t data) override;
};