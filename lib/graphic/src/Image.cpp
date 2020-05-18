#include "graphic/Image.h"

#include "graphic/Renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cassert>
#include <iostream>

namespace lib
{
namespace graphic
{

Image::Image(const char * file)
{
    assert(file);

    SDL_Surface * img = IMG_Load(file);

    if(!img)
        std::cout << "IMG_Load: " << IMG_GetError() << std::endl;

    mTex = SDL_CreateTextureFromSurface(Renderer::Instance()->mSysRenderer, img);

    mRect = new SDL_Rect{0, 0, img->w, img->h};

    SDL_FreeSurface(img);
}

Image::~Image()
{
    SDL_DestroyTexture(mTex);

    delete mRect;
}

void Image::Render()
{
    SDL_Renderer * r = Renderer::Instance()->mSysRenderer;

    SDL_RenderCopyEx(r, mTex, nullptr, mRect, 0, nullptr, SDL_FLIP_NONE);
}

} // namespace graphic
} // namespace lib
