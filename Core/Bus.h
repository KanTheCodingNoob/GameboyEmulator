//
// Created by anngu on 7/27/2025.
//

#pragma once
#define BUS_H
#include <array>
#include <cstdint>

#include "sm83.h"


class Bus {
public:
    Bus();
    ~Bus();

    sm83 cpu;

    std::array<uint8_t, 64 * 1024> ram{};

    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);
};



