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
    uint8_t readBank00(uint16_t addr);
    uint8_t readBank01_nn(uint16_t addr);

    void write(uint16_t addr, uint8_t data);

    std::vector<uint8_t> eram; // Occupy A000 - BFFF in bus
    std::vector<uint8_t> rom;
    uint8_t* bank00; // pointer to the first 16KiB of Rom, occupy 0000 - 3FFF from bus
    uint8_t* bank01_NN ; // switchable pointer to different memory bank, occupy 4000 - 7FFF from bus
private:
    std::unique_ptr<MBC> mapper; // Responsible for switching memory bank and ram
};