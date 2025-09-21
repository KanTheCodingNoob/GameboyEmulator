//
// Created by anngu on 7/27/2025.
//

#pragma once
#define BUS_H
#include <array>
#include <cstdint>
#include <string>
#include <vector>

class sm83;

class Bus {
public:
    Bus();
    ~Bus();

    sm83* cpu = nullptr;   // pointer instead of value

    std::array<uint8_t, 64 * 1024> ram{};

    std::vector<uint8_t> rom;                   // cartridge ROM
    void loadROM(const std::string& filename);  // load a .gb file
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);
};



