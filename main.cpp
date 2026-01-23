#include <iomanip>
#include <thread>
#include <atomic>

#include "Core/Bus.h"
#include "Core/sm83.h"

#define SDL_MAIN_USE_CALLBACKS 1
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
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", WIDTH * 2, HEIGHT * 2, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WIDTH * 2, HEIGHT * 2, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
        );
    // start background thread
    worker = std::thread(backgroundTask);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        bus.joypad.handleKeyPressed(event->key.scancode);
    } else if (event->type == SDL_EVENT_KEY_UP)
    {
        bus.joypad.handleKeyReleased(event->key.scancode);
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_UpdateTexture(texture, nullptr, framebuffer, WIDTH * sizeof(Uint32));
    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
    running = false;
    if (worker.joinable())
        worker.join();
}
