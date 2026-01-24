//
// Created by ADMIN on 1/25/2026.
//

#include "InputMapper.h"

InputMapper::InputMapper(const std::unordered_map<SDL_Scancode, Action>& bindings)
{
    this->bindings = bindings;
}

void InputMapper::bind(const SDL_Scancode key, Action action)
{
    bindings[key] = std::move(action);
}

void InputMapper::handleKeyDown(const SDL_Scancode scancode) {
    if (const auto it = bindings.find(scancode); it != bindings.end()) {
        it->second(true);
    }
}

void InputMapper::handleKeyUp(const SDL_Scancode scancode) {
    if (const auto it = bindings.find(scancode); it != bindings.end()) {
        it->second(false);
    }
}