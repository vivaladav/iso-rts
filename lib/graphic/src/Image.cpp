#include "graphic/Image.h"

#include "graphic/Renderer.h"
#include "graphic/Texture.h"
#include "graphic/TextureManager.h"

#include <SDL2/SDL.h>

#include <cassert>

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

    mTex = TextureManager::Instance()->GetTexture(file);

    mRect = new SDL_Rect{ 0, 0, mTex->GetWidth(), mTex->GetHeight() };
}

Image::~Image()
{
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

int Image::GetWidth() const { return mRect->w; }
int Image::GetHeight() const { return mRect->h; }

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

    SDL_RenderCopyEx(r, mTex->mData, nullptr, mRect, mRot, nullptr, flip);
}

} // namespace graphic
} // namespace lib
