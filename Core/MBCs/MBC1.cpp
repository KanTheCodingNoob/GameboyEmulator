//
// Created by ADMIN on 10/5/2025.
//

#include "MBC1.h"

MBC1::MBC1()
= default;

MBC1::~MBC1()
= default;


bool MBC1::MBCWrite(uint16_t addr, uint8_t data)
{
    if (addr <= 0x1FFF && data == 0xA)
    {
        ramEnabled = true;
        return true;
    }
    if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        if (data == 0) // If zero then default to one
        {
            romBankNum = 1;
            return true;
        }
        romBankNum = data & 0x0F; // If num is over 15, higher bit are discarded
        return true;
    }
    if (addr >= 0x4000 && addr <= 0x5FFF)
    {
        return true;
    }
    if (addr >= 0x6000 && addr <= 0x7FFF)
    {
        return true;
    }
    return false;
}

