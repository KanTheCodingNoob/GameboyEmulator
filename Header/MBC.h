//
// Created by ADMIN on 10/5/2025.
//

#pragma once
#include <cstdint>


class MBC {
public:
    virtual ~MBC() = default;

    bool mode = true; // true = default mode, false = alternate mode
    bool ramEnabled = false;
    uint8_t romBankNum = 1;
    uint8_t ramBankNum = 0;

    virtual bool MBCWrite(uint16_t addr, uint8_t data) = 0;
};
