//
// Created by ADMIN on 10/11/2025.
//


#pragma once
#include <cstdint>

class Bus;

class Interrupt {
public:

    enum interruptBit
    {
        VBlankBit = 0,
        LCDBit = 1,
        TimerBit = 2,
        SerialBit = 3,
        JoypadBit = 4
    };

    explicit Interrupt(Bus* bus);
    ~Interrupt();

    void connectBus(Bus* bus)
    {
        this->bus = bus;
    }

    void requestInterrupt(uint8_t bit);
    void requestJoypadInterrupt();
    void requestSTATInterrupt();
    void requestVBlankInterrupt();

private:
    Bus* bus{};

    const uint16_t IELocation = 0xFFFF;
    const uint16_t IFLocation = 0xFF0F;
};



