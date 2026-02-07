//
// Created by ADMIN on 10/5/2025.
//

#pragma once
#include <cstdint>
#include <vector>


class MBC {
public:
    virtual ~MBC() = default;
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t data) = 0;
protected:

    std::vector<uint8_t>& rom;
    std::vector<uint8_t>& eram;
    MBC(std::vector<uint8_t>& rom, std::vector<uint8_t>& eram):
        rom(rom), eram(eram) {}
};
