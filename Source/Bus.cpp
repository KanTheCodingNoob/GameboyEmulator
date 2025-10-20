//
// Created by anngu on 7/27/2025.
//

#include "../Header/Bus.h"
#include "../Header/sm83.h"

Bus::Bus() {
    for (auto &i : wram) i = 0x00;

    cpu.connectBus(this);
    joypad.connectBus(this);
    timer.connectBus(this);
    interrupt.connectBus(this);
    io.connectBus(this);
}

Bus::~Bus() = default;

void Bus::write(const uint16_t addr, const uint8_t data) {
    if (addr <= 0x7FFF) // Write to Bank 00 (Not actually writable to rom, the actual purpose is to adjust the mapper)
    {
        cartridge->write(addr, data);
        return;
    }
    if (addr >= 0x8000 && addr <= 0x9FFF) // Write Vram
    {
        vram[addr - 0x8000] = data;
        return;
    }
    if (addr >= 0xA000 && addr <= 0xBFFF) // Write external ram from cartridge
    {
        cartridge->eram[addr - 0xA000] = data;
        return;
    }
    if (addr >= 0xC000 && addr <= 0xDFFF) // Write work ram
    {
        wram[addr - 0xC000] = data;
        return;
    }
    if (addr >= 0xE000 && addr <= 0xFDFF) // Write work ram (echo ram mirror wram)
    {
        wram[addr - 0xE000] = data;
        return;
    }
    if (addr >= 0xFE00 && addr <= 0xFE9F) // Write OAM
    {
        OAM[addr - 0xFE00] = data;
        return;
    }

    // --- SERIAL OUTPUT HANDLING FOR BLARGG TESTS ---
    if (addr == 0xFF01) { // SB - Serial transfer data
        IORegisters[addr - 0xFF00] = data;
        serialData = data; // store last written byte
        return;
    }
    // Serial Control (SC)
    if (addr == 0xFF02) {
        IORegisters[addr - 0xFF00] = data;

        if (data == 0x81) { // Internal clock + transfer start
            std::cout << static_cast<char>(serialData) << std::flush;
        }
        return;
    }
    // ------------------------------------------------

    if (addr >= 0xFF04 && addr <= 0xFF07)
    {
        timer.write(addr, data);
        return;
    }

    if (addr >= 0xFF00 && addr <= 0xFF7F) // Write I/O Registers
    {
        IORegisters[addr - 0xFF00] = data;
        return;
    }
    if (addr >= 0xFF80 && addr <= 0xFFFE) // Write HRAM
    {
        HRAM[addr - 0xFF80] = data;
        return;
    }
    if (addr == 0xFFFF) // Write IE
    {
        IE = data;
        return;
    }
}

uint8_t Bus::read(const uint16_t addr) {
    if (addr <= 0x3FFF) // Read Bank 00
    {
        return cartridge->readBank00(addr);
    }
    if (addr >= 0x4000 && addr <= 0x7FFF) // Read bank NN (NN is based on the mapper on the cartridge)
    {
        return cartridge->readBank01_nn(addr - 0x4000); // Modify the address to properly read bank 01_NN array
    }
    if (addr >= 0x8000 && addr <= 0x9FFF) // Read Vram
    {
        return vram[addr - 0x8000];
    }
    if (addr >= 0xA000 && addr <= 0xBFFF) // Read external ram from cartridge
    {
        return cartridge->eram[addr - 0xA000];
    }
    if (addr >= 0xC000 && addr <= 0xDFFF) // Read work ram
    {
        return wram[addr - 0xC000];
    }
    if (addr >= 0xE000 && addr <= 0xFDFF) // Read work ram (echo ram mirror wram)
    {
        return wram[addr - 0xE000];
    }
    if (addr >= 0xFE00 && addr <= 0xFE9F) // Read OAM
    {
        return OAM[addr - 0xFE00];
    }
    if (addr >= 0xFF00 && addr <= 0xFF7F) // Read I/O Registers
    {
        return IORegisters[addr - 0xFF00];
    }
    if (addr >= 0xFF80 && addr <= 0xFFFE) // Read HRAM
    {
        return HRAM[addr - 0xFF80];
    }
    if (addr == 0xFFFF) // Read IE
    {
        return IE;
    }

    return 0;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    this->cartridge = cartridge;
}

void Bus::clock()
{
    cpu.clock(); // Execute one M-cycle worth of instruction
    systemClockCounter++; // Keep track of amount of clock (which is reset after reaching 64 by the function below)
    timer.tick();
}


