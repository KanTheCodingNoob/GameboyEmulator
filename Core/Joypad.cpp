//
// Created by ADMIN on 10/11/2025.
//

#include "Joypad.h"

#include "Bus.h"

Joypad::Joypad(Bus* bus): bus(bus){}

Joypad::~Joypad()
= default;

void Joypad::handleKey(const KeyValue key_value, const bool pressed)
{
    pressed ? handleKeyPressed(key_value)
            : handleKeyReleased(key_value);
}

void Joypad::handleKeyPressed(KeyValue key_value)
{
    int key = static_cast<int>(key_value);

    // Already pressed? → no edge → no interrupt
    if (!(input & (1 << key)))
        return;

    // Mark as pressed
    input &= ~(1 << key);

    const uint8_t joypad = bus->io.directIORead(joypadRegisterLocation); // last value written to FF00

    bool directionSelected = !(joypad & (1 << 4));
    bool buttonSelected   = !(joypad & (1 << 5));

    bool shouldInterrupt =
        (isDirectionKey(key) && directionSelected) ||
        (isButtonKey(key)    && buttonSelected);

    if (shouldInterrupt)
    {
        bus->interrupt.requestJoypadInterrupt();
    }
}

void Joypad::handleKeyReleased(KeyValue key_value)
{
    if (const int key = static_cast<int>(key_value); key >= 0)
        input |= (1 << key);
}

uint8_t Joypad::getJoypadState(const uint8_t& JOYPAD) const
{
    uint8_t result = JOYPAD | 0xCF; // 1100 1111 (lower bits default high)

    // Direction keys selected
    if (!(JOYPAD & (1 << 4)))
    {
        if (!(input & (1 << static_cast<int>(KeyValue::right)))) result &= ~(1 << 0);
        if (!(input & (1 << static_cast<int>(KeyValue::left))))  result &= ~(1 << 1);
        if (!(input & (1 << static_cast<int>(KeyValue::up))))    result &= ~(1 << 2);
        if (!(input & (1 << static_cast<int>(KeyValue::down))))  result &= ~(1 << 3);
    }

    // Button keys selected
    if (!(JOYPAD & (1 << 5)))
    {
        if (!(input & (1 << static_cast<int>(KeyValue::a))))      result &= ~(1 << 0);
        if (!(input & (1 << static_cast<int>(KeyValue::b))))      result &= ~(1 << 1);
        if (!(input & (1 << static_cast<int>(KeyValue::select)))) result &= ~(1 << 2);
        if (!(input & (1 << static_cast<int>(KeyValue::start))))  result &= ~(1 << 3);
    }
    return result;
}


