//
// Created by anngu on 7/27/2025.
//

#include "Bus.h"
#include "sm83.h"
#include <fstream>
#include <iostream>

Bus::Bus() {
    for (auto &i : ram) i = 0x00;

    cpu = new sm83();
    cpu->connectBus(this);
}

Bus::~Bus() {
    delete cpu;
}

void Bus::write(const uint16_t addr, const uint8_t data) {
    if (addr >= 0x000 && addr <= 0xFFFF) {
        ram[addr] = data;
    }
}

uint8_t Bus::read(const uint16_t addr) {
    if (addr >= 0x000 && addr <= 0xFFFF)
        return ram[addr];
    return 0;
}

void Bus::loadROM(const std::string& filename) {
    std::ifstream romFile(filename, std::ios::binary | std::ios::ate);
    if (!romFile.is_open()) {
        throw std::runtime_error("Failed to open ROM: " + filename);
    }

    std::streamsize size = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    rom.resize(size);
    if (!romFile.read(reinterpret_cast<char*>(rom.data()), size)) {
        throw std::runtime_error("Failed to read ROM data");
    }

    std::cout << "Loaded ROM: " << filename << " (" << size << " bytes)\n";
}



