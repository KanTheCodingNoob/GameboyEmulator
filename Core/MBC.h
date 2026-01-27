//
// Created by ADMIN on 10/5/2025.
//

#pragma once
#include <cstdint>


class MBC {
public:
    virtual ~MBC() = default;

    bool ramEnabled = false; // Whether external ram is used
    uint8_t romBankNum = 1; // ROM bank number
    uint8_t ramBankNum = 0; // RAM bank number

    virtual bool MBCWrite(uint16_t addr, uint8_t data) = 0;
};
