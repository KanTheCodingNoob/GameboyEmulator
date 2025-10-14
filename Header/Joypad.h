//
// Created by ADMIN on 10/11/2025.
//

#pragma once

#include <memory>

class Bus;

class Joypad {
public:
    Joypad();
    ~Joypad();

    void connectBus(Bus *n)
    {
        bus = n;
    }
private:
    Bus* bus{};

    const uint16_t joypadRegisterLocation = 0xFF00;
};



