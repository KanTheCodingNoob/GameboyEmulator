//
// Created by anngu on 7/27/2025.
//

#pragma once
#include <array>
#include <memory>

#include "APU.h"
#include "Cartridge.h"
#include "Interrupt.h"
#include "IO.h"
#include "Joypad.h"
#include "PPU.h"
#include "sm83.h"
#include "Timer.h"

class Bus {
public:
    Bus();
    ~Bus();

    sm83 cpu;
    Joypad joypad;
    Timer timer;
    Interrupt interrupt;
    IO io;
    PPU ppu;
    APU apu;

    std::shared_ptr<Cartridge> cartridge;

    std::array<uint8_t, 0x2000> wram{};
    std::array<uint8_t, 0x2000> vram{};
    std::array<uint8_t, 160> OAM{};
    std::array<uint8_t, 128> IORegisters{};
    std::array<uint8_t, 127> HRAM{};
    uint8_t IE = 0;
    // uint8_t serialData = 0;
    uint64_t systemClockCounter = 0;

    void OAM_DMA_Transfer(uint8_t XX);

    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);

    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();
    void reset();
};



