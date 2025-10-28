#include <iomanip>
#include <thread>
#include <atomic>

#include "Header/Bus.h"
#include "Header/sm83.h"

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

uint32_t framebuffer[WIDTH * HEIGHT];

const std::array<uint32_t, 4> colorPalette = {0xff0f380f, 0xff306230, 0xff8bac0f, 0xff9bbc0f};

void backgroundTask()
{
    std::shared_ptr<Cartridge> cartridge(new Cartridge("TestRoms/cpu_instrs.gb"));
    Bus bus;
    bus.insertCartridge(cartridge);
    while (running)
    {
        bus.clock();
    }
}

void updatePixel()
{
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            Uint8 r = x;             // Red gradient
            Uint8 g = y;             // Green gradient
            Uint8 b = (x + y) / 2;   // Blue mix
            framebuffer[y * WIDTH + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }
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
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    updatePixel();
    SDL_UpdateTexture(texture, NULL, framebuffer, WIDTH * sizeof(Uint32));
    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);
    SDL_Delay(16);

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
