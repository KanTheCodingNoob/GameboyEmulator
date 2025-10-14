//
// Created by ADMIN on 10/11/2025.
//

#pragma once
#include <cstdint>

class Bus;

class Timer {
public:
    Timer();
    ~Timer();
    void tick();

    void connectBus(Bus* n)
    {
        bus = n;
    };

private:
    Bus* bus{};

    const uint16_t DIVLocation = 0x04;
    const uint16_t TIMALocation = 0x05;
    const uint16_t TMALocation = 0x06;
    const uint16_t TACLocation = 0x07;
};