//
// Created by ADMIN on 1/24/2026.
//

#include "SDLDisplay.h"
// Window.cpp
#include <chrono>



SDLDisplay::SDLDisplay()
: inputMapper({
    {SDL_SCANCODE_RIGHT,  [this](const bool p) {bus.joypad.handleKey(Joypad::KeyValue::right, p); }},
    {SDL_SCANCODE_LEFT,  [this](const bool p) {bus.joypad.handleKey(Joypad::KeyValue::left, p); }},
    {SDL_SCANCODE_UP,  [this](const bool p) {bus.joypad.handleKey(Joypad::KeyValue::up, p); }},
    {SDL_SCANCODE_DOWN,  [this](const bool p) {bus.joypad.handleKey(Joypad::KeyValue::down, p); }},

    {SDL_SCANCODE_Z,  [this](const bool p) {bus.joypad.handleKey(Joypad::KeyValue::a, p); }},
    {SDL_SCANCODE_X,  [this](const bool p) {bus.joypad.handleKey(Joypad::KeyValue::b, p); }},
    {SDL_SCANCODE_RETURN,  [this](const bool p) {bus.joypad.handleKey(Joypad::KeyValue::start, p); }},
    {SDL_SCANCODE_RSHIFT,  [this](const bool p) {bus.joypad.handleKey(Joypad::KeyValue::select, p); }},
})
{}
SDLDisplay::~SDLDisplay() {
    shutdown();
}

bool SDLDisplay::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(
            "Game Boy Emulator",
            WIDTH * 2, HEIGHT * 2,
            SDL_WINDOW_RESIZABLE,
            &window, &renderer)) {
        SDL_Log("Window/Renderer creation failed: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderLogicalPresentation(
        renderer, WIDTH * 2, HEIGHT * 2,
        SDL_LOGICAL_PRESENTATION_LETTERBOX
    );

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    worker = std::thread(&SDLDisplay::backgroundTask, this);
    return true;
}

void SDLDisplay::renderIdleScreen()
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // dark gray
    SDL_RenderClear(renderer);

    const auto msg = "Ctrl+O to load a ROM";

    // Center-ish placement (logical coords)
    constexpr int x = 20;
    constexpr int y = 20;

    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderDebugText(renderer, x, y, msg);

    SDL_RenderPresent(renderer);
}

// void SDLDisplay::loadCartridge(const std::string& path)
// {
//     emulator.loadCartridge(path);
//     emulator.start();
// }

void SDLDisplay::backgroundTask()
{
    const auto cartridge = std::make_shared<Cartridge>("TestRoms/tetris.gb");
    bus.insertCartridge(cartridge);

    using clock = std::chrono::steady_clock;
    constexpr auto FRAME_DURATION =
        std::chrono::duration_cast<clock::duration>(
            std::chrono::duration<double>(1.0 / 59.7275)
        );

    auto nextFrame = clock::now();

    while (running) {
        constexpr int M_CYCLES_PER_FRAME = 17556;
        for (int i = 0; i < M_CYCLES_PER_FRAME; i++) {
            bus.clock();
        }

        nextFrame += FRAME_DURATION;
        std::this_thread::sleep_until(nextFrame);
    }
}

void SDLDisplay::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        inputMapper.handleKeyDown(event.key.scancode);
    }
    else if (event.type == SDL_EVENT_KEY_UP) {
        inputMapper.handleKeyUp(event.key.scancode);
    }
}

void SDLDisplay::iterate()
{
    // if (!emulator.hasRom())
    // {
    //     renderIdleScreen();
    //     return;
    // }

    SDL_UpdateTexture(texture, nullptr, framebuffer, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void SDLDisplay::shutdown()
{
    running = false;
    if (worker.joinable())
        worker.join();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
