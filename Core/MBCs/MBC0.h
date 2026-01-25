//
// Created by ADMIN on 10/5/2025.
//

#pragma once
#include "../MBC.h"


class MBC0: public MBC {
public:
    MBC0();
    ~MBC0() override;

    bool MBCWrite(uint16_t addr, uint8_t data) override;
};