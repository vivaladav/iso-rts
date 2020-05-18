#include "graphic/Renderer.h"

#include "graphic/Window.h"

#include <SDL2/SDL.h>

#include <cassert>

namespace lib
{
namespace graphic
{

Renderer * Renderer::mInstance = nullptr;

Renderer * Renderer::Create(Window * win)
{
    assert(win);

    if(!mInstance)
        mInstance = new Renderer(win);

    return mInstance;
}

void Renderer::Destroy()
{
    delete mInstance;
    mInstance = nullptr;
}

Renderer::Renderer(Window * win)
{
    SDL_Window * w = static_cast<SDL_Window *>(win->mSysWin);
    mSysRenderer = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(mSysRenderer);
}

void Renderer::SetRenderColor(unsigned char r, unsigned char g,
                              unsigned char b, unsigned char a)
{
    SDL_SetRenderDrawColor(mSysRenderer, r, g, b, a);
}
void Renderer::Clear()
{
    SDL_RenderClear(mSysRenderer);
}

void Renderer::Finalize()
{
    SDL_RenderPresent(mSysRenderer);
}

} // namespace graphic
} // namespace lib
