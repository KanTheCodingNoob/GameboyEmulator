//
// Created by ADMIN on 1/24/2026.
//


#pragma once

#include "Emulator.h"
#include "SDLInputMapper.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"

#include "../Core/Bus.h"

class SDLDisplay
{
public:
    static constexpr int WIDTH = 160;
    static constexpr int HEIGHT = 144;

    SDLDisplay();
    ~SDLDisplay();

    bool init();
    void renderIdleScreen();
    void handleEvent(const SDL_Event& event);
    void iterate();
    void shutdown();

private:
    Emulator emulator;
    SDLInputMapper inputMapper;

    void openRomDialog();
    void loadCartridge(const std::string& path);

    static void onRomSelected(
        void* userdata,
        const char* const* filelist,
        int filter
        );

    SDL_DialogFileFilter filters[1] = {
        { "Game Boy ROMs", "gb" },
    };

    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture*  texture  = nullptr;
};

