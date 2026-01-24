//
// Created by ADMIN on 1/24/2026.
//

#include "Display.h"
// Window.cpp
#include <chrono>



Display::Display() = default;
Display::~Display() {
    shutdown();
}

bool Display::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(
            "Game Boy Emulator",
            WIDTH, HEIGHT,
            SDL_WINDOW_RESIZABLE,
            &window, &renderer)) {
        SDL_Log("Window/Renderer creation failed: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderLogicalPresentation(
        renderer, WIDTH, HEIGHT,
        SDL_LOGICAL_PRESENTATION_LETTERBOX
    );

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    worker = std::thread(&Display::backgroundTask, this);
    return true;
}

void Display::backgroundTask()
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

void Display::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        bus.joypad.handleKeyPressed(event.key.scancode);
    }
    else if (event.type == SDL_EVENT_KEY_UP) {
        bus.joypad.handleKeyReleased(event.key.scancode);
    }
}

void Display::iterate()
{
    SDL_UpdateTexture(texture, nullptr, framebuffer, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Display::shutdown()
{
    running = false;
    if (worker.joinable())
        worker.join();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
