//
// Created by ADMIN on 10/5/2025.
//

#pragma once
#include "../MBC.h"


class MBC1: public MBC {
public:
    MBC1(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram);
    ~MBC1() override;

    bool bankingMode = false;   // false = simple mode, true = complex mode
    bool ramEnable = false;
    uint8_t romBankNumber = 0;
    uint8_t ramBankNumber = 0; // Can be used as extra 2 bit for rom larger than 512 Kb in mode 0

    uint8_t romBankCount = rom.size() / 0x4000;
    uint8_t ramBankCount = eram.size() / 0x2000;

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t data) override;

private:
};