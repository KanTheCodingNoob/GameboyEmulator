//
// Created by anngu on 7/24/2025.
//
#include "sm83.h"


sm83::sm83(): F(AF_PAIR.low), A(AF_PAIR.high), B(BC_PAIR.high), C(BC_PAIR.low),
    D(DE_PAIR.high), E(DE_PAIR.low), H(HL_PAIR.high), L(HL_PAIR.low),
    AF(AF_PAIR.value), BC(BC_PAIR.value), DE(DE_PAIR.value), HL(HL_PAIR.value)
{

};

sm83::~sm83() = default;

uint8_t sm83::read(uint16_t addr) {
    return bus->read(addr);
}

void sm83::write(uint16_t addr, uint8_t data) {
    bus->write(addr, data);
}


