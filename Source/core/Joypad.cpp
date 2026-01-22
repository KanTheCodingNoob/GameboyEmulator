//
// Created by ADMIN on 10/11/2025.
//

#include "../../Header/core/Joypad.h"

#include "../../Header/core/Bus.h"

Joypad::Joypad(Bus* bus): bus(bus){}

Joypad::~Joypad()
= default;

void Joypad::handleKeyPressed(const SDL_Scancode scancode)
{
    const int key = scancodeToKey(scancode);
    if (key < 0) return;

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

void Joypad::handleKeyReleased(const SDL_Scancode scancode)
{
    if (const int key = scancodeToKey(scancode); key >= 0)
        input |= (1 << key);
}

uint8_t Joypad::getJoypadState(const uint8_t& JOYPAD) const
{
    uint8_t result = JOYPAD | 0xCF; // 1100 1111 (lower bits default high)

    // Direction keys selected
    if (!(JOYPAD & (1 << 4)))
    {
        if (!(input & (1 << right))) result &= ~(1 << 0);
        if (!(input & (1 << left)))  result &= ~(1 << 1);
        if (!(input & (1 << up)))    result &= ~(1 << 2);
        if (!(input & (1 << down)))  result &= ~(1 << 3);
    }

    // Button keys selected
    if (!(JOYPAD & (1 << 5)))
    {
        if (!(input & (1 << a)))      result &= ~(1 << 0);
        if (!(input & (1 << b)))      result &= ~(1 << 1);
        if (!(input & (1 << select))) result &= ~(1 << 2);
        if (!(input & (1 << start)))  result &= ~(1 << 3);
    }
    return result;
}


