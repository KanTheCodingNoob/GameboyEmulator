//
// Created by ADMIN on 1/24/2026.
//


#pragma once
#include <atomic>
#include <thread>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"


#include "../Core/Bus.h"

class Display
{
public:
    static constexpr int WIDTH = 160;
    static constexpr int HEIGHT = 144;

    Display();
    ~Display();

    bool init();
    void handleEvent(const SDL_Event& event);
    void iterate();
    void shutdown();

private:
    void backgroundTask();

    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture*  texture  = nullptr;

    std::thread worker;
    std::atomic<bool> running{true};

    Bus bus;
    uint32_t (&framebuffer)[HEIGHT][WIDTH] = bus.ppu.LCD;
};

