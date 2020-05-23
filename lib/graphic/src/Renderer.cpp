#include "graphic/Renderer.h"

#include "graphic/TextureManager.h"
#include "graphic/Window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cassert>
#include <iostream>

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

    // -- init SDL_image --
    const int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;

    if(IMG_Init(imgFlags) != imgFlags)
    {
        std::cerr << "SDL_image init failed: " << IMG_GetError() << std::endl;
        exit(-1);
    }

    // -- init TextureManager --
    // TODO decide if keeping this here or move it to a ModuleGraphic
    TextureManager::Create();
}

Renderer::~Renderer()
{
    TextureManager::Destroy();

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

void Renderer::Clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    SDL_SetRenderDrawColor(mSysRenderer, r, g, b, a);
    SDL_RenderClear(mSysRenderer);
}

void Renderer::Finalize()
{
    SDL_RenderPresent(mSysRenderer);
}

} // namespace graphic
} // namespace lib
