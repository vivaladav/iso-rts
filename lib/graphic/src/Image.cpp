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

const int Image::FLIP_NO    = SDL_FLIP_NONE;
const int Image::FLIP_HORIZ = SDL_FLIP_HORIZONTAL;
const int Image::FLIP_VERT  = SDL_FLIP_VERTICAL;

Image::Image(const char * file)
    : mFlip(FLIP_NO)
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

void Image::SetPosition(int x, int y)
{
    mRect->x = x;
    mRect->y = y;
}

void Image::SetX(int x) { mRect->x = x; }

void Image::SetY(int y) { mRect->y = y; }

void Image::Scale(float s)
{
    mRect->w *= s;
    mRect->h *= s;
}

void Image::ScaleW(float s) { mRect->w *= s; }

void Image::ScaleH(float s) { mRect->h *= s; }

void Image::SetFlip(int flip)
{
    if(flip > SDL_FLIP_NONE && flip <= SDL_FLIP_VERTICAL)
        mFlip = flip;
    else
        mFlip = SDL_FLIP_NONE;
}

void Image::Render()
{
    SDL_Renderer * r = Renderer::Instance()->mSysRenderer;
    const SDL_RendererFlip flip = static_cast<SDL_RendererFlip>(mFlip);

    SDL_RenderCopyEx(r, mTex, nullptr, mRect, mRot, nullptr, flip);
}

} // namespace graphic
} // namespace lib
