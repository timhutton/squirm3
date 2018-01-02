// SDL2:
#include <SDL2/SDL.h>

// Emscripten:
#include <emscripten.h>

// stdlib:
#include <cstdlib>

// local:
#include "SquirmGrid_SDL.h"

struct context
{
    SDL_Renderer *renderer;
    SquirmGrid_SDL *grid;
    int iteration;
    float scale;
};

void mainloop(void *arg)
{
    context *ctx = static_cast<context*>(arg);
    SDL_Renderer *renderer = ctx->renderer;
    
    const size_t iterations_per_render = 5;
    for(size_t i = 0; i < iterations_per_render; ++i)
    {
        ctx->grid->DoTimeStep();
    }
    ctx->grid->Draw(renderer, ctx->scale);

    SDL_RenderPresent(renderer);

    ctx->iteration++;
}

int main()
{
    const int SIDE_X = 400;
    const int SIDE_Y = 300;
    const float scale = 2.0f;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(SIDE_X*scale, SIDE_Y*scale, 0, &window, &renderer);
    
    SquirmGrid_SDL grid;
    grid.Create(SIDE_X,SIDE_Y);
    const float DENSITY = 200.0F/(200.0F*200.0F);
    grid.InitSimple((int)(SIDE_X*SIDE_Y*DENSITY));

    context ctx;
    ctx.renderer = renderer;
    ctx.iteration = 0;
    ctx.grid = &grid;
    ctx.scale = scale;

    const int simulate_infinite_loop = 1; // call the function repeatedly
    const int fps = -1; // call the function as fast as the browser wants to render (typically 60fps)
    emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
