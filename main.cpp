#include <iomanip>
#include <thread>
#include <atomic>

#include "Core/Bus.h"
#include "Core/sm83.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include "App/Display.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#define WIDTH 160
#define HEIGHT 144

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;
static SDL_Texture *texture = nullptr;

static std::thread worker;

static std::atomic<bool> running{true};
Bus bus;
uint32_t (&framebuffer)[HEIGHT][WIDTH] = bus.ppu.LCD;

void backgroundTask()
{
    std::shared_ptr<Cartridge> cartridge(new Cartridge("TestRoms/tetris.gb"));
    bus.insertCartridge(cartridge);

    using clock = std::chrono::steady_clock;

    constexpr auto FRAME_DURATION =
        std::chrono::duration_cast<clock::duration>(
            std::chrono::duration<double>(1.0 / 59.7275)
    );

    auto nextFrame = clock::now();

    while (running) // Roughly emulate the master clock
    {
        constexpr int M_CYCLES_PER_FRAME = 17556;
        for (int i = 0; i < M_CYCLES_PER_FRAME; i++)
        {
            bus.clock();
        }

        nextFrame += FRAME_DURATION;
        std::this_thread::sleep_until(nextFrame);
    }
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("GB Emulator", "1.0", "com.example.gb");

    auto* app = new Display();
    if (!app->init()) {
        delete app;
        return SDL_APP_FAILURE;
    }

    *appstate = app;
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    auto* app = static_cast<Display*>(appstate);

    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;

    app->handleEvent(*event);
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto* app = static_cast<Display*>(appstate);
    app->iterate();
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    auto* app = static_cast<Display*>(appstate);
    delete app;
}
