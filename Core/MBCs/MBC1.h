//
// Created by ADMIN on 10/5/2025.
//

#pragma once
#include "../MBC.h"


class MBC1: public MBC {
public:
    MBC1();
    ~MBC1() override;
    bool mode = true; // true = default mode, false = alternate mode

    bool MBCWrite(uint16_t addr, uint8_t data) override;
};