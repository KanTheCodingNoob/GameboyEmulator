//
// Created by ADMIN on 2/7/2026.
//

#pragma once
#include "MBC.h"

class MBC3: public MBC
{
public:
    MBC3(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram);
    ~MBC3() override;

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t data) override;
private:
    uint8_t romBankNumber = 0;
    uint8_t romBankCount = rom.size() / 0x4000;
    uint8_t ramBankNumber = 0;
    uint8_t ramBankCount = eram.size() / 0x2000;

    bool ramAndRTCEnabled = false;
};