//
// Created by ADMIN on 10/11/2025.
//

#pragma once

#include "SDL3/SDL_scancode.h"

class Bus;

class Joypad {
public:
    explicit Joypad(Bus* bus);
    ~Joypad();
    void connectBus(Bus *n)
    {
        bus = n;
    }

    enum class KeyValue
    {
        right = 0,
        left = 1,
        up = 2,
        down = 3,
        a = 4,
        b = 5,
        start = 6,
        select = 7,
    };


    // static int scancodeToKey(const SDL_Scancode sc)
    // {
    //     switch (sc)
    //     {
    //     case SDL_SCANCODE_RIGHT:  return right;
    //     case SDL_SCANCODE_LEFT:   return left;
    //     case SDL_SCANCODE_UP:     return up;
    //     case SDL_SCANCODE_DOWN:   return down;
    //     case SDL_SCANCODE_Z:      return a;
    //     case SDL_SCANCODE_X:      return b;
    //     case SDL_SCANCODE_RETURN: return start;
    //     case SDL_SCANCODE_RSHIFT: return select;
    //     default: return -1;
    //     }
    // }

    static bool isDirectionKey(const int key)
    {
        return key >= static_cast<int>(KeyValue::right) && key <= static_cast<int>(KeyValue::down);
    }

    static bool isButtonKey(const int key)
    {
        return key >= static_cast<int>(KeyValue::a) && key <= static_cast<int>(KeyValue::start);
    }

    void handleKey(KeyValue key_value, bool pressed);

    void handleKeyPressed(KeyValue key_value);

    void handleKeyReleased(KeyValue key_value);

    [[nodiscard]] uint8_t getJoypadState(const uint8_t& JOYPAD) const;

private:
    Bus* bus{};

    const uint16_t joypadRegisterLocation = 0xFF00;

    // For some reason, unpressed is represented as 1 and pressed as 0.
    uint8_t input = 0xFF;
};



