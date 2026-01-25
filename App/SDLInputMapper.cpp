//
// Created by ADMIN on 1/25/2026.
//

#include "SDLInputMapper.h"

SDLInputMapper::SDLInputMapper(const std::unordered_map<SDL_Scancode, Action>& bindings)
{
    this->bindings = bindings;
}

void SDLInputMapper::bind(const SDL_Scancode key, Action action)
{
    bindings[key] = std::move(action);
}

void SDLInputMapper::handleKeyDown(const SDL_Scancode scancode) {
    if (const auto it = bindings.find(scancode); it != bindings.end()) {
        it->second(true);
    }
}

void SDLInputMapper::handleKeyUp(const SDL_Scancode scancode) {
    if (const auto it = bindings.find(scancode); it != bindings.end()) {
        it->second(false);
    }
}