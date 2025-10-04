//
// Created by anngu on 7/27/2025.
//

#include "../Header/Bus.h"
#include "../Header/sm83.h"

Bus::Bus() {
    for (auto &i : ram) i = 0x00;

    cpu.connectBus(this);
}

Bus::~Bus() = default;

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

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    this->cartridge = cartridge;
}


