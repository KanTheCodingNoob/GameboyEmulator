#define SDL_MAIN_USE_CALLBACKS 1
#include "App/SDLDisplay.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("GB Emulator", "1.0", "com.example.gb");

    auto* app = new SDLDisplay();
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
    auto* app = static_cast<SDLDisplay*>(appstate);

    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;

    app->handleEvent(*event);
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto* app = static_cast<SDLDisplay*>(appstate);
    app->iterate();
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    auto* app = static_cast<SDLDisplay*>(appstate);
    delete app;
}
