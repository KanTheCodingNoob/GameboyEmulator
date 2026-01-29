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

private:
    std::vector<uint8_t> eram; // Occupy A000 - BFFF in bus
    std::vector<uint8_t> rom;
    std::unique_ptr<MBC> mapper; // Responsible for switching memory bank and ram
};