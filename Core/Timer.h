//
// Created by ADMIN on 10/11/2025.
//

#pragma once
#include <cstdint>

class Bus;

class Timer {
public:
    explicit Timer(Bus* bus);
    ~Timer();
    void tick();
    void write(uint16_t addr, uint8_t data);
    void reset();

    void connectBus(Bus* n)
    {
        bus = n;
    };

private:
    Bus* bus{};

    const uint16_t DIVLocation = 0xFF04;
    const uint16_t TIMALocation = 0xFF05;
    const uint16_t TMALocation = 0xFF06;
    const uint16_t TACLocation = 0xFF07;
    const uint16_t IFLocation = 0xFF0F;

    uint16_t DIVCounter = 0;
    uint16_t TIMACounter = 0;
    uint16_t DIVIncrementCycle = 64;
    uint16_t TIMAIncrementCycle = 256;
};