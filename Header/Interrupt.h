//
// Created by ADMIN on 10/11/2025.
//


#pragma once
#include <cstdint>

class Bus;

class Interrupt {
public:
    Interrupt();
    ~Interrupt();

    void connectBus(Bus* bus)
    {
        this->bus = bus;
    }

    void requestInterrupt(uint8_t bit);

private:
    Bus* bus{};

    const uint16_t IELocation = 0xFFFF;
    const uint16_t IFLocation = 0xFF0F;
};



