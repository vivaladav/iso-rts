#include "graphic/Renderer.h"

#include "graphic/Window.h"

#include <SDL2/SDL.h>

#include <cassert>

namespace lib
{
namespace graphic
{

Renderer::Renderer(Window * win)
{
    assert(win);

    mRenderer = SDL_CreateRenderer(static_cast<SDL_Window *>(win->mWin),
                                   -1, SDL_RENDERER_ACCELERATED);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(mRenderer);
}

void Renderer::SetRenderColor(unsigned char r, unsigned char g,
                              unsigned char b, unsigned char a)
{
    SDL_SetRenderDrawColor(mRenderer, r, g, b, a);
}
void Renderer::Clear()
{
    SDL_RenderClear(mRenderer);
}

void Renderer::Finalize()
{
    SDL_RenderPresent(mRenderer);
}

} // namespace graphic
} // namespace lib
