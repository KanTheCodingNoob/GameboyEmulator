//
// Created by ADMIN on 10/19/2025.
//

#pragma once
#include <array>
#include <cstdint>

class Bus;

class IO {
public:
    explicit IO(Bus* bus);
    ~IO();

    void busIOWrite(uint16_t addr, uint8_t data) const;
    uint8_t busIORead(uint16_t addr) const;

    void connectBus(Bus* n)
    {
        bus = n;
    }
private:
    Bus* bus = {};
    std::array<uint8_t, 128>& IORegisters;
    // Only accessible through selected (emulated) hardware
    void directIOWrite(uint16_t addr, uint8_t data);
    uint8_t directIORead(uint16_t addr);
    friend class Timer;
    friend class Interrupt;
    friend class Joypad;
};
