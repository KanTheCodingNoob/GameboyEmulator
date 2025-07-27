//
// Created by anngu on 7/24/2025.
//
#include "sm83.h"


sm83::sm83(): F(AF.low), A(AF.high), B(BC.high), C(BC.low), D(DE.high), E(DE.low), H(HL.high), L(HL.low){

};

sm83::~sm83() = default;

uint8_t sm83::read(uint16_t addr) {
    return bus->read(addr);
}

void sm83::write(uint16_t addr, uint8_t data) {
    bus->write(addr, data);
}


