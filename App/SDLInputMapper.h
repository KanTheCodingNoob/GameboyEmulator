// InputMapper.h
#pragma once
#include <functional>
#include <SDL3/SDL.h>
#include <unordered_map>

class SDLInputMapper {
public:
    using Action = std::function<void(bool pressed)>;

    explicit SDLInputMapper(const std::unordered_map<SDL_Scancode, Action>& bindings);
    ~SDLInputMapper() = default;

    void bind(SDL_Scancode key, Action action);
    void handleKeyDown(SDL_Scancode scancode);
    void handleKeyUp(SDL_Scancode scancode);
private:
    std::unordered_map<SDL_Scancode, Action> bindings;
};