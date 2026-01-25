//
// Created by ADMIN on 1/24/2026.
//

#include "SDLDisplay.h"
// Window.cpp
#include <chrono>



SDLDisplay::SDLDisplay()
: inputMapper({
    {SDL_SCANCODE_RIGHT,  [this](const bool p) {emulator.bus.joypad.handleKey(Joypad::KeyValue::right, p); }},
    {SDL_SCANCODE_LEFT,  [this](const bool p) {emulator.bus.joypad.handleKey(Joypad::KeyValue::left, p); }},
    {SDL_SCANCODE_UP,  [this](const bool p) {emulator.bus.joypad.handleKey(Joypad::KeyValue::up, p); }},
    {SDL_SCANCODE_DOWN,  [this](const bool p) {emulator.bus.joypad.handleKey(Joypad::KeyValue::down, p); }},

    {SDL_SCANCODE_Z,  [this](const bool p) {emulator.bus.joypad.handleKey(Joypad::KeyValue::a, p); }},
    {SDL_SCANCODE_X,  [this](const bool p) {emulator.bus.joypad.handleKey(Joypad::KeyValue::b, p); }},
    {SDL_SCANCODE_RETURN,  [this](const bool p) {emulator.bus.joypad.handleKey(Joypad::KeyValue::start, p); }},
    {SDL_SCANCODE_RSHIFT,  [this](const bool p) {emulator.bus.joypad.handleKey(Joypad::KeyValue::select, p); }},
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

void SDLDisplay::openRomDialog()
{
    SDL_ShowOpenFileDialog(
        &SDLDisplay::onRomSelected,
        this,               // userdata
        window,             // parent window
        filters,            // Only gb file
        SDL_arraysize(filters), // filter count
        nullptr,            // default location
        false               // allow multiple
    );
}


void SDLDisplay::onRomSelected(
    void* userdata,
    const char* const* filelist,
    int filter
)
{
    if (!filelist || !filelist[0])
        return; // user cancelled

    auto* display =
        static_cast<SDLDisplay*>(userdata);

    display->loadCartridge(filelist[0]);
}

void SDLDisplay::loadCartridge(const std::string& path)
{
    emulator.loadCartridge(path);
    emulator.start();
}

void SDLDisplay::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if ((event.key.mod & SDL_KMOD_CTRL) && event.key.scancode == SDL_SCANCODE_O && !emulator.hasRom())
        {
            openRomDialog();
        }
        inputMapper.handleKeyDown(event.key.scancode);
    }
    else if (event.type == SDL_EVENT_KEY_UP) {
        inputMapper.handleKeyUp(event.key.scancode);
    }
}

void SDLDisplay::iterate()
{
    if (!emulator.hasRom())
    {
        renderIdleScreen();
        return;
    }

    SDL_UpdateTexture(texture, nullptr, emulator.framebuffer, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void SDLDisplay::shutdown()
{
    emulator.stop();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
