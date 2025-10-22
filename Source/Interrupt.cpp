//
// Created by ADMIN on 10/11/2025.
//

#include "../Header/Interrupt.h"

#include "../Header/Bus.h"

Interrupt::Interrupt(Bus *bus): bus(bus){}

Interrupt::~Interrupt()
= default;

void Interrupt::requestInterrupt(uint8_t bit)
{
    bus->write(IFLocation, bus->read(IFLocation) | (1 << bit));
    bus->cpu.checkInterrupts();
}