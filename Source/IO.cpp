//
// Created by ADMIN on 10/19/2025.
//

#include "../Header/IO.h"

#include "../Header/Bus.h"

IO::IO(Bus *bus) : bus(bus), IORegisters(bus->IORegisters)
{
}

IO::~IO()
= default;

void IO::busIOWrite(uint16_t addr, uint8_t data)
{
    if (addr == 0xFF00)
    {
        return;
    }
    if (addr >= 0xFF04 && addr <= 0xFF07)
    {
        bus->timer.write(addr, data);
        return;
    }

    // OAM DMA Transfer activated through writing to FF46
    if (addr == 0xFF46)
    {
        IORegisters[addr - 0xFF00] = data;
        bus->OAM_DMA_Transfer(data);
        return;
    }

    if (addr >= 0xFF00 && addr <= 0xFF7F) // Write I/O Registers
    {
        IORegisters[addr - 0xFF00] = data;
    }
}

uint8_t IO::busIORead(uint16_t addr)
{
    return IORegisters[addr - 0xFF00];
}

void IO::directIOWrite(uint16_t addr, uint8_t data)
{
    IORegisters[addr - 0xFF00] = data;
}

uint8_t IO::directIORead(uint16_t addr)
{
    return IORegisters[addr - 0xFF00];
}
