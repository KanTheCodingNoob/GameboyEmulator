//
// Created by ADMIN on 10/4/2025.
//

#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include <iostream>

#include "MBCs/MBC0.h"
#include "MBCs/MBC1.h"


class Cartridge
{
public:
    explicit Cartridge(const std::string& filename);
    ~Cartridge();

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

    enum class Type : uint8_t
    {
        ROM_ONLY = 0x00,
        MBC1 = 0x01,
        MBC1_RAM = 0x02,
        MBC1_RAM_BATTERY = 0x03,
        MBC2 = 0x05,
        MBC2_BATTERY = 0x06,
        MBC3_TIMER_BATTERY = 0x0F,
        MBC3_TIMER_RAM_BATTERY = 0x10,
        MBC3 = 0x11,
        MBC3_RAM = 0x12,
        MBC3_RAM_BATTERY = 0x13,
        MBC5 = 0x19,
        MBC5_RAM = 0x1A,
        MBC5_RAM_BATTERY = 0x1B,
    };

    static constexpr uint16_t CARTRIDGE_TYPE_ADDR = 0x0147;
    static constexpr uint16_t RAM_SIZE_ADDR = 0x0149;

private:
    std::vector<uint8_t> eram; // Occupy A000 - BFFF in bus
    std::vector<uint8_t> rom;
    std::unique_ptr<MBC> mapper; // Responsible for switching memory bank and ram
};