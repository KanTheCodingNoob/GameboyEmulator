//
// Created by ADMIN on 10/4/2025.
//

#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <cstdint>


class Cartridge
{
public:
    Cartridge();
    ~Cartridge();

    bool read(uint16_t addr, uint8_t &data);
    bool write(uint16_t addr, uint8_t data);
};



#endif //CARTRIDGE_H
