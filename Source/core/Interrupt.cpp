//
// Created by ADMIN on 10/11/2025.
//

#include "../../Header/core/Interrupt.h"

#include "../../Header/core/Bus.h"

Interrupt::Interrupt(Bus *bus): bus(bus){}

Interrupt::~Interrupt()
= default;

void Interrupt::requestInterrupt(uint8_t bit)
{
    bus->write(IFLocation, bus->read(IFLocation) | (1 << bit));
}

void Interrupt::requestJoypadInterrupt()
{
    requestInterrupt(JoypadBit);
}

void Interrupt::requestSTATInterrupt()
{
    requestInterrupt(LCDBit);
}

void Interrupt::requestVBlankInterrupt()
{
    requestInterrupt(VBlankBit);
}